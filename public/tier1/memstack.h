//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A fast stack memory allocator that uses virtual memory if available
//
//=============================================================================//

#ifndef MEMSTACK_H
#define MEMSTACK_H

#include "tier0/platform.h"

#if defined( _WIN32 )
#pragma once
#endif

//-----------------------------------------------------------------------------

typedef size_t MemoryStackMark_t;

class CMemoryStack
{
public:
	CMemoryStack();
	~CMemoryStack();

	bool Init( size_t maxSize = 0, size_t commitSize = 0, size_t initialCommit = 0, size_t alignment = 16 );
	void Term();

	intp GetSize() const;
	intp GetMaxSize() const;
	intp GetUsed() const;
	
	RESTRICT_FUNC void * Alloc(size_t bytes, bool bClear = false);

	MemoryStackMark_t GetCurrentAllocPoint();
	void FreeToAllocPoint( MemoryStackMark_t mark, bool bDecommit = true );
	void FreeAll( bool bDecommit = true );
	
	void Access( void **ppRegion, size_t *pBytes );

	void PrintContents() const;

	void *GetBase();
	const void *GetBase() const {  return const_cast<CMemoryStack *>(this)->GetBase(); }

private:
	bool CommitTo( byte * RESTRICT );

	byte *m_pNextAlloc;
	byte *m_pCommitLimit;
	byte *m_pAllocLimit;
	
	byte *m_pBase;

	size_t m_maxSize;
	size_t m_alignment;
#ifdef _WIN32
	size_t m_commitSize;
	size_t m_minCommit;
#endif
};

//-------------------------------------

FORCEINLINE RESTRICT_FUNC void * CMemoryStack::Alloc( size_t bytes, bool bClear )
{
	Assert( m_pBase );

	size_t alignment = m_alignment;
	if ( bytes )
	{
		bytes = AlignValue( bytes, alignment );
	}
	else
	{
		bytes = alignment;
	}


	void *pResult = m_pNextAlloc;
	byte * RESTRICT pNextAlloc = m_pNextAlloc + bytes;

	if ( pNextAlloc > m_pCommitLimit )
	{
		if ( !CommitTo( pNextAlloc ) )
		{
			return NULL;
		}
	}

	if ( bClear )
	{
		memset( pResult, 0, bytes );
	}

	m_pNextAlloc = pNextAlloc;

	return pResult;
}

//-------------------------------------

inline intp CMemoryStack::GetMaxSize() const
{ 
	return m_maxSize;
}

//-------------------------------------

inline intp CMemoryStack::GetUsed() const
{ 
	return ( m_pNextAlloc - m_pBase );
}

//-------------------------------------

inline void *CMemoryStack::GetBase()
{
	return m_pBase;
}

//-------------------------------------

inline MemoryStackMark_t CMemoryStack::GetCurrentAllocPoint()
{
	return ( m_pNextAlloc - m_pBase );
}

//-----------------------------------------------------------------------------
// The CUtlMemoryStack class:
// A fixed memory class
//-----------------------------------------------------------------------------
template< typename T, typename I, size_t MAX_SIZE, size_t COMMIT_SIZE = 0, size_t INITIAL_COMMIT = 0 >
class CUtlMemoryStack
{
public:
	// constructor, destructor
	CUtlMemoryStack( intp nGrowSize = 0, intp nInitSize = 0 )
	{ 
		static_assert( sizeof(T) % 4 == 0 );
		m_MemoryStack.Init( MAX_SIZE * sizeof(T), COMMIT_SIZE * sizeof(T), INITIAL_COMMIT * sizeof(T), 4 );
		// dimhotepus: Add zero init.
		m_nAllocated = 0;
	}
	CUtlMemoryStack( T* pMemory, int numElements ) = delete;

	// Can we use this index?
	bool IsIdxValid( I i ) const							{ return (i >= 0) && (i < m_nAllocated); }

	// Specify the invalid ('null') index that we'll only return on failure
	static const I INVALID_INDEX = ( I )-1; // For use with COMPILE_TIME_ASSERT
	static I InvalidIndex() { return INVALID_INDEX; }

	class Iterator_t
	{
		Iterator_t( I i ) : index( i ) {}
		I index;
		friend class CUtlMemoryStack<T,I,MAX_SIZE, COMMIT_SIZE, INITIAL_COMMIT>;
	public:
		bool operator==( const Iterator_t it ) const		{ return index == it.index; }
		bool operator!=( const Iterator_t it ) const		{ return index != it.index; }
	};
	Iterator_t First() const								{ return Iterator_t( m_nAllocated ? 0 : InvalidIndex() ); }
	Iterator_t Next( const Iterator_t &it ) const			{ return Iterator_t( it.index < m_nAllocated ? it.index + 1 : InvalidIndex() ); }
	I GetIndex( const Iterator_t &it ) const				{ return it.index; }
	bool IsIdxAfter( I i, const Iterator_t &it ) const		{ return i > it.index; }
	bool IsValidIterator( const Iterator_t &it ) const		{ return it.index >= 0 && it.index < m_nAllocated; }
	Iterator_t InvalidIterator() const						{ return Iterator_t( InvalidIndex() ); }

	// Gets the base address
	T* Base()												{ return (T*)m_MemoryStack.GetBase(); }
	const T* Base() const									{ return (const T*)m_MemoryStack.GetBase(); }

	// element access
	T& operator[]( I i )									{ Assert( IsIdxValid(i) ); return Base()[i];	}
	const T& operator[]( I i ) const						{ Assert( IsIdxValid(i) ); return Base()[i];	}
	T& Element( I i )										{ Assert( IsIdxValid(i) ); return Base()[i];	}
	const T& Element( I i ) const							{ Assert( IsIdxValid(i) ); return Base()[i];	}

	// Attaches the buffer to external memory....
	void SetExternalBuffer( T* pMemory, intp numElements )	{ Assert( 0 ); }

	// Size
	intp NumAllocated() const								{ return m_nAllocated; }
	intp Count() const										{ return m_nAllocated; }

	// Grows the memory, so that at least allocated + num elements are allocated
	void Grow( intp num = 1 )								{ Assert( num > 0 ); m_nAllocated += num; m_MemoryStack.Alloc( num * sizeof(T) ); }

	// Makes sure we've got at least this much memory
	void EnsureCapacity( intp num )							{ Assert( num <= static_cast<intp>(MAX_SIZE) ); if ( m_nAllocated < num ) Grow( num - m_nAllocated ); }

	// Memory deallocation
	void Purge()											{ m_MemoryStack.FreeAll(); m_nAllocated = 0; }

	// is the memory externally allocated?
	bool IsExternallyAllocated() const						{ return false; }

	// Set the size by which the memory grows
	void SetGrowSize( intp size )							{}

private:
	CMemoryStack m_MemoryStack;
	intp m_nAllocated;
};

//-----------------------------------------------------------------------------

#endif // MEMSTACK_H
