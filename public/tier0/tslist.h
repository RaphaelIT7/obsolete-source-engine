// Copyright Valve Corporation, All rights reserved.
//
// LIFO from disassembly of Windows API and
// http://jim.afim-asso.org/jim2002/articles/L17_Fober.pdf
// FIFO from
// http://jim.afim-asso.org/jim2002/articles/L17_Fober.pdf

#ifndef TIER0_TSLIST_H_
#define TIER0_TSLIST_H_

#if defined( USE_NATIVE_SLIST )
#include "winlite.h"
#endif

#include "tier0/dbg.h"
#include "tier0/threadtools.h"
#include "tier0/memalloc.h"
#include "tier0/memdbgoff.h"

//-----------------------------------------------------------------------------

#if defined( PLATFORM_64BITS )

#if defined (PLATFORM_WINDOWS) && defined( _XM_SSE_INTRINSICS_ )
#else  // PLATFORM_WINDOWS
typedef __int128_t int128;
#define int128_zero() 0
#endif// PLATFORM_WINDOWS

#define TSLIST_HEAD_ALIGNMENT 16
#define TSLIST_NODE_ALIGNMENT 16

#ifdef POSIX
inline bool ThreadInterlockedAssignIf128( int128 volatile * pDest, const int128 &value, const int128 &comparand ) 
{
	// We do not want the original comparand modified by the swap
	// so operate on a local copy.
	int128 local_comparand = comparand;
	return __sync_bool_compare_and_swap( pDest, local_comparand, value );
}
#endif

inline bool ThreadInterlockedAssignIf64x128( volatile int128 *pDest, const int128 &value, const int128 &comperand )
	{ return ThreadInterlockedAssignIf128( pDest, value, comperand ); }
#else

#define TSLIST_HEAD_ALIGNMENT 8
#define TSLIST_NODE_ALIGNMENT 8

inline bool ThreadInterlockedAssignIf64x128( volatile int64 *pDest, const int64 value, const int64 comperand )
	{ return ThreadInterlockedAssignIf64( pDest, value, comperand ); }

#endif

#define TSLIST_HEAD_ALIGN alignas(TSLIST_HEAD_ALIGNMENT)
#define TSLIST_NODE_ALIGN alignas(TSLIST_NODE_ALIGNMENT)
#define TSLIST_HEAD_ALIGN_POST
#define TSLIST_NODE_ALIGN_POST

// Lock free stack node.
struct TSLIST_NODE_ALIGN TSLNodeBase_t
{
	TSLNodeBase_t *Next; // name to match Windows
};

// Lock free stack head.
union TSLIST_HEAD_ALIGN TSLHead_t
{
	struct Value_t
	{
		TSLNodeBase_t *Next;
		int16   Depth;
		int16	Sequence;

#ifdef PLATFORM_64BITS
		int32   Padding;
#endif
	} value;

	struct Value32_t
	{
		TSLNodeBase_t *Next_do_not_use_me;
		int32   DepthAndSequence;
	} value32;

#ifdef PLATFORM_64BITS
	int128 value64x128;
#else
	int64 value64x128;
#endif
};

// Basic lock free stack.
class TSLIST_HEAD_ALIGN CTSListBase : public CAlignedNewDelete<TSLIST_HEAD_ALIGNMENT> {
 public:
  CTSListBase() : head_{TSLHead_t{nullptr, 0}} {
    if (((size_t)&head_) % TSLIST_HEAD_ALIGNMENT != 0) {
      Error("CTSListBase: Misaligned list head.\n");
    }
  }

  ~CTSListBase() { Detach(); }

  TSLNodeBase_t *Push(TSLNodeBase_t *node) {
    TSLHead_t next = {}, orig = head_.load(std::memory_order::memory_order_relaxed);

    do {
      node->Next = orig.value.Next;

      next.value.Next = node;
      next.value.Depth = orig.value.Depth + 1;
	  // Solve ABA problem.
      next.value.Sequence = orig.value.Sequence + 1;
    } while (!head_.compare_exchange_weak(
        orig, next, std::memory_order::memory_order_acq_rel,
        std::memory_order::memory_order_relaxed));

    return orig.value.Next;
  }

