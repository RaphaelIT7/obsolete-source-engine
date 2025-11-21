//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#if !defined( PACKED_ENTITY_H )
#define PACKED_ENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include <const.h>
#include <basetypes.h>
#include <mempool.h>
#include <utlvector.h>
#include <tier0/dbg.h>

#include "common.h"

// Matched with the memdbgoff at end of header
#include "memdbgon.h"

// This is extra spew to the files cltrace.txt + svtrace.txt
// #define DEBUG_NETWORKING 1

#if defined( DEBUG_NETWORKING )
#include "convar.h"
void SpewToFile( PRINTF_FORMAT_STRING char const* pFmt, ... );
extern ConVar  sv_packettrace;
#define TRACE_PACKET( text ) if ( sv_packettrace.GetInt() ) { SpewToFile text ; };
#else
#define TRACE_PACKET( text )
#endif

enum
{
	// dimhotepus: Bump from 9999 to int_max.
	ENTITY_SENTINEL = INT_MAX	// larger number than any real entity number
};

class SendTable;
class RecvTable;
class ServerClass;
class ClientClass;



// Replaces entity_state_t.
// This is what we send to clients.

class PackedEntity
{
public:
	PackedEntity();
	~PackedEntity();
	
	int			GetNumBits() const;
	int			GetNumBytes() const;

	// Access the data in the entity.
	void*		GetData();
	void		FreeData();

	// Copy the data into the PackedEntity's data and make sure the # bytes allocated is
	// an integer multiple of 4.
	bool		AllocAndCopyPadded( const void *pData, intp size );

	void SetPackedData( int nSize )
	{
		m_nBytes = nSize;
		m_pData = malloc( nSize );
	};

	void				SetSnapshotCreationTick( int nTick );
	int					GetSnapshotCreationTick() const;

	void				SetShouldCheckCreationTick( bool bState );
	bool				ShouldCheckCreationTick() const;

	void				SetServerAndClientClass( ServerClass *pServerClass, ClientClass *pClientClass );

public:
	ServerClass *m_pServerClass;	// Valid on the server
	ClientClass	*m_pClientClass;	// Valid on the client
		
	int			m_nEntityIndex;		// Entity index.
	int			m_ReferenceCount;	// reference count;

	int					m_nBytes;
	void				*m_pData;

private:
	// This is the tick this PackedEntity was created on
	unsigned int		m_nSnapshotCreationTick : 31;
	unsigned int		m_nShouldCheckCreationTick : 1;
};

inline int PackedEntity::GetNumBits() const
{
	return m_nBytes * 8;
}

inline int PackedEntity::GetNumBytes() const
{
	return m_nBytes; 
}

inline void* PackedEntity::GetData()
{
	return m_pData;
}

inline void PackedEntity::SetSnapshotCreationTick( int nTick )
{
	m_nSnapshotCreationTick = (unsigned int)nTick;
}

inline int PackedEntity::GetSnapshotCreationTick() const
{
	return (int)m_nSnapshotCreationTick;
}

inline void PackedEntity::SetShouldCheckCreationTick( bool bState )
{
	m_nShouldCheckCreationTick = bState ? 1 : 0;
}

inline bool PackedEntity::ShouldCheckCreationTick() const
{
	return m_nShouldCheckCreationTick == 1 ? true : false;
}

inline void PackedEntity::FreeData()
{
	if ( m_pData )
	{
		free(m_pData);
		m_pData = NULL;
	}
}

#include "memdbgoff.h"

#endif // PACKED_ENTITY_H

