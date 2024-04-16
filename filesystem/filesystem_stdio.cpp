//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include "basefilesystem.h"
#include "packfile.h"
#include "tier0/dbg.h"
#include "tier0/threadtools.h"
#ifdef _WIN32
#include "tier0/tslist.h"
#elif defined(POSIX)
#include <fcntl.h>
#ifdef LINUX
#include <sys/file.h>
#endif
#endif
#include "tier1/convar.h"
#include "tier0/vcrmode.h"
#include "tier0/vprof.h"
#include "tier1/fmtstr.h"
#include "tier1/utlrbtree.h"
#include "vstdlib/osversion.h"
#include "steam/isteamugc.h"

#include <GarrysMod/IGet.h>
#include <GarrysMod/IMenuSystem.h>
#include <GarrysMod/Lua/LuaShared.h>
#include <GarrysMod/Lua/LuaInterface.h>
#include "tier1/KeyValues.h"

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#ifdef _X360
#undef WaitForSingleObject
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ASSERT_INVARIANT( SEEK_CUR == FILESYSTEM_SEEK_CURRENT );
ASSERT_INVARIANT( SEEK_SET == FILESYSTEM_SEEK_HEAD );
ASSERT_INVARIANT( SEEK_END == FILESYSTEM_SEEK_TAIL );

//-----------------------------------------------------------------------------

class CFileSystem_Stdio : public CBaseFileSystem
{
public:
	CFileSystem_Stdio();
	~CFileSystem_Stdio();

	// Used to get at older versions
	void *QueryInterface( const char *pInterfaceName );

	// Higher level filesystem methods requiring specific behavior
	virtual void GetLocalCopy( const char *pFileName );
	virtual int	HintResourceNeed( const char *hintlist, int forgetEverything );
	virtual bool IsFileImmediatelyAvailable(const char *pFileName);
	virtual WaitForResourcesHandle_t WaitForResources( const char *resourcelist );
	virtual bool GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */ , bool *complete /* out */ );
	virtual void CancelWaitForResources( WaitForResourcesHandle_t handle );
	virtual bool IsSteam() const { return false; }
	virtual	FilesystemMountRetval_t MountSteamContent( int nExtraAppId = -1 ) { return FILESYSTEM_MOUNT_OK; }

	bool GetOptimalIOConstraints( FileHandle_t hFile, unsigned *pOffsetAlign, unsigned *pSizeAlign, unsigned *pBufferAlign );
	void *AllocOptimalReadBuffer( FileHandle_t hFile, unsigned nSize, unsigned nOffset );
	void FreeOptimalReadBuffer( void *p );

#ifdef BUILD_GMOD
	virtual void RemoveSearchPathsByGroup( int group );
	virtual void SetGet( IGet *get );
	virtual Addon::FileSystem *Addons( );
	virtual Gamemode::System *Gamemodes( );
	virtual GameDepot::System *Games( );
	virtual LegacyAddons::System *LegacyAddons( );
	virtual CLanguage *Language( );
	virtual void DoFilesystemRefresh( );
	virtual int LastFilesystemRefresh( );
	virtual void AddVPKFileFromPath( const char *a, const char *b, unsigned int c );
	virtual void GMOD_SetupDefaultPaths( const char *a, const char *b );
	virtual void GMOD_FixPathCase( char *a, size_t b );
#endif

protected:
	// implementation of CBaseFileSystem virtual functions
	virtual FILE *FS_fopen( const char *filename, const char *options, unsigned flags, int64 *size );
	virtual void FS_setbufsize( FILE *fp, unsigned nBytes );
	virtual void FS_fclose( FILE *fp );
	virtual void FS_fseek( FILE *fp, int64 pos, int seekType );
	virtual long FS_ftell( FILE *fp );
	virtual int FS_feof( FILE *fp );
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size, FILE *fp );
	virtual size_t FS_fwrite( const void *src, size_t size, FILE *fp );
	virtual bool FS_setmode( FILE *fp, FileMode_t mode );
	virtual size_t FS_vfprintf( FILE *fp, const char *fmt, va_list list );
	virtual int FS_ferror( FILE *fp );
	virtual int FS_fflush( FILE *fp );
	virtual char *FS_fgets( char *dest, int destSize, FILE *fp );
	virtual int FS_stat( const char *path, struct _stat *buf, bool *pbLoadedFromSteamCache=NULL  );
	virtual int FS_chmod( const char *path, int pmode );
	virtual HANDLE FS_FindFirstFile(const char *findname, WIN32_FIND_DATA *dat);
	virtual bool FS_FindNextFile(HANDLE handle, WIN32_FIND_DATA *dat);
	virtual bool FS_FindClose(HANDLE handle);
	virtual int FS_GetSectorSize( FILE * );

private:
	bool CanAsync() const
	{
		return m_bCanAsync;
	}

	bool m_bMounted;
	bool m_bCanAsync;
};


//-----------------------------------------------------------------------------
// Per-file worker classes
//-----------------------------------------------------------------------------
abstract_class CStdFilesystemFile
{
public:
	virtual ~CStdFilesystemFile() {}
	virtual void FS_setbufsize( unsigned nBytes ) = 0;
	virtual void FS_fclose() = 0;
	virtual void FS_fseek( int64 pos, int seekType ) = 0;
	virtual long FS_ftell() = 0;
	virtual int FS_feof() = 0;
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size ) = 0;
	virtual size_t FS_fwrite( const void *src, size_t size ) = 0;
	virtual bool FS_setmode( FileMode_t mode ) = 0;
	virtual size_t FS_vfprintf( const char *fmt, va_list list ) = 0;
	virtual int FS_ferror() = 0;
	virtual int FS_fflush() = 0;
	virtual char *FS_fgets( char *dest, int destSize ) = 0;
	virtual int FS_GetSectorSize() { return 1; }
};

//---------------------------------------------------------

class CStdioFile : public CStdFilesystemFile
{
public:
	static CStdioFile *FS_fopen( const char *filename, const char *options, int64 *size );

	virtual void FS_setbufsize( unsigned nBytes );
	virtual void FS_fclose();
	virtual void FS_fseek( int64 pos, int seekType );
	virtual long FS_ftell();
	virtual int FS_feof();
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size);
	virtual size_t FS_fwrite( const void *src, size_t size );
	virtual bool FS_setmode( FileMode_t mode );
	virtual size_t FS_vfprintf( const char *fmt, va_list list );
	virtual int FS_ferror();
	virtual int FS_fflush();
	virtual char *FS_fgets( char *dest, int destSize );

#ifdef POSIX
	static CUtlMap< ino_t, CThreadMutex * > m_LockedFDMap;
	static CThreadMutex	m_MutexLockedFD;
#endif
private:
	CStdioFile( FILE *pFile, bool bWriteable )
		: m_pFile( pFile )
#ifdef POSIX
		, m_bWriteable( bWriteable )
#endif
	{
	}

	FILE *m_pFile;

#ifdef POSIX
	bool m_bWriteable;
#endif
};

#ifdef POSIX
CUtlMap< ino_t, CThreadMutex * > CStdioFile::m_LockedFDMap;
CThreadMutex	CStdioFile::m_MutexLockedFD;
#endif

//-----------------------------------------------------------------------------

#ifdef _WIN32
class CWin32ReadOnlyFile : public CStdFilesystemFile
{
public:
	static bool CanOpen( const char *filename, const char *options );
	static CWin32ReadOnlyFile *FS_fopen( const char *filename, const char *options, int64 *size );

	virtual void FS_setbufsize( unsigned nBytes ) {}
	virtual void FS_fclose();
	virtual void FS_fseek( int64 pos, int seekType );
	virtual long FS_ftell();
	virtual int FS_feof();
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size);
	virtual size_t FS_fwrite( const void *src, size_t size ) { return 0; }
	virtual bool FS_setmode( FileMode_t mode ) { Error( "Can't set mode, open a second file in right mode\n" ); return false; }
	virtual size_t FS_vfprintf( const char *fmt, va_list list ) { return 0; }
	virtual int FS_ferror() { return 0;	}
	virtual int FS_fflush() { return 0; }
	virtual char *FS_fgets( char *dest, int destSize );
	virtual int FS_GetSectorSize() { return m_SectorSize; }