  TSLNodeBase_t *Pop() {
    TSLHead_t next = {}, orig = head_.load(std::memory_order::memory_order_relaxed);

    do {
      if (orig.value.Next == nullptr) return nullptr;

	  // dimhotepus: Same bug as in original code.
	  // Here is data race, if old head was deleted between nullptr check and
	  // ->Next it accesses stall data!
      next.value.Next = orig.value.Next->Next;
      next.value.Depth = orig.value.Depth - 1;
	  // No need to solve ABA problem, solved in Push.
    } while (!head_.compare_exchange_weak(
        orig, next, std::memory_order::memory_order_acq_rel,
        std::memory_order::memory_order_relaxed));

    return orig.value.Next;
  }

  TSLNodeBase_t *Detach() {
    TSLHead_t next = {}, orig = head_.load(std::memory_order::memory_order_relaxed);

#ifdef USE_NATIVE_SLIST
		return (TSLNodeBase_t *)InterlockedPushEntrySList( &m_Head, pNode );
#else
		TSLHead_t oldHead;
		TSLHead_t newHead;

    return orig.value.Next;
  }

#ifdef PLATFORM_64BITS
		newHead.value.Padding = 0;
#endif
		for (;;)
		{
			oldHead.value64x128 = m_Head.value64x128;
			pNode->Next = oldHead.value.Next;
			newHead.value.Next = pNode;
			
			newHead.value32.DepthAndSequence = oldHead.value32.DepthAndSequence + 0x10001;
			
			
			if ( ThreadInterlockedAssignIf64x128( &m_Head.value64x128, newHead.value64x128, oldHead.value64x128 ) )
			{
				break;
			}
			ThreadPause();
		}

  [[nodiscard]] int Count() const {
    TSLHead_t orig = head_.load(std::memory_order::memory_order_relaxed);
    return orig.value.Depth;
  }

	TSLNodeBase_t *Pop()
	{
#ifdef USE_NATIVE_SLIST
		TSLNodeBase_t *pNode = (TSLNodeBase_t *)InterlockedPopEntrySList( &m_Head );
		return pNode;
#else
		TSLHead_t oldHead;
		TSLHead_t newHead;

#ifdef PLATFORM_64BITS
		newHead.value.Padding = 0;
#endif
		for (;;)
		{
			oldHead.value64x128 = m_Head.value64x128;
			if ( !oldHead.value.Next )
				return NULL;

			newHead.value.Next = oldHead.value.Next->Next;
			newHead.value32.DepthAndSequence = oldHead.value32.DepthAndSequence	- 1;


			if ( ThreadInterlockedAssignIf64x128( &m_Head.value64x128, newHead.value64x128, oldHead.value64x128 ) )
			{
				#if defined( PLATFORM_PS3 ) || defined( PLATFORM_X360 )
					__lwsync(); // read-acquire barrier
				#endif
				break;
			}
			ThreadPause();
		}

		return (TSLNodeBase_t *)oldHead.value.Next;
#endif
	}

	TSLNodeBase_t *Detach()
	{
#ifdef USE_NATIVE_SLIST
		TSLNodeBase_t *pBase = (TSLNodeBase_t *)InterlockedFlushSList( &m_Head );
		return pBase;
#else
		TSLHead_t oldHead;
		TSLHead_t newHead;

#ifdef PLATFORM_64BITS
		newHead.value.Padding = 0;
#endif
		do
		{
			ThreadPause();

			oldHead.value64x128 = m_Head.value64x128;
			if ( !oldHead.value.Next )
				return NULL;

			newHead.value.Next = NULL;
			// <sergiy> the reason for AND'ing it instead of poking a short into memory 
			//          is probably to avoid store forward issues, but I'm not sure because
			//          I didn't construct this code. In any case, leaving it as is on big-endian
			newHead.value32.DepthAndSequence = oldHead.value32.DepthAndSequence & 0xffff0000;

		} while( !ThreadInterlockedAssignIf64x128( &m_Head.value64x128, newHead.value64x128, oldHead.value64x128 ) );

		return (TSLNodeBase_t *)oldHead.value.Next;
#endif
	}

