//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//===========================================================================//
#include "tier2/riff.h"
#include "snd_io.h"
#include "filesystem.h"
#include "filesystem_engine.h"
#include "tier1/strtools.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: Implements Audio IO on the engine's COMMON filesystem
//-----------------------------------------------------------------------------
class COM_IOReadBinary : public IFileReadBinary
{
public:
	intp open( const char *pFileName );
	int read( void *pOutput, int size, intp file );
	void seek( intp file, int pos );
	unsigned int tell( intp file );
	unsigned int size( intp file );
	void close( intp file );
};


// prepend sound/ to the filename -- all sounds are loaded from the sound/ directory
intp COM_IOReadBinary::open( const char *pFileName )
{
	char namebuffer[512];
	Q_strncpy( namebuffer, "sound", sizeof( namebuffer ) );

	// the server is sending back sound names with slashes in front... 
	if ( pFileName[0] != '/' && pFileName[0] != '\\' )
	{
		Q_strncat( namebuffer, "/", sizeof( namebuffer ), COPY_ALL_CHARACTERS );
	}

	Q_strncat( namebuffer, pFileName, sizeof( namebuffer ), COPY_ALL_CHARACTERS );

	FileHandle_t hFile = g_pFileSystem->Open(namebuffer, "rb", "GAME");

	return (intp)hFile;
}

int COM_IOReadBinary::read( void *pOutput, int size, intp file )
{
	if ( !file )
		return 0;

	return g_pFileSystem->Read( pOutput, size, (FileHandle_t)file );
}

void COM_IOReadBinary::seek( intp file, int pos )
{
	if ( !file )
		return;

	g_pFileSystem->Seek( (FileHandle_t)file, pos, FILESYSTEM_SEEK_HEAD );
}

unsigned int COM_IOReadBinary::tell( intp file )
{
	if ( !file )
		return 0;

	return g_pFileSystem->Tell( (FileHandle_t)file );
}

unsigned int COM_IOReadBinary::size( intp file )
{
	if (!file)
		return 0;

	return g_pFileSystem->Size( (FileHandle_t)file );
}

void COM_IOReadBinary::close( intp file )
{
	if (!file)
		return;

	g_pFileSystem->Close( (FileHandle_t)file );
}

static COM_IOReadBinary io;
IFileReadBinary *g_pSndIO = &io;