private:
	CWin32ReadOnlyFile( HANDLE hFileUnbuffered, HANDLE hFileBuffered, int sectorSize, int64 fileSize, bool bOverlapped )
	 :	m_ReadPos( 0 ),
		m_Size( fileSize ),
		m_hFileUnbuffered( hFileUnbuffered ),
		m_hFileBuffered( hFileBuffered ),
		m_SectorSize( sectorSize ),
		m_bOverlapped( bOverlapped )
	{
	}

	int64				m_ReadPos;
	int64				m_Size;
	HANDLE				m_hFileUnbuffered;
	HANDLE				m_hFileBuffered;
	CThreadFastMutex	m_Mutex;
	int					m_SectorSize;
	bool				m_bOverlapped;
};

#endif

//-----------------------------------------------------------------------------
// singleton
//-----------------------------------------------------------------------------
CFileSystem_Stdio g_FileSystem_Stdio;
#if defined(_WIN32) && defined(DEDICATED)
CBaseFileSystem *BaseFileSystem_Stdio( void )
{
	return &g_FileSystem_Stdio;
}
#endif
 
#ifdef DEDICATED // "hack" to allow us to not export a stdio version of the FILESYSTEM_INTERFACE_VERSION anywhere

IFileSystem *g_pFileSystem = &g_FileSystem_Stdio;
IBaseFileSystem *g_pBaseFileSystem = &g_FileSystem_Stdio;

#else

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CFileSystem_Stdio, IFileSystem, FILESYSTEM_INTERFACE_VERSION, g_FileSystem_Stdio );
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CFileSystem_Stdio, IBaseFileSystem, BASEFILESYSTEM_INTERFACE_VERSION, g_FileSystem_Stdio );

#endif

//-----------------------------------------------------------------------------

#ifndef _RETAIL
bool UseOptimalBufferAllocation()
{
	static bool bUseOptimalBufferAllocation = ( !IsLinux() && Q_stristr( Plat_GetCommandLine(), "-unbuffered_io" ) != NULL );
	return bUseOptimalBufferAllocation;
}
ConVar filesystem_unbuffered_io( "filesystem_unbuffered_io", "1", 0, "" );
#define UseUnbufferedIO() ( UseOptimalBufferAllocation() && filesystem_unbuffered_io.GetBool() )
#else
#define UseUnbufferedIO() true
#endif

ConVar filesystem_native( "filesystem_native", "1", 0, "Use native FS or STDIO" );
ConVar filesystem_max_stdio_read( "filesystem_max_stdio_read", IsX360() ? "64" : "16", 0, "" );
ConVar filesystem_report_buffered_io( "filesystem_report_buffered_io", "0" );