	TSLHead_t *AccessUnprotected()
	{
		return &m_Head;
	}

	int Count() const
	{
#ifdef USE_NATIVE_SLIST
		return QueryDepthSList( const_cast<TSLHead_t*>( &m_Head ) );
#else
		return m_Head.value.Depth;
#endif
	}

private:
	TSLHead_t m_Head;
} TSLIST_HEAD_ALIGN_POST;

//-------------------------------------

template <typename T>
class TSLIST_HEAD_ALIGN CTSSimpleList : public CTSListBase
{
public:
	void Push( T *pNode )
	{
		Assert( sizeof(T) >= sizeof(TSLNodeBase_t) );
		CTSListBase::Push( pNode );
	}

	T *Pop()
	{
		return static_cast<T *>(CTSListBase::Pop());
	}
};

// Replacement for CTSList<> and CObjectPool<> that does not have a per-item,
// per-alloc new/delete overhead similar to CTSSimpleList except that it
// allocates it's own pool objects and frees them on destruct.  Also it does not
// overlay the TSLNodeBase_t memory on T's memory.
template< class T > 
class TSLIST_HEAD_ALIGN CTSPool : public CTSListBase
{
	// packs the node and the item (T) into a single struct and pools those
	struct TSLIST_NODE_ALIGN TSPoolNode_t :
		public CAlignedNewDelete<TSLIST_NODE_ALIGNMENT, TSLNodeBase_t>
	{
		T elem;
	};

public:
	~CTSPool()
	{
		Purge();
	}

	void Purge()
	{
		while ( true )
		{
			auto *pNode = static_cast<TSPoolNode_t *>(CTSListBase::Pop());
			if ( !pNode )
				break;

			delete pNode;
		}
	}

	void PutObject( T *pInfo )
	{
		auto *pElem = reinterpret_cast<unsigned char *>(pInfo);
		pElem -= offsetof(TSPoolNode_t, elem);
		auto *pNode = reinterpret_cast<TSPoolNode_t *>(pElem);

		CTSListBase::Push( pNode );
	}

	T *GetObject()
	{
		auto *pNode = static_cast<TSPoolNode_t *>(CTSListBase::Pop());
		if ( !pNode )
		{
			pNode = new TSPoolNode_t;
		}
		return &pNode->elem;
	}

	// omg windows sdk - why do you #define GetObject()?
	FORCEINLINE T *Get()
	{
		return GetObject();
	}
};

// Lock free typed list.
template <typename T>
class TSLIST_HEAD_ALIGN CTSList : public CTSListBase
{
public:
	struct TSLIST_NODE_ALIGN Node_t : public CAlignedNewDelete<TSLIST_NODE_ALIGNMENT, TSLNodeBase_t>
	{
		Node_t() = default;
		Node_t( const T &init ) : elem( init ) {}

		T elem;

	    // override new/delete so we can guarantee 8-byte aligned allocs
	    static void * operator new( size_t size )
	    {
      		Node_t *pNode = (Node_t *)MemAlloc_AllocAligned( size, TSLIST_NODE_ALIGNMENT, __FILE__, __LINE__ );
			return pNode;
	    }

		// override new/delete so we can guarantee 8-byte aligned allocs
		static void * operator new( size_t size, int, const char *pFileName, int nLine )
		{
			Node_t *pNode = (Node_t *)MemAlloc_AllocAligned( size, TSLIST_NODE_ALIGNMENT, pFileName, nLine );
			return pNode;
		}

	    static void operator delete( void *p)
	    {
			MemAlloc_FreeAligned( p );
	    }
		static void operator delete( void *p, int, const char *, int )
		{
			MemAlloc_FreeAligned( p );
		}

	} TSLIST_NODE_ALIGN_POST;

	~CTSList()
	{
		Purge();
	}

	void Purge()
	{
		Node_t *pCurrent = Detach();
		while ( pCurrent )
		{
			Node_t *pNext = (Node_t *)pCurrent->Next;
			delete pCurrent;
			pCurrent = pNext;
		}
	}

	void RemoveAll()
	{
		Purge();
	}

	Node_t *Push( Node_t *pNode )
	{
		return static_cast<Node_t *>(CTSListBase::Push( pNode ));
	}

	Node_t *Pop()
	{
		return static_cast<Node_t *>(CTSListBase::Pop());
	}

	void PushItem( const T &init )
	{
		Push( new Node_t( init ) );
	}

	bool PopItem( T *pResult)
	{
		Node_t *pNode = Pop();
		if ( !pNode )
			return false;

		*pResult = pNode->elem;
		delete pNode;
		return true;
	}

	Node_t *Detach()
	{
		return static_cast<Node_t *>(CTSListBase::Detach());
	}
};

// Lock free list with list for free nodes.
template <typename T>
class TSLIST_HEAD_ALIGN CTSListWithFreeList : public CTSListBase
{
public:
	struct TSLIST_NODE_ALIGN Node_t : public TSLNodeBase_t
	{
		Node_t() = default;
		Node_t( const T &init ) : elem( init ) {}

		T elem;
	};

	~CTSListWithFreeList()
	{
		Purge();
	}

	void Purge()
	{
		Node_t *pCurrent = Detach();
		while ( pCurrent )
		{
			auto *pNext = static_cast<Node_t *>(pCurrent->Next);
			delete pCurrent;
			pCurrent = pNext;
		}
		pCurrent = static_cast<Node_t *>(m_FreeList.Detach());
		while ( pCurrent )
		{
			auto *pNext = static_cast<Node_t *>(pCurrent->Next);
			delete pCurrent;
			pCurrent = pNext;
		}
	}

	void RemoveAll()
	{
		Node_t *pCurrent = Detach();
		while ( pCurrent )
		{
			auto *pNext = static_cast<Node_t *>(pCurrent->Next);
			m_FreeList.Push( pCurrent );
			pCurrent = pNext;
		}
	}

	Node_t *Push( Node_t *pNode )
	{
		return static_cast<Node_t *>(CTSListBase::Push( pNode ));
	}

	Node_t *Pop()
	{
		return static_cast<Node_t *>(CTSListBase::Pop());
	}

	void PushItem( const T &init )
	{
		auto *pNode = static_cast<Node_t *>(m_FreeList.Pop());
		if ( !pNode )
		{
			pNode = new Node_t;
		}
		pNode->elem = init;
		Push( pNode );
	}

	bool PopItem( T *pResult)
	{
		Node_t *pNode = Pop();
		if ( !pNode )
			return false;

		*pResult = pNode->elem;
		m_FreeList.Push( pNode );
		return true;
	}

	Node_t *Detach()
	{
		return static_cast<Node_t *>(CTSListBase::Detach());
	}

	void FreeNode( Node_t *pNode )
	{
		m_FreeList.Push( pNode );
	}

private:
	CTSListBase m_FreeList;
};

// Lock free queue.
//
// A special consideration: the element type should be simple.  This code
// actually dereferences freed nodes as part of pop, but later detects that.
// If the item in the queue is a complex type, only bad things can come of that.
// 
// Also, therefore, if you're using Push/Pop instead of push item, be aware that
// the node memory cannot be freed until all threads that might have been
// popping have completed the pop.  The PushItem()/PopItem() for handles this by
// keeping a persistent free list.  Don't mix Push/PushItem.  Note also nodes
// will be freed at the end, and are expected to have been allocated with
// operator new.
template <typename T, bool bTestOptimizer = false>
class TSLIST_NODE_ALIGN CTSQueue : public CAlignedNewDelete<TSLIST_NODE_ALIGNMENT>
{
public:
	struct TSLIST_NODE_ALIGN Node_t : public CAlignedNewDelete<TSLIST_NODE_ALIGNMENT>
	{
		CTSQueue *pNode = (CTSQueue *)MemAlloc_AllocAligned( size, TSLIST_NODE_ALIGNMENT, __FILE__, __LINE__ );
		return pNode;
	}

	// override new/delete so we can guarantee 8-byte aligned allocs
	static void * operator new( size_t size, int, const char *pFileName, int nLine )
	{
		CTSQueue *pNode = (CTSQueue *)MemAlloc_AllocAligned( size, TSLIST_NODE_ALIGNMENT, pFileName, nLine );
		return pNode;
	}

	static void operator delete( void *p)
	{
		MemAlloc_FreeAligned( p );
	}

	static void operator delete( void *p, int nBlockUse, const char *pFileName, int nLine )
	{
		MemAlloc_FreeAligned( p );
	}

private:
	// These ain't gonna work
	static void * operator new[] ( size_t size )
	{
		return NULL;
	}

	static void operator delete [] ( void *p)
	{
	}

public:

	struct TSLIST_NODE_ALIGN Node_t
	{
		// override new/delete so we can guarantee 8-byte aligned allocs
		static void * operator new( size_t size )
		{
			Node_t *pNode = (Node_t *)MemAlloc_AllocAligned( size, TSLIST_NODE_ALIGNMENT, __FILE__, __LINE__ );
			return pNode;
		}

		static void * operator new( size_t size, int nBlockUse, const char *pFileName, int nLine )
		{
			Node_t *pNode = (Node_t *)MemAlloc_AllocAligned( size, TSLIST_NODE_ALIGNMENT, pFileName, nLine );
			return pNode;
		}

		static void operator delete( void *p)
		{
			MemAlloc_FreeAligned( p );
		}

		static void operator delete( void *p, int nBlockUse, const char *pFileName, int nLine )
		{
			MemAlloc_FreeAligned( p );
		}

		Node_t() = default;
		Node_t( const T &init ) : pNext{nullptr}, elem{init} {}

		Node_t *pNext;
		T elem;
	};

	union TSLIST_HEAD_ALIGN NodeLink_t
	{
		// override new/delete so we can guarantee {8,16}-byte aligned allocs
		static void * operator new( size_t size )
		{
			auto *link = static_cast<NodeLink_t *>(MemAlloc_AllocAligned( size, TSLIST_HEAD_ALIGNMENT, __FILE__, __LINE__ ));
			return link;
		}

		static void operator delete( void *p )
		{
			MemAlloc_FreeAligned( p );
		}

		struct Value_t
		{
			Node_t *pNode;
			intp	sequence;
		} value;

#ifdef PLATFORM_64BITS
		int128 value64x128;
#else
		int64 value64x128;
#endif
	};

	CTSQueue()
	{
		static_assert( sizeof(Node_t) >= sizeof(TSLNodeBase_t) );

		if ( ((size_t)&m_Head) % TSLIST_HEAD_ALIGNMENT != 0 )
		{
			Error( "CTSQueue: Misaligned queue head %p.\n", &m_Head );
		}
		if ( ((size_t)&m_Tail) % TSLIST_HEAD_ALIGNMENT != 0 )
		{
			Error( "CTSQueue: Misaligned queue tail %p.\n", &m_Tail );
		}

		m_Count.store(0, std::memory_order::memory_order_relaxed );
		
		 // list always contains a dummy node
		m_Head.value.pNode = m_Tail.value.pNode = new Node_t;
		m_Head.value.pNode->pNext = End();
		m_Head.value.sequence = m_Tail.value.sequence = 0;
	}

	~CTSQueue()
	{
		Purge();

		Assert( m_Count.load( std::memory_order::memory_order_relaxed ) == 0 );
		Assert( m_Head.value.pNode == m_Tail.value.pNode );
		Assert( m_Head.value.pNode->pNext == End() );

		delete m_Head.value.pNode;
	}

	// Note: Purge, RemoveAll, and Validate are *not* threadsafe
	void Purge()
	{
#ifdef _DEBUG
		ValidateQueue();
#endif

		Node_t *pNode;
		while ( ( pNode = Pop() ) != nullptr )
		{
			delete pNode;
		}

		while ( ( pNode = reinterpret_cast<Node_t *>(m_FreeNodes.Pop()) ) != nullptr )
		{
			delete pNode;
		}

		Assert( m_Count.load() == 0 );
		Assert( m_Head.value.pNode == m_Tail.value.pNode );
		Assert( m_Head.value.pNode->pNext == End() );

		m_Head.value.sequence = m_Tail.value.sequence = 0;
	}

	void RemoveAll()
	{
#ifdef _DEBUG
		ValidateQueue();
#endif

		Node_t *pNode;
		while ( ( pNode = Pop() ) != nullptr )
		{
			m_FreeNodes.Push( (TSLNodeBase_t *)pNode );
		}
	}

	bool ValidateQueue()
	{
#ifdef _DEBUG
		{
			bool bResult = true;
			int nNodes = 0;
			if ( m_Tail.value.pNode->pNext != End() )
			{
				DebuggerBreakIfDebugging();
				bResult = false;
			}

			if ( m_Count.load( std::memory_order::memory_order_relaxed ) == 0 )
			{
				if ( m_Head.value.pNode != m_Tail.value.pNode )
				{
					DebuggerBreakIfDebugging();
					bResult = false;
				}
			}

			Node_t *pNode = m_Head.value.pNode;
			while ( pNode != End() )
			{
				nNodes++;
				pNode = pNode->pNext;
			}

			nNodes--;// skip dummy node

			if ( nNodes != m_Count.load( std::memory_order::memory_order_relaxed) )
			{
				DebuggerBreakIfDebugging();
				bResult = false;
			}

			if ( !bResult )
			{
				Warning( "Corrupt CTSQueue %p detected.\n", this );
			}

			return bResult;
		}
#else
		return true;
#endif
	}

	void FinishPush( Node_t *pNode, const NodeLink_t &oldTail )
	{
		NodeLink_t newTail;

		newTail.value.pNode = pNode;
		newTail.value.sequence = oldTail.value.sequence + 1;

		ThreadMemoryBarrier();

		InterlockedCompareExchangeNodeLink( &m_Tail, newTail, oldTail );
	}

	Node_t *Push( Node_t *pNode )
	{
#ifdef _DEBUG
		if ( (size_t)pNode % TSLIST_NODE_ALIGNMENT != 0 )
		{
			Error( "CTSQueue: Misaligned node %p.\n", pNode );
		}
#endif

		NodeLink_t oldTail;

		pNode->pNext = End();

		for (;;)
		{
			oldTail.value.sequence = m_Tail.value.sequence;
			oldTail.value.pNode = m_Tail.value.pNode;
			if ( InterlockedCompareExchangeNode( &(oldTail.value.pNode->pNext), pNode, End() ) == End() ) //-V1051
			{
				break;
			}
			else
			{
				// Another thread is trying to push, help it along
				FinishPush( oldTail.value.pNode->pNext, oldTail );
			}
		}

		// This can fail if another thread pushed between the sequence and node grabs above. Later pushes or pops corrects
		FinishPush( pNode, oldTail ); 

		m_Count.fetch_add(1, std::memory_order::memory_order_relaxed);

		return oldTail.value.pNode;
	}

	Node_t *Pop()
	{
		// dimhtepus: x64 support.
#ifdef PLATFORM_64BITS
		Node_t * TSQUEUE_BAD_NODE_LINK = (Node_t *)(void*)0xdeadbeefdeadbeef;
#else
		Node_t * TSQUEUE_BAD_NODE_LINK = (Node_t *)(void*)0xdeadbeef;
#endif
		NodeLink_t * volatile		pHead = &m_Head;
		NodeLink_t * volatile		pTail = &m_Tail;
		Node_t * volatile *			pHeadNode = &m_Head.value.pNode;
		volatile intp * volatile	pHeadSequence = &m_Head.value.sequence;
		Node_t * volatile * 		pTailNode = &pTail->value.pNode;

		NodeLink_t head;
		NodeLink_t newHead;
		Node_t *pNext;
		intp tailSequence;
		T elem;

		for (;;)
		{
			head.value.sequence = *pHeadSequence; // must grab sequence first, which allows condition below to ensure pNext is valid
			ThreadMemoryBarrier(); // need a barrier to prevent reordering of these assignments
			head.value.pNode	= *pHeadNode;
			tailSequence		= pTail->value.sequence;
			pNext				= head.value.pNode->pNext;

			// Checking pNext only to force optimizer to not reorder the assignment
			// to pNext and the compare of the sequence
			if ( !pNext || head.value.sequence != *pHeadSequence ) 
				continue;

			if ( bTestOptimizer )
			{
				if ( pNext == TSQUEUE_BAD_NODE_LINK )
				{
					Warning( "TSQueue: Bad node link detected.\n" );
					continue;
				}
			}

			if ( head.value.pNode == *pTailNode )
			{
				if ( pNext == End() )
					return NULL;

				// Another thread is trying to push, help it along
				NodeLink_t &oldTail = head; // just reuse local memory for head to build old tail
				oldTail.value.sequence = tailSequence; // reuse head pNode
				FinishPush( pNext, oldTail );
				continue;
			}
			
			if ( pNext != End() )
			{
				elem = pNext->elem; // NOTE: next could be a freed node here, by design
				newHead.value.pNode = pNext;
				newHead.value.sequence = head.value.sequence + 1;
				if ( InterlockedCompareExchangeNodeLink( pHead, newHead, head ) )
				{
					ThreadMemoryBarrier();
					if ( bTestOptimizer )
					{
						head.value.pNode->pNext = TSQUEUE_BAD_NODE_LINK;
					}
					break;
				}
			}
		}

		m_Count.fetch_sub(1, std::memory_order::memory_order_relaxed);
		head.value.pNode->elem = elem;
		return head.value.pNode;
	}

	void FreeNode( Node_t *pNode )
	{
		m_FreeNodes.Push( reinterpret_cast<Node_t *>(pNode) );
	}

	void PushItem( const T &init )
	{
		auto *pNode = reinterpret_cast<Node_t *>(m_FreeNodes.Pop());
		if ( pNode )
		{
			pNode->elem = init;
		}
		else
		{
			pNode = new Node_t( init );
		}
		Push( pNode );
	}

	bool PopItem( T *pResult )
	{
		Node_t *pNode = Pop();
		if ( !pNode )
			return false;

		*pResult = pNode->elem;
		m_FreeNodes.Push( reinterpret_cast<TSLNodeBase_t *>(pNode) );
		return true;
	}

	int Count() const
	{
		return m_Count.load(std::memory_order::memory_order_relaxed);
	}

private:
	// just need a unique signifier
	Node_t *End() { return &m_end; }

	[[nodiscard]] static Node_t *InterlockedCompareExchangeNode( Node_t * volatile *ppNode, Node_t *value, Node_t *comperand )
	{
		return (Node_t *)::ThreadInterlockedCompareExchangePointer( (void * volatile *)ppNode, value, comperand );
	}

	bool InterlockedCompareExchangeNodeLink( NodeLink_t volatile *pLink, const NodeLink_t &value, const NodeLink_t &comperand )
	{
		return ThreadInterlockedAssignIf64x128( &pLink->value64x128, value.value64x128, comperand.value64x128 );
	}

	NodeLink_t m_Head;
	NodeLink_t m_Tail;

	std::atomic_int m_Count;
	
	CTSListBase m_FreeNodes;

#endif  // TIER0_TSLIST_H_