//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
CFileSystem_Stdio::CFileSystem_Stdio()
{
	m_bMounted = false;
	m_bCanAsync = true;
#ifdef POSIX
	SetDefLessFunc( CStdioFile::m_LockedFDMap );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFileSystem_Stdio::~CFileSystem_Stdio()
{
#ifdef POSIX
	FOR_EACH_MAP_FAST( CStdioFile::m_LockedFDMap, i )
	{
		Assert( CStdioFile::m_LockedFDMap[ i ] );
		delete CStdioFile::m_LockedFDMap[ i ];
	}
	CStdioFile::m_LockedFDMap.RemoveAll();
#endif

	Assert(!m_bMounted);
}


//-----------------------------------------------------------------------------
// QueryInterface: 
//-----------------------------------------------------------------------------
void *CFileSystem_Stdio::QueryInterface( const char *pInterfaceName )
{
	// We also implement the IMatSystemSurface interface
	if (!Q_strncmp(	pInterfaceName, FILESYSTEM_INTERFACE_VERSION, ssize(FILESYSTEM_INTERFACE_VERSION) ))
		return (IFileSystem*)this;

	return CBaseFileSystem::QueryInterface( pInterfaceName );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::GetOptimalIOConstraints( FileHandle_t hFile, unsigned *pOffsetAlign, unsigned *pSizeAlign, unsigned *pBufferAlign )
{
	unsigned sectorSize;
	
	if ( hFile && UseOptimalBufferAllocation() )
	{
		CFileHandle *fh = ( CFileHandle *)hFile;
		sectorSize = fh->GetSectorSize();

		if ( !sectorSize || ( fh->m_pPackFileHandle && ( fh->m_pPackFileHandle->AbsoluteBaseOffset() % sectorSize ) ) )
		{
			sectorSize = 1;
		}
	}
	else
	{
		sectorSize = 1;
	}

	if ( pOffsetAlign )
	{
		*pOffsetAlign = sectorSize;
	}

	if ( pSizeAlign )
	{
		*pSizeAlign = sectorSize;
	}

	if ( pBufferAlign )
	{
		if ( IsX360() )
		{
			*pBufferAlign = 4;
		}
		else
		{
			*pBufferAlign = sectorSize;
		}
	}

	return ( sectorSize > 1 );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void *CFileSystem_Stdio::AllocOptimalReadBuffer( FileHandle_t hFile, unsigned nSize, unsigned nOffset )
{
	if ( !UseOptimalBufferAllocation() )
	{
		return CBaseFileSystem::AllocOptimalReadBuffer( hFile, nSize, nOffset );
	}

	unsigned sectorSize;
	if ( hFile != FILESYSTEM_INVALID_HANDLE )
	{
		CFileHandle *fh = ( CFileHandle *)hFile;
		sectorSize = fh->GetSectorSize();

		if ( !nSize )
		{
			nSize = fh->Size();
		}

		if ( fh->m_pPackFileHandle )
		{
			nOffset += fh->m_pPackFileHandle->AbsoluteBaseOffset();
		}
	}
	else
	{
		// an invalid handle gets a fake "optimal" but valid buffer
		// this path is for a caller that isn't doing i/o, 
		// but needs an "optimal" buffer that can end up passed to FreeOptimalReadBuffer()
		sectorSize = 4;
	}

	bool bOffsetIsAligned = ( nOffset % sectorSize == 0 );
	unsigned nAllocSize = ( bOffsetIsAligned ) ? AlignValue( nSize, sectorSize ) : nSize;

	if ( IsX360() )
	{
		return malloc( nAllocSize );
	}
	else
	{
		unsigned nAllocAlignment = ( bOffsetIsAligned ) ? sectorSize : 4;
		return _aligned_malloc( nAllocSize, nAllocAlignment );
	}
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FreeOptimalReadBuffer( void *p )
{
	if ( !UseOptimalBufferAllocation() )
	{
		CBaseFileSystem::FreeOptimalReadBuffer( p );
		return;
	}

	if ( p )
	{
		if ( IsX360() )
		{
			free( p );
		}
		else
		{
			 _aligned_free( p );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
FILE *CFileSystem_Stdio::FS_fopen( const char *filenameT, const char *options, unsigned flags, int64 *size )
{
	VPROF_BUDGET( "CFileSystem_Stdio::FS_fopen", VPROF_BUDGETGROUP_OTHER_FILESYSTEM );
	CStdFilesystemFile *pFile = NULL;

	char filename[ MAX_PATH ];

	CBaseFileSystem::FixUpPath ( filenameT, filename, sizeof( filename ) );

#ifdef _WIN32
	if ( CWin32ReadOnlyFile::CanOpen( filename, options ) )
	{
		pFile = CWin32ReadOnlyFile::FS_fopen( filename, options, size );
		if ( pFile )
		{
			return (FILE *)pFile;
		}
		// If you have filesystem_native 1 checking if a file exists can take twice as long. There are some cases where CWin32[...]::FS_fopen fails but CStdioFile::FS_fopen works
	}
#endif

	pFile = CStdioFile::FS_fopen( filename, options, size );

	return (FILE *)pFile;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FS_setbufsize( FILE *fp, unsigned nBytes )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	pFile->FS_setbufsize( nBytes );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FS_fclose( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);

	pFile->FS_fclose();
	delete pFile;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::FS_fseek( FILE *fp, int64 pos, int seekType )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);

	pFile->FS_fseek( pos, seekType );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
long CFileSystem_Stdio::FS_ftell( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_ftell();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_feof( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_feof();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CFileSystem_Stdio::FS_fread( void *dest, size_t destSize, size_t size, FILE *fp )
{
	tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s", __FUNCTION__ );
	if( ThreadInMainThread() )
	{
		tmPlotI32( TELEMETRY_LEVEL0, TMPT_MEMORY, 0, size, "FileBytesRead" );
	}

	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	size_t nBytesRead = pFile->FS_fread( dest, destSize, size);

	Trace_FRead( nBytesRead, fp );

	return nBytesRead;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CFileSystem_Stdio::FS_fwrite( const void *src, size_t size, FILE *fp )
{
	tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s %t", __FUNCTION__, tmSendCallStack( TELEMETRY_LEVEL0, 0 ) );
	if( ThreadInMainThread() )
	{
		tmPlotI32( TELEMETRY_LEVEL0, TMPT_MEMORY, 0, size, "FileBytesWrite" );
	}

	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);

	size_t nBytesWritten = pFile->FS_fwrite(src, size);

	return nBytesWritten;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::FS_setmode( FILE *fp, FileMode_t mode )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_setmode( mode );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CFileSystem_Stdio::FS_vfprintf( FILE *fp, const char *fmt, va_list list )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_vfprintf(fmt, list);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_ferror( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_ferror();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_fflush( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_fflush();
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
char *CFileSystem_Stdio::FS_fgets( char *dest, int destSize, FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_fgets(dest, destSize);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *path - 
//			pmode - 
// Output : int
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_chmod( const char *pathT, int pmode )
{
	if ( !pathT )
		return -1;

	char path[ MAX_PATH ];

	CBaseFileSystem::FixUpPath ( pathT, path, sizeof( path ) );

	int rt = _chmod( path, pmode );
#if defined(LINUX)
	if (rt==-1)
	{
		char caseFixedName[ MAX_PATH ];
		const bool found = findFileInDirCaseInsensitive_safe( path, caseFixedName );
		if ( found )
		{
			rt=_chmod( caseFixedName, pmode );
		}
	}	
#endif
	return rt;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_stat( const char *pathT, struct _stat *buf, bool *pbLoadedFromSteamCache )
{
	if ( pbLoadedFromSteamCache )
		*pbLoadedFromSteamCache = false;
		
	if ( !pathT )
	{
		return -1;
	}

	char path[ MAX_PATH ];

	CBaseFileSystem::FixUpPath ( pathT, path, sizeof( path ) );

	int rt = _stat( path, buf );

#if defined(LINUX)
	if ( rt == -1 )
	{
		char caseFixedName[ MAX_PATH ];
		bool found = findFileInDirCaseInsensitive_safe( path, caseFixedName );
		if ( found )
		{
			rt = _stat( caseFixedName, buf );
		}
	}	
#endif
	return rt;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
HANDLE CFileSystem_Stdio::FS_FindFirstFile(const char *findnameT, WIN32_FIND_DATA *dat)
{
	char findname[ MAX_PATH ];

	CBaseFileSystem::FixUpPath ( findnameT, findname, sizeof( findname ) );

	return ::FindFirstFile(findname, dat);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::FS_FindNextFile(HANDLE handle, WIN32_FIND_DATA *dat)
{

	if (INVALID_HANDLE_VALUE == handle)  // invalid handle should return false
		return false;

	return (::FindNextFile(handle, dat) != 0);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::FS_FindClose(HANDLE handle)
{
	return (::FindClose(handle) != 0);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::FS_GetSectorSize( FILE *fp )
{
	CStdFilesystemFile *pFile = ((CStdFilesystemFile *)fp);
	return pFile->FS_GetSectorSize();
}

//-----------------------------------------------------------------------------
// Purpose: files are always immediately available on disk
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::IsFileImmediatelyAvailable(const char *pFileName)
{
	return true;
}

// enable this if you want the stdio filesystem to pretend it's steam, and make people wait for resources
//#define DEBUG_WAIT_FOR_RESOURCES_API

#if defined(DEBUG_WAIT_FOR_RESOURCES_API)
static float g_flDebugProgress = 0.0f;
#endif

//-----------------------------------------------------------------------------
// Purpose: steam call, unnecessary in stdio
//-----------------------------------------------------------------------------
WaitForResourcesHandle_t CFileSystem_Stdio::WaitForResources( const char *resourcelist )
{
#if defined(DEBUG_WAIT_FOR_RESOURCES_API)
	g_flDebugProgress = 0.0f;
#endif

	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: steam call, unnecessary in stdio
//-----------------------------------------------------------------------------
bool CFileSystem_Stdio::GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */ , bool *complete /* out */ )
{
#if defined(DEBUG_WAIT_FOR_RESOURCES_API)
	g_flDebugProgress += 0.002f;
	if (g_flDebugProgress < 1.0f)
	{
		*progress = g_flDebugProgress;
		*complete = false;
		return true;
	}
#endif

	// always return that we're complete
	*progress = 0.0f;
	*complete = true;
	return true;
}

//-----------------------------------------------------------------------------
// Purpose: steam call, unnecessary in stdio
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::CancelWaitForResources( WaitForResourcesHandle_t handle )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFileSystem_Stdio::GetLocalCopy( const char *pFileName )
{
	// do nothing. . everything is local.
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CFileSystem_Stdio::HintResourceNeed( const char *hintlist, int forgetEverything )
{
	// do nothing. . everything is local.
	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
CStdioFile *CStdioFile::FS_fopen( const char *filenameT, const char *options, int64 *size )
{
	VPROF_BUDGET( "CBaseFileSystem::FS_fopen", VPROF_BUDGETGROUP_OTHER_FILESYSTEM );
	FILE *pFile = NULL;
	char *p = NULL;
	char filename[MAX_PATH];
	struct _stat buf;

	V_strncpy( filename, filenameT, sizeof(filename) );
	
	// stop newline characters at end of filename
	p = strchr( filename, '\n' );
	if ( p )
		*p = '\0';
	p = strchr( filename, '\r' );
	if ( p )
		*p = '\0';


	pFile = fopen(filename, options);
	if (pFile && size)
	{
		// todo: replace with filelength()? 
		int rt = _stat( filename, &buf );
		if (rt == 0)
		{
			*size = buf.st_size;
		}
	}

#if defined(LINUX)
	if(!pFile && !strchr(options,'w') && !strchr(options,'+') ) // try opening the lower cased version
	{
		char caseFixedName[ MAX_PATH ];
		bool found = findFileInDirCaseInsensitive_safe( filename, caseFixedName );
		if ( found )
		{	
			pFile = fopen( caseFixedName, options );

			if (pFile && size)
			{
				// todo: replace with filelength()? 
				struct _stat buf;
				int rt = _stat( caseFixedName, &buf );
				if (rt == 0)
				{
					*size = buf.st_size;
				}
			}
		}
	}
#endif

	if ( pFile )
	{
		bool bWriteable = false;
		if ( strchr(options,'w') || strchr(options,'a') )
			bWriteable = true;
		
#if defined POSIX
		if ( bWriteable )
		{
			CThreadMutex *pMutex = NULL;

			{
				AUTO_LOCK( m_MutexLockedFD );
				// Win32 has an undocumented feature that is serialized ALL writes to a file across threads (i.e only 1 thread can open a file at a time)
				// so add a lock here to mimic that behavior

				int iLockID = m_LockedFDMap.Find( buf.st_ino );
				if ( iLockID != m_LockedFDMap.InvalidIndex() )
				{
					pMutex = m_LockedFDMap[iLockID];
				}
				else
				{
					CThreadMutex *newMutex = new CThreadMutex;
					pMutex = m_LockedFDMap[m_LockedFDMap.Insert( buf.st_ino, newMutex )];
				}
			}
			// grab the lock once we have UNLOCKED m_MutexLockedFD so we don't deadlock on a close
			pMutex->Lock();

			rewind( pFile );

			// we need to get the file size again after the lock returns
			if (pFile && size)
			{
				int rt = _stat( filename, &buf );
				if (rt == 0)
				{
					*size = buf.st_size;
				}
			}

		}
#endif
		return new CStdioFile( pFile, bWriteable );
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CStdioFile::FS_setbufsize( unsigned nBytes )
{
#ifdef _WIN32
	if ( nBytes )
	{
		setvbuf( m_pFile, NULL, _IOFBF,  32768 );
	}
	else
	{
		setvbuf( m_pFile, NULL, _IONBF,  0 );
#if defined(_MSC_VER) && ( _MSC_VER < 1900 )
		// hack to make microsoft stdio not always read one stray byte on odd sized files
		m_pFile->_bufsiz = 1;
#endif
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CStdioFile::FS_fclose()
{
#ifdef POSIX
	if ( m_bWriteable )
	{
		AUTO_LOCK( m_MutexLockedFD );

		struct _stat buf;
		int fd = fileno_unlocked( m_pFile );
		fstat( fd, &buf );

		fflush( m_pFile );
		int iLockID = m_LockedFDMap.Find( buf.st_ino );
		if ( iLockID != m_LockedFDMap.InvalidIndex() )
		{
			m_LockedFDMap[iLockID]->Unlock();
		}
	}
#endif
	fclose(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CStdioFile::FS_fseek( int64 pos, int seekType )
{
	fseek( m_pFile, pos, seekType );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
long CStdioFile::FS_ftell()
{
	return ftell(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CStdioFile::FS_feof()
{
	return feof(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CStdioFile::FS_fread( void *dest, size_t destSize, size_t size )
{
	tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s %t", __FUNCTION__, tmSendCallStack( TELEMETRY_LEVEL0, 0 ) );
	if( ThreadInMainThread() )
	{
		tmPlotI32( TELEMETRY_LEVEL0, TMPT_MEMORY, 0, size, "FileBytesRead" );
	}

	// read (size) of bytes to ensure truncated reads returns bytes read and not 0
	return fread( dest, 1, size, m_pFile );
}


#define WRITE_CHUNK		(256 * 1024)

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//
// This routine breaks data into chunks if the amount to be written is beyond WRITE_CHUNK (256kb)
// Windows can fail on monolithic writes of ~12MB or more, so we work around that here
//-----------------------------------------------------------------------------
size_t CStdioFile::FS_fwrite( const void *src, size_t size )
{
	tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s %t", __FUNCTION__, tmSendCallStack( TELEMETRY_LEVEL0, 0 ) );
	if( ThreadInMainThread() )
	{
		tmPlotI32( TELEMETRY_LEVEL0, TMPT_MEMORY, 0, size, "FileBytesWrite" );
	}

	if ( size > WRITE_CHUNK )
	{
		size_t remaining = size;
		const byte* current = (const byte *) src;
		size_t total = 0;

		while ( remaining > 0 )
		{
			size_t bytesToCopy = min(remaining, (size_t)WRITE_CHUNK);

			total += fwrite(current, 1, bytesToCopy, m_pFile);

			remaining -= bytesToCopy;
			current += bytesToCopy;
		}

		Assert( total == size );
		return total;
	}

	return fwrite(src, 1, size, m_pFile);// return number of bytes written (because we have size = 1, count = bytes, so it return bytes)
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
bool CStdioFile::FS_setmode( FileMode_t mode )
{
#ifdef _WIN32
	int fd = _fileno( m_pFile );
	int newMode = ( mode == FM_BINARY ) ? _O_BINARY : _O_TEXT;
	return ( _setmode( fd, newMode) != -1 );
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CStdioFile::FS_vfprintf( const char *fmt, va_list list )
{
	return vfprintf(m_pFile, fmt, list);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CStdioFile::FS_ferror()
{
	return ferror(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CStdioFile::FS_fflush()
{
	return fflush(m_pFile);
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
char *CStdioFile::FS_fgets( char *dest, int destSize )
{
	return fgets(dest, destSize, m_pFile);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
#ifdef _WIN32

ConVar filesystem_use_overlapped_io( "filesystem_use_overlapped_io", "1", 0, "" );
#define UseOverlappedIO() filesystem_use_overlapped_io.GetBool()

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int GetSectorSize( const char *pszFilename )
{
	if ( ( !pszFilename[0] || !pszFilename[1] ) ||
		 ( pszFilename[0] == '\\' && pszFilename[1] == '\\' ) ||
		 ( pszFilename[0] == '/' && pszFilename[1] == '/' ) )
	{
		// Cannot determine sector size with a UNC path (need volume identifier)
		return 0;
	}

#if defined( _WIN32 ) && !defined( FILESYSTEM_STEAM )
	char szAbsoluteFilename[MAX_FILEPATH];
	if ( pszFilename[1] != ':' )
	{
		Q_MakeAbsolutePath( szAbsoluteFilename, sizeof(szAbsoluteFilename), pszFilename );
		pszFilename = szAbsoluteFilename;
	}

	DWORD sectorSize = 1;

	struct DriveSectorSize_t
	{
		char volume;
		DWORD sectorSize;
	};

	static DriveSectorSize_t cachedSizes[8];

	char volume = tolower( *pszFilename );

	size_t i;
	for ( i = 0; i < std::size(cachedSizes) && cachedSizes[i].volume; i++ )
	{
		if ( cachedSizes[i].volume == volume )
		{
			sectorSize = cachedSizes[i].sectorSize;
			break;
		}
	}

	if ( sectorSize == 1 )
	{
		char root[4] = "X:\\";
		root[0] = *pszFilename;

		DWORD ignored;
		if ( !GetDiskFreeSpace( root, &ignored, &sectorSize, &ignored, &ignored ) )
		{
			sectorSize = 0;
		}

		if ( i < std::size(cachedSizes) )
		{
			cachedSizes[i].volume = volume;
			cachedSizes[i].sectorSize = sectorSize;
		}
	}

	return sectorSize;
#else
	return 0;
#endif
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------

class CThreadIOEventPool
{
public:
	~CThreadIOEventPool()
	{
		CThreadEvent *pEvent;

		while ( m_Events.PopItem( &pEvent ) )
		{
			delete pEvent;
		}
	}

	CThreadEvent *GetEvent()
	{
		CThreadEvent *pEvent;

		if ( m_Events.PopItem( &pEvent ) )
		{
			return pEvent;
		}

		return new CThreadEvent;
	}

	void ReleaseEvent( CThreadEvent *pEvent )
	{
		m_Events.PushItem( pEvent );
	}

private:
	CTSList<CThreadEvent *> m_Events;
};


CThreadIOEventPool g_ThreadIOEvents;


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CWin32ReadOnlyFile::CanOpen( const char *, const char *options )
{
	return ( options[0] == 'r' && options[1] == 'b' && options[2] == 0 && filesystem_native.GetBool() );
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
static HANDLE OpenWin32File( const char *filename, bool bOverlapped, bool bUnbuffered, int64 *pFileSize )
{
	VPROF_BUDGET( "OpenWin32File", VPROF_BUDGETGROUP_OTHER_FILESYSTEM );
	HANDLE hFile;

	DWORD createFlags = FILE_ATTRIBUTE_NORMAL;
		
	if ( bOverlapped )
	{
		createFlags |= FILE_FLAG_OVERLAPPED;
	}

	if ( bUnbuffered )
	{
		createFlags |= FILE_FLAG_NO_BUFFERING;
	}

	hFile = ::CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, createFlags, NULL );
	if ( hFile != INVALID_HANDLE_VALUE && !*pFileSize )
	{
		LARGE_INTEGER fileSize;
		if ( !GetFileSizeEx( hFile, &fileSize ) )
		{
			CloseHandle( hFile );
			hFile = INVALID_HANDLE_VALUE;
		}
		*pFileSize = fileSize.QuadPart;
	}
	return hFile;
}

CWin32ReadOnlyFile *CWin32ReadOnlyFile::FS_fopen( const char *filename, const char *options, int64 *size )
{
	VPROF_BUDGET( "CWin32ReadOnlyFile::FS_fopen", VPROF_BUDGETGROUP_OTHER_FILESYSTEM );
	Assert( CanOpen( filename, options ) );

	int sectorSize = 0;
	bool bTryUnbuffered = ( UseUnbufferedIO() && ( sectorSize = GetSectorSize( filename ) ) != 0 );
	bool bOverlapped = UseOverlappedIO();

	HANDLE hFileUnbuffered = INVALID_HANDLE_VALUE;
	int64 fileSize = 0;

	if ( bTryUnbuffered )
	{
		hFileUnbuffered = OpenWin32File( filename, bOverlapped, true, &fileSize );
		if ( hFileUnbuffered == INVALID_HANDLE_VALUE )
		{
			return NULL;
		}
	}

	HANDLE hFileBuffered = OpenWin32File( filename, bOverlapped, false, &fileSize );
	if ( hFileBuffered == INVALID_HANDLE_VALUE )
	{
		if ( hFileUnbuffered != INVALID_HANDLE_VALUE )
		{
			CloseHandle( hFileUnbuffered );
		}
		return NULL;
	}

	if ( size )
	{
		*size = fileSize;
	}

	return new CWin32ReadOnlyFile( hFileUnbuffered, hFileBuffered, ( sectorSize ) ? sectorSize : 1, fileSize, bOverlapped );
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CWin32ReadOnlyFile::FS_fclose()
{
	if ( m_hFileUnbuffered != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hFileUnbuffered );
	}

	if ( m_hFileBuffered != INVALID_HANDLE_VALUE )
	{
		CloseHandle( m_hFileBuffered );
	}
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
void CWin32ReadOnlyFile::FS_fseek( int64 pos, int seekType )
{
	switch ( seekType )
	{
	case SEEK_SET:
		m_ReadPos = pos;
		break;

	case SEEK_CUR:
		m_ReadPos += pos;
		break;

	case SEEK_END:
		m_ReadPos = m_Size - pos;
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
long CWin32ReadOnlyFile::FS_ftell()
{
	return m_ReadPos;	
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
int CWin32ReadOnlyFile::FS_feof()
{
	return ( m_ReadPos >= m_Size );	
}

// ends up on a thread's stack, don't blindly increase without awareness of that implication
// 360 threads have small stacks, using small buffer of the worst case quantum sector size
#if !defined( _X360 )
#define READ_TEMP_BUFFER	( 32*1024 )
#else
#define READ_TEMP_BUFFER	( 2*XBOX_DVD_SECTORSIZE )
#endif

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
size_t CWin32ReadOnlyFile::FS_fread( void *dest, size_t destSize, size_t size )
{
	VPROF_BUDGET( "CWin32ReadOnlyFile::FS_fread", VPROF_BUDGETGROUP_OTHER_FILESYSTEM );
	tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s %t", __FUNCTION__, tmSendCallStack( TELEMETRY_LEVEL0, 0 ) );
	if( ThreadInMainThread() )
	{
		tmPlotI32( TELEMETRY_LEVEL0, TMPT_MEMORY, 0, size, "FileBytesRead" );
	}

	if ( !size || ( m_hFileUnbuffered == INVALID_HANDLE_VALUE && m_hFileBuffered == INVALID_HANDLE_VALUE ) )
	{
		return 0;
	}

	CThreadEvent *pEvent = NULL;

	if ( destSize == (size_t)-1 )
	{
		destSize = size;
	}

	byte tempBuffer[READ_TEMP_BUFFER];
	HANDLE hReadFile = m_hFileBuffered;
	int nBytesToRead = size;
	byte *pDest = (byte *)dest;
	int64 offset = m_ReadPos;

	if ( m_hFileUnbuffered != INVALID_HANDLE_VALUE )
	{
		const int destBaseAlign = ( IsX360() ) ? 4 : m_SectorSize;
		bool bDestBaseIsAligned = ( (uintp)dest % destBaseAlign == 0 );
		bool bCanReadUnbufferedDirect = ( bDestBaseIsAligned && ( destSize % m_SectorSize == 0 ) && ( m_ReadPos % m_SectorSize == 0 ) );

		if ( bCanReadUnbufferedDirect )
		{
			// fastest path, unbuffered
			nBytesToRead = AlignValue( size, m_SectorSize );
			hReadFile = m_hFileUnbuffered;
		}
		else
		{
			// not properly aligned, snap to alignments
			// attempt to perform single unbuffered operation using stack buffer
			int64 alignedOffset = AlignValue( ( m_ReadPos - m_SectorSize ) + 1, m_SectorSize );
			unsigned int alignedBytesToRead = AlignValue( ( m_ReadPos - alignedOffset ) + size, m_SectorSize );
			if ( alignedBytesToRead <= sizeof( tempBuffer ) - destBaseAlign )
			{
				// read operation can be performed as unbuffered follwed by a post fixup
				nBytesToRead = alignedBytesToRead;
				offset = alignedOffset;
				pDest = AlignValue( tempBuffer, destBaseAlign );
				hReadFile = m_hFileUnbuffered;
			}
		}
	}

	OVERLAPPED overlapped = {};
	if ( m_bOverlapped )
	{
		pEvent = g_ThreadIOEvents.GetEvent();
		overlapped.hEvent = *pEvent;
	}

#ifdef REPORT_BUFFERED_IO
	if ( hReadFile == m_hFileBuffered && filesystem_report_buffered_io.GetBool() )
	{
		Msg( "Buffered Operation :(\n" );
	}
#endif

	// some disk drivers will fail if read is too large
	static int MAX_READ = filesystem_max_stdio_read.GetInt()*1024*1024;
	const int MIN_READ = 64*1024;
	bool bReadOk = true;
	DWORD nBytesRead = 0;
	size_t result = 0;
	int64 currentOffset = offset;

	while ( bReadOk && nBytesToRead > 0 )
	{
		int nCurBytesToRead = min( nBytesToRead, MAX_READ );
		DWORD nCurBytesRead = 0;

		overlapped.Offset = currentOffset & 0xFFFFFFFF;
		overlapped.OffsetHigh = ( currentOffset >> 32 ) & 0xFFFFFFFF;

		bReadOk = ( ::ReadFile( hReadFile, pDest + nBytesRead, nCurBytesToRead, &nCurBytesRead, &overlapped ) != 0 );
		if ( !bReadOk )
		{
			if ( m_bOverlapped && GetLastError() == ERROR_IO_PENDING )
			{
				bReadOk = true;
			}
		}

		if ( bReadOk )
		{
			if ( !m_bOverlapped || GetOverlappedResult( hReadFile, &overlapped, &nCurBytesRead, true ) )
			{
				nBytesRead += nCurBytesRead;
				nBytesToRead -= nCurBytesRead;
				currentOffset += nCurBytesRead;
			}
			else
			{
				if ( m_bOverlapped )
				{
					if ( GetLastError() == ERROR_HANDLE_EOF )
					{
						nBytesToRead = 0; // we have hit the end of the file					
					}
					else
					{
						bReadOk = false;
					}
				}
				else
				{
					bReadOk = false;
				}
			}

			if ( !m_bOverlapped && nCurBytesRead == 0 )
			{
				nBytesToRead = 0; // we have hit the end of the file
			}

		}

		if ( nBytesToRead > 0 && nCurBytesRead == 0 ) // if you failed to ready anything this time then bail the loop
		{
			DevMsg( "Got zero length read" );
			bReadOk = false;
		}
		else if ( !bReadOk  )
		{
			DWORD dwError = GetLastError();

			if ( IsX360() )
			{
				if ( dwError == ERROR_DISK_CORRUPT || dwError == ERROR_FILE_CORRUPT )
				{
					FSDirtyDiskReportFunc_t func = g_FileSystem_Stdio.GetDirtyDiskReportFunc();
					if ( func )
					{
						func();
						result = 0;
					}
				}
			}

			if ( dwError == ERROR_NO_SYSTEM_RESOURCES && MAX_READ > MIN_READ )
			{
				MAX_READ /= 2;
				bReadOk = true;
				DevMsg( "ERROR_NO_SYSTEM_RESOURCES: Reducing max read to %d bytes\n", MAX_READ );
			}
			else
			{
				DevMsg( "Unknown read error %d\n", dwError );
			}
		}
	}

	if ( bReadOk )
	{
		if ( nBytesRead && hReadFile == m_hFileUnbuffered && pDest != dest )
		{
			int nBytesExtra = ( m_ReadPos - offset );
			nBytesRead -= nBytesExtra;
			if ( nBytesRead )
			{
				memcpy( dest, (byte *)pDest + nBytesExtra, size );
			}
		}

		result = min( (size_t)nBytesRead, size );
	}

	if ( m_bOverlapped )
	{
		pEvent->Reset();
		g_ThreadIOEvents.ReleaseEvent( pEvent );
	}

	m_ReadPos += result;

	return result;
}

//-----------------------------------------------------------------------------
// Purpose: low-level filesystem wrapper
//-----------------------------------------------------------------------------
char *CWin32ReadOnlyFile::FS_fgets( char *dest, int destSize ) 
{  
	if ( FS_feof() )
	{
		return NULL;
	}
	int64 nStartPos = m_ReadPos;
	int nBytesRead = FS_fread( dest, destSize, destSize );
	if ( !nBytesRead )
	{
		return NULL;
	}

	dest[min( nBytesRead, destSize - 1)] = 0;
	char *pNewline = strchr( dest, '\n' );
	if ( pNewline )
	{
		// advance past, leave \n
		pNewline++;
		*pNewline = 0;
	}
	else
	{
		pNewline = &dest[min( nBytesRead, destSize - 1)];
	}
	m_ReadPos = nStartPos + ( pNewline - dest ) + 1;

	return dest; 
}


#endif // _WIN32

#ifdef BUILD_GMOD
void CFileSystem_Stdio::RemoveSearchPathsByGroup(int group)
{
	Msg("CFileSystem_Stdio::RemoveSearchPathsByGroup\n");
	// Don't care
}

void CFileSystem_Stdio::SetGet(IGet* get)
{
	m_pIGet = get;
	Msg("CFileSystem_Stdio::SetGet\n");
}

ConVar fs_tellmeyoursecrets("fs_tellmeyoursecrets", "0");
class CAddonFileSystem : public Addon::FileSystem
{
public:
	virtual void Clear( );
	virtual void Refresh( );
	virtual int MountFile( const std::string &, std::vector<std::string> * );
	virtual bool ShouldMount( const std::string & );
	virtual bool ShouldMount( uint64_t );
	virtual void SetShouldMount( const std::string &, bool );
	virtual void Save( );
	virtual const std::list<IAddonSystem::Information> &GetList( ) const;
	virtual const std::list<IAddonSystem::UGCInfo> &GetUGCList( ) const;
	virtual void ScanForSubscriptions( CSteamAPIContext *, const char * );
	virtual void Think( );
	virtual void SetDownloadNotify( IAddonDownloadNotification * );
	virtual int Notify( );
	virtual bool IsSubscribed( uint64_t );
	virtual const IAddonSystem::Information *FindFileOwner( const std::string & );
	virtual void AddFile( const IAddonSystem::Information & );
	virtual void ClearAllGMAs( );
	virtual void GetSteamUGCFile( uint64_t, bool );
	virtual void UnmountAddon( uint64_t );
	virtual void UnmountServerAddons( );
	virtual void MountFloatingAddons( );
	virtual void Shutdown( );
	virtual void AddFile( const SteamUGCDetails_t & );
	virtual void AddSubscription( const SteamUGCDetails_t & );
	virtual void AddJob( Addon::Job::Base * );
	virtual bool HasChanges( );
	virtual void MarkChanged( );
	virtual void AddonDownloaded( IAddonSystem::Information & );
	virtual void NotifyAddonFailedToDownload( IAddonSystem::Information & );
	virtual const std::list<SteamUGCDetails_t> &GetSubList( ) const;
	virtual void IsAddonValidPreInstall( SteamUGCDetails_t );
	virtual void Load( );
private:
	std::list<IAddonSystem::Information> m_pAddons;
	std::list<IAddonSystem::UGCInfo> m_pUgcAddons;
	std::list<SteamUGCDetails_t> m_pSubscriptions;
	IAddonDownloadNotification* m_pDownloadNotify;
};

void CAddonFileSystem::Clear()
{
	Msg("CAddonFileSystem::Clear\n");
}

void CAddonFileSystem::Refresh()
{
	Msg("CAddonFileSystem::Refresh\n");
}

int CAddonFileSystem::MountFile(const std::string& file, std::vector<std::string>* filelist)
{
	Msg("CAddonFileSystem::MountFile\n");
	return 0;
}

bool CAddonFileSystem::ShouldMount(const std::string& file)
{
	Msg("CAddonFileSystem::ShouldMount\n");
	return false;
}

bool CAddonFileSystem::ShouldMount(uint64_t workshopid)
{
	Msg("CAddonFileSystem::ShouldMount\n");
	return false;
}

void CAddonFileSystem::SetShouldMount(const std::string& file, bool shouldmount)
{
	Msg("CAddonFileSystem::SetShouldMount\n");
}

void CAddonFileSystem::Save()
{
	Msg("CAddonFileSystem::Save\n");
}

const std::list<IAddonSystem::Information>& CAddonFileSystem::GetList( ) const
{
	Msg("CAddonFileSystem::GetList\n");
	return m_pAddons;
}

const std::list<IAddonSystem::UGCInfo>& CAddonFileSystem::GetUGCList( ) const
{
	Msg("CAddonFileSystem::GetUGCList\n");
	return m_pUgcAddons;
}

void CAddonFileSystem::ScanForSubscriptions(CSteamAPIContext* context, const char* unknown)
{
	Msg("CAddonFileSystem::ScanForSubscriptions\n");
}

void CAddonFileSystem::Think()
{
	//Msg("CAddonFileSystem::Think\n");
}

void CAddonFileSystem::SetDownloadNotify(IAddonDownloadNotification*)
{
	Msg("CAddonFileSystem::SetDownloadNotify\n");
}

int CAddonFileSystem::Notify()
{
	Msg("CAddonFileSystem::Notify\n");
	return 0;
}

bool CAddonFileSystem::IsSubscribed(uint64_t)
{
	Msg("CAddonFileSystem::IsSubscribed\n");
	return false;
}

const IAddonSystem::Information* CAddonFileSystem::FindFileOwner(const std::string&)
{
	Msg("CAddonFileSystem::FindFileOwner\n");
	return nullptr;
}

void CAddonFileSystem::AddFile(const IAddonSystem::Information&)
{
	Msg("CAddonFileSystem::AddFile\n");
}

void CAddonFileSystem::ClearAllGMAs()
{
	Msg("CAddonFileSystem::ClearAllGMAs\n");
}

void CAddonFileSystem::GetSteamUGCFile(uint64_t, bool)
{
	Msg("CAddonFileSystem::GetSteamUGCFile\n");
}

void CAddonFileSystem::UnmountAddon(uint64_t)
{
	Msg("CAddonFileSystem::UnmountAddon\n");
}

void CAddonFileSystem::UnmountServerAddons()
{
	Msg("CAddonFileSystem::UnmountServerAddons\n");
}

void CAddonFileSystem::MountFloatingAddons()
{
	Msg("CAddonFileSystem::MountFloatingAddons\n");
}

void CAddonFileSystem::Shutdown()
{
	Msg("CAddonFileSystem::Shutdown\n");
}

void CAddonFileSystem::AddFile(const SteamUGCDetails_t&)
{
	Msg("CAddonFileSystem::AddFile\n");
}

void CAddonFileSystem::AddSubscription(const SteamUGCDetails_t&)
{
	Msg("CAddonFileSystem::AddSubscription\n");
}

void CAddonFileSystem::AddJob(Addon::Job::Base*)
{
	Msg("CAddonFileSystem::AddJob\n");
}

bool CAddonFileSystem::HasChanges()
{
	Msg("CAddonFileSystem::HasChanges\n");
	return false;
}

void CAddonFileSystem::MarkChanged()
{
	Msg("CAddonFileSystem::MarkChanged\n");
}

void CAddonFileSystem::AddonDownloaded(IAddonSystem::Information&)
{
	Msg("CAddonFileSystem::AddonDownloaded\n");
}

void CAddonFileSystem::NotifyAddonFailedToDownload(IAddonSystem::Information&)
{
	Msg("CAddonFileSystem::NotifyAddonFailedToDownload\n");
}

const std::list<SteamUGCDetails_t>& CAddonFileSystem::GetSubList() const
{
	Msg("CAddonFileSystem::GetSubList\n");
	return m_pSubscriptions;
}

void CAddonFileSystem::IsAddonValidPreInstall(SteamUGCDetails_t)
{
	Msg("CAddonFileSystem::IsAddonValidPreInstall\n");
}

void CAddonFileSystem::Load()
{
	Msg("CAddonFileSystem::Load\n");
}

CAddonFileSystem g_pAddonFileSystem;
Addon::FileSystem* CFileSystem_Stdio::Addons()
{
	//Msg("CFileSystem_Stdio::Addons\n");
	return &g_pAddonFileSystem;
}

class CGamemodeSystem : public Gamemode::System
{
public:
	virtual void OnJoinServer( const std::string & );
	virtual void OnLeaveServer( );
	virtual void Refresh( );
	virtual void Clear( );
	virtual const IGamemodeSystem::Information &Active( );
	virtual IGamemodeSystem::Information FindByName( const std::string & );
	virtual void SetActive( const std::string & );
	virtual const std::list<IGamemodeSystem::Information> &GetList( ) const;
	virtual bool IsServerBlacklisted( char const*, char const*, char const*, char const*, char const* );

	CGamemodeSystem();
protected:
	void AddGamemode(std::string gamemode);

private:
	std::list<IGamemodeSystem::Information> m_pGamemodes;
	IGamemodeSystem::Information m_pActive;
};

void CGamemodeSystem::OnJoinServer(const std::string&)
{
	Msg("CGamemodeSystem::OnJoinServer\n");
}

void CGamemodeSystem::OnLeaveServer()
{
	Msg("CGamemodeSystem::OnLeaveServer\n");
}

uint64_t StringToUInt64(const char* str)
{
	char* end;
	return strtoull( str, &end, 10);
}

void CGamemodeSystem::Refresh()
{
	m_pGamemodes.clear();

	FileFindHandle_t findHandle;
	const char* pFilename = g_pFullFileSystem->FindFirstEx( "gamemodes/*", "GAME", &findHandle);
	while (pFilename)
	{
		std::string strFilename = pFilename;
		if (g_pFullFileSystem->FindIsDirectory( findHandle ))
		{
			std::string path = "gamemodes/" + strFilename + "/" + strFilename + ".txt";
			if (!g_pFullFileSystem->FileExists(path.c_str(), "GAME"))
			{
				pFilename = g_pFullFileSystem->FindNext( findHandle );
				continue;
			}

			// Gmod adds the gamemode inside Gamemode::System::AddGamemode

			KeyValues* kv = new KeyValues(pFilename);
			kv->LoadFromFile(g_pFullFileSystem, path.c_str(), "GAME");

			IGamemodeSystem::Information information;
			KeyValues* settings = kv->FindKey("settings");

			information.title = kv->GetString("title", "Missing Title");
			information.name = kv->GetString("name", pFilename);
			information.basename = kv->GetString("base", "base");
			information.category = kv->GetString("category", "");
			information.maps = kv->GetString("maps", "");
			information.menusystem = kv->GetBool("menusystem", false);
			information.dontcreate = kv->GetBool("dontcreate", false);
			information.workshopid = StringToUInt64(kv->GetString("workshopid", "0"));

			if (information.name == "")
				information.name = information.title;

			m_pGamemodes.push_back(information);

			// ToDo: Fix an exploit that I reported a good while ago about the gamemode system
		}

		pFilename = g_pFullFileSystem->FindNext( findHandle );
	}

	g_pFullFileSystem->FindClose( findHandle );

	if (m_pActive.title == "")
		m_pActive = FindByName("sandbox");

	Msg("CGamemodeSystem::Refresh\n");
}

void CGamemodeSystem::Clear()
{
	Msg("CGamemodeSystem::Clear\n");
}

const IGamemodeSystem::Information& CGamemodeSystem::Active()
{
	Msg("CGamemodeSystem::Active\n");
	return m_pActive;
}

IGamemodeSystem::Information CGamemodeSystem::FindByName(const std::string& gamemode)
{
	Msg("CGamemodeSystem::FindByName\n");
	for ( IGamemodeSystem::Information info : m_pGamemodes )
	{
		if ( info.name == gamemode )
			return info;
	}

	IGamemodeSystem::Information info;
	info.basename = "INVALID";

	return info;
}

void CGamemodeSystem::SetActive(const std::string& gamemode)
{
	Msg("CGamemodeSystem::SetActive %s\n", gamemode.c_str());
	IGamemodeSystem::Information info = FindByName(gamemode);
	if ( info.basename != "INVALID" )
		m_pActive = info;
}

const std::list<IGamemodeSystem::Information>& CGamemodeSystem::GetList() const
{
	Msg("CGamemodeSystem::GetList\n");
	return m_pGamemodes;
}

bool CGamemodeSystem::IsServerBlacklisted(char const* address, char const* hostname, char const* description, char const* gm, char const* map)
{
	Msg("CGamemodeSystem::IsServerBlacklisted\n");
	// ToDo: Why do u crash :<
	return false; //((CBaseFileSystem*)g_pFullFileSystem)->GetIGet()->MenuSystem()->IsServerBlacklisted(address, hostname, description, gm, map);
}

CGamemodeSystem::CGamemodeSystem()
{
	Refresh();
	SetActive( "sandbox" );
}

CGamemodeSystem g_pGamemodeSystem;
static void gamemode_Changed( IConVar *var, const char *pOldValue, float flOldValue )
{
	g_pGamemodeSystem.SetActive( ((ConVar*)var)->GetString() );
}
ConVar gamemode("gamemode", "sandbox", 0, "The current gamemode", gamemode_Changed);

Gamemode::System* CFileSystem_Stdio::Gamemodes()
{
	Msg("CFileSystem_Stdio::Gamemodes\n");
	return &g_pGamemodeSystem;
}

class CGameDepotySystem : public GameDepot::System
{
public:
	virtual void Refresh( );
	virtual void Clear( );
	virtual void Save( );
	virtual void SetMount( uint32_t, bool );
	virtual void MarkGameAsMounted( const std::string );
	virtual const std::list<IGameDepotSystem::Information> &GetList( ) const;
	virtual void MountAsMapFix( uint32_t );
	virtual void MountCurrentGame( const std::string & );
private:
	std::list<IGameDepotSystem::Information> m_pGames;
};

void CGameDepotySystem::Refresh()
{
	Msg("CGameDepotySystem::Refresh\n");
}

void CGameDepotySystem::Clear()
{
	Msg("CGameDepotySystem::Clear\n");
}

void CGameDepotySystem::Save()
{
	Msg("CGameDepotySystem::Save\n");
}

void CGameDepotySystem::SetMount(uint32_t, bool)
{
	Msg("CGameDepotySystem::SetMount\n");
}

void CGameDepotySystem::MarkGameAsMounted(const std::string)
{
	Msg("CGameDepotySystem::MarkGameAsMounted\n");
}

const std::list<IGameDepotSystem::Information>& CGameDepotySystem::GetList() const
{
	Msg("CGameDepotySystem::GetList\n");
	return m_pGames;
}

void CGameDepotySystem::MountAsMapFix(uint32_t)
{
	Msg("CGameDepotySystem::MountAsMapFix\n");
}

void CGameDepotySystem::MountCurrentGame(const std::string&)
{
	Msg("CGameDepotySystem::MountCurrentGame\n");
}

CGameDepotySystem g_pGameDepoSystem;
GameDepot::System* CFileSystem_Stdio::Games()
{
	Msg("CFileSystem_Stdio::Games\n");
	return &g_pGameDepoSystem;
}

class CLegacyAddonSystem : public LegacyAddons::System
{
public:
	virtual void Refresh( );
	virtual const std::list<ILegacyAddons::Information> &GetList( ) const;
private:
	std::list<ILegacyAddons::Information> m_pAddons;
};

void CLegacyAddonSystem::Refresh()
{
	for ( ILegacyAddons::Information info : m_pAddons )
	{
		g_pFullFileSystem->RemoveSearchPath( info.path.c_str(), "GAME" );
		g_pFullFileSystem->RemoveSearchPath( info.path.c_str(), "thirdparty" );
	}

	m_pAddons.clear();

	FileFindHandle_t findHandle;
	const char* pFilename = g_pFullFileSystem->FindFirstEx("addons/*", "MOD", &findHandle );
	while ( pFilename )
	{
		if( g_pFullFileSystem->FindIsDirectory( findHandle ) )
		{
			std::string path = "addons/";
			path = path + pFilename;

			char fullpath[1024];
			g_pFullFileSystem->RelativePathToFullPath( path.c_str(), "MOD", fullpath, sizeof(fullpath) );


			g_pFullFileSystem->AddSearchPath(fullpath, "GAME");
			g_pFullFileSystem->AddSearchPath(fullpath, "thirdparty");

			ILegacyAddons::Information information;
			information.name = pFilename;
			information.path = (std::string)fullpath;
			information.luapath = path;
			information.placeholder4 = ""; // ToDo: Find out.

			m_pAddons.push_back(information);
		}

		pFilename = g_pFullFileSystem->FindNext( findHandle );
	}

	Msg("CLegacyAddonSystem::Refresh\n");
}

const std::list<ILegacyAddons::Information>& CLegacyAddonSystem::GetList() const
{
	Msg("CLegacyAddonSystem::GetList\n");
	return m_pAddons;
}

CLegacyAddonSystem g_pLegacyAddonSystem;
LegacyAddons::System* CFileSystem_Stdio::LegacyAddons()
{
	Msg("CFileSystem_Stdio::LegacyAddons\n");
	return &g_pLegacyAddonSystem;
}

class Language2 : public CLanguage
{
public:
	virtual void ChangeLanguage( const char * );
	virtual void ChangeLanguage_Steam( const char * );
	virtual void ReloadLanguage( );
	virtual void GetString( const char *, wchar_t *, int32_t );
	virtual void UpdateSourceEngineLanguage( );
protected:
	void ProcessFile( std::string, const char* );
	void TellLuaLanguageChanged( const char* );

private:
	std::unordered_map<std::string, std::string> m_pStrings;
};

extern Language2 g_pLanguage;
static void language_Changed( IConVar *var, const char *pOldValue, float flOldValue )
{
	g_pLanguage.ReloadLanguage();
}
ConVar gmod_language( "gmod_language", "en", 0, "Changes language of Garry's mod", language_Changed );

void Language2::ChangeLanguage( const char* lang )
{
	ProcessFile( lang, "" );
	TellLuaLanguageChanged( lang );
	Msg( "Language::ChangeLanguage %s\n", lang );
}

void Language2::ChangeLanguage_Steam(const char* lang)
{
	g_pLanguage.ReloadLanguage(); // Should fix a bug where the Language is just not set.
	Msg("Language::ChangeLanguage_Steam %s\n", lang);
}

void Language2::ReloadLanguage()
{
	g_pLanguage.ChangeLanguage( gmod_language.GetString() );
	Msg("Language::ReloadLanguage\n");
}

std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
void Language2::GetString(const char* inputString, wchar_t* outputString, int32_t bufferSize)
{
	auto it = m_pStrings.find(inputString);
	if (it == m_pStrings.end())
	{
		std::wcsncpy(outputString, converter.from_bytes(inputString).c_str(), bufferSize);
		return;
	}

	std::wcsncpy(outputString, converter.from_bytes(it->second).c_str(), bufferSize);
	Msg("Language::GetString %s\n", inputString);
}

void Language2::UpdateSourceEngineLanguage()
{
	Msg("Language::UpdateSourceEngineLanguage\n");
}

// Workaround to relace all \ in language strings. Gmod uses them for something but idk.
std::string ReplaceAll( std::string str, const std::string& from, const std::string& to ) {
    size_t start_pos = 0;
    while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos ) {
        str.replace( start_pos, from.length(), to );
        start_pos += to.length();
    }

    return str;
}

void Language2::ProcessFile( std::string language, const char* idk ) // Gmod does this definetly differently but this completly works
{
	m_pStrings.clear();

	std::string lang_path = "resource/localization/" + language + "/";

	FileFindHandle_t findHandle;
	const char* pFilename = g_pFullFileSystem->FindFirstEx( (lang_path + "*.properties").c_str(), "MOD", &findHandle );
	while (pFilename)
	{
		FileHandle_t fh = g_pFullFileSystem->Open( (lang_path + pFilename).c_str(), "r", "MOD" );
		if(fh)
		{
			int max_size = g_pFullFileSystem->Size( fh ) + 1;
			char* content = new char[ max_size ];
			g_pFullFileSystem->Read( content, max_size, fh );

			char* token = std::strtok( content, "\n" );
			while ( token != nullptr ) { // Loops thru each line and checks if a "=" exists.
				char* equalsSign = std::strchr( token, '=' );
				if ( equalsSign != nullptr ) {
					*equalsSign = '\0';
					std::string key( token );
					std::string value( equalsSign + 1 );
					m_pStrings[ key ] = ReplaceAll( value, "\\", "" );
				}

				token = std::strtok( nullptr, "\n" );
			}

			delete[] content;
		}
		g_pFullFileSystem->Close( fh );

		pFilename = g_pFullFileSystem->FindNext( findHandle );
	}

	g_pFullFileSystem->FindClose( findHandle );
}

void Language2::TellLuaLanguageChanged( const char* language )
{
	IGet* get = ((CBaseFileSystem*)g_pFullFileSystem)->GetIGet();
	if (!get)
		return;

	GarrysMod::Lua::ILuaShared* shared = (GarrysMod::Lua::ILuaShared*)get->LuaShared();
	GarrysMod::Lua::ILuaInterface* LUA = shared->GetLuaInterface( GarrysMod::Lua::State::MENU );

	if ( LUA )
	{
		LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
			LUA->GetField( -1, "LanguageChanged" );
			if ( LUA->IsType(-1, GarrysMod::Lua::Type::Function ) )
			{
				LUA->PushString( language );
				LUA->Call( 1, 0 );
			} else {
				LUA->Pop(1);
			}

		LUA->Pop(1);
	}
}

Language2 g_pLanguage;
CLanguage* CFileSystem_Stdio::Language( )
{
	Msg("CFileSystem_Stdio::Language\n");
	return &g_pLanguage;
}

void CFileSystem_Stdio::DoFilesystemRefresh()
{
	Msg("CFileSystem_Stdio::DoFilesystemRefresh\n");
}

int CFileSystem_Stdio::LastFilesystemRefresh()
{
	//Msg("CFileSystem_Stdio::LastFilesystemRefresh\n");
	return 1;
}

void CFileSystem_Stdio::AddVPKFileFromPath(const char *vpk, const char *path, unsigned int id)
{
	Msg("CFileSystem_Stdio::AddVPKFileFromPath called with %s %s (%u)\n", vpk, path, id);
	AddVPKFile(vpk, path, (SearchPathAdd_t)id);
}

void CFileSystem_Stdio::GMOD_SetupDefaultPaths(const char *path, const char *game)
{
	Msg("CFileSystem_Stdio::GMOD_SetupDefaultPaths called with %s %s\n", path, game);

	std::string strpath = path;
	std::string folder = strpath + "/" + game + "/";

	AddSearchPath((strpath + "/bin/").c_str(), "EXECUTABLE_PATH", PATH_ADD_TO_TAIL);
	AddSearchPath((strpath + "/bin/").c_str(), "BASE_PATH", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "bin/").c_str(), "GAMEBIN", PATH_ADD_TO_TAIL);
	AddSearchPath(folder.c_str(), "LOGDIR", PATH_ADD_TO_TAIL);
	AddSearchPath((strpath + "/platform/config/").c_str(), "CONFIG", PATH_ADD_TO_TAIL);
	AddSearchPath((strpath + "/platform/").c_str(), "PLATFORM", PATH_ADD_TO_TAIL);

	AddSearchPath(folder.c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddSearchPath(folder.c_str(), "MOD_WRITE", PATH_ADD_TO_TAIL);
	AddSearchPath(folder.c_str(), "DEFAULT_WRITE_PATH", PATH_ADD_TO_TAIL);
	AddSearchPath(folder.c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddSearchPath(folder.c_str(), "GAME_WRITE", PATH_ADD_TO_TAIL);
	AddSearchPath(folder.c_str(), "garrysmod", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "data/").c_str(), "DATA", PATH_ADD_TO_TAIL);

	AddSearchPath((folder + "download/").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "download/").c_str(), "DOWNLOAD", PATH_ADD_TO_TAIL);

	AddVPKFile((folder + "garrysmod_dir.vpk").c_str(), "garrysmod", PATH_ADD_TO_TAIL);
	AddVPKFile((folder + "garrysmod_dir.vpk").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((folder + "garrysmod_dir.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);

	AddSearchPath((folder + "workshop/").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "workshop/").c_str(), "workshop", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "workshop/").c_str(), "thirdparty", PATH_ADD_TO_TAIL);

	AddSearchPath((folder + "overrides/").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "overrides/").c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "overrides/").c_str(), "garrysmod", PATH_ADD_TO_TAIL);
	
	AddVPKFile((folder + "fallbacks_dir.vpk").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((folder + "fallbacks_dir.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "fallbacks/").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "fallbacks/").c_str(), "MOD", PATH_ADD_TO_TAIL);

	AddVPKFile((folder + "fallbacks_dir.vpk").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((folder + "fallbacks_dir.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "fallbacks/").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddSearchPath((folder + "fallbacks/").c_str(), "MOD", PATH_ADD_TO_TAIL);

	std::string sourceengine = path;
	sourceengine = sourceengine + "/sourceengine/";
	AddSearchPath(sourceengine.c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_misc.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_misc.vpk").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_sound_misc.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_sound_misc.vpk").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_sound_vo_english.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_sound_vo_english.vpk").c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_textures.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);
	AddVPKFile((sourceengine + "hl2_textures.vpk").c_str(), "GAME", PATH_ADD_TO_TAIL);

	std::string platform = path;
	platform = platform + "/platform/";
	AddSearchPath(platform.c_str(), "GAME", PATH_ADD_TO_TAIL);
	AddVPKFile((platform + "platform_misc.vpk").c_str(), "MOD", PATH_ADD_TO_TAIL);

	PrintSearchPaths();
}

void CFileSystem_Stdio::GMOD_FixPathCase(char *a, size_t b)
{
	Msg("CFileSystem_Stdio::GMOD_FixPathCase\n");
}
#endif