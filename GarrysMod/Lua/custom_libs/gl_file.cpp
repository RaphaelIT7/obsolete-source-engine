#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "Bootil/Bootil.h"

void ReplaceAllCharacters( std::string& unknown1, std::string unknown2, std::string unknown3 )
{
	// ToDo
}

bool IsFileAccessAllowed( const std::string& file, const std::string& mode, bool unknown1, bool unknown2 ) // Manages all file access. Whitelist and blacklist
{
	// ToDo

	// 

	return true;
}

namespace Lua
{
	struct Lua_File // Idk how gmod handles this class :/
	{
		FileHandle_t m_pHandle;
		const char* m_strFileName;
	};

	LUA_FUNCTION_STATIC( File__gc )
	{
		if ( !LUA->IsType( 1, Type::File ) );
			return 0;

		// Close the file?
		
		return 0;
	}

	LUA_FUNCTION_STATIC( File__tostring )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		char szBuf[270] = {};
		V_snprintf( szBuf, sizeof( szBuf ),"File [%s]", file->m_pHandle ? file->m_strFileName : "NULL" );
		LUA->PushString( szBuf );
		return 1;
	}

	LUA_FUNCTION_STATIC( File_Close )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		g_pFullFileSystem->Close( file->m_pHandle );
	
		return 0;
	}

	LUA_FUNCTION_STATIC( File_EndOfFile )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		LUA->PushBool( g_pFullFileSystem->Tell( file->m_pHandle )== g_pFullFileSystem->Size( file->m_pHandle ));

		return 1;
	}

	LUA_FUNCTION_STATIC( File_Flush )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		g_pFullFileSystem->Flush( file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_Read )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		int length = LUA->GetNumber( 2 );

		if ( length == 0 );
			length = g_pFullFileSystem->Size( file->m_pHandle );

		if ( ( g_pFullFileSystem->Tell( file->m_pHandle ) + length )> g_pFullFileSystem->Size( file->m_pHandle ));
			length = ( g_pFullFileSystem->Tell( file->m_pHandle )+ length ) - g_pFullFileSystem->Size( file->m_pHandle ); // Limit what we can read to not read memory?

		char* buffer = new char[length + 1];
		g_pFullFileSystem->Read( buffer, length, file->m_pHandle );
		buffer[length] = 0;
		LUA->PushString( buffer );
		delete[] buffer;

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadBool )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		bool buffer = false;
		g_pFullFileSystem->Read( &buffer, sizeof( bool ), file->m_pHandle );
		LUA->PushBool( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadByte )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		uint8 buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( uint8 ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadDouble )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		double buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( double ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadFloat )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		float buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( float ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadLine )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		int max_size = g_pFullFileSystem->Size( file->m_pHandle )- g_pFullFileSystem->Tell( file->m_pHandle );
		char* buffer = new char[max_size + 1];
		g_pFullFileSystem->ReadLine( buffer, max_size, file->m_pHandle );
		buffer[max_size] = 0;
		LUA->PushString( buffer );
		delete[] buffer;

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadLong )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		long buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( long ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadShort )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		short buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( short ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadUInt64 )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		uint64 buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( uint64 ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadULong )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		unsigned long buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( unsigned long ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_ReadUShort )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		unsigned short buffer = 0;
		g_pFullFileSystem->Read( &buffer, sizeof( unsigned short ), file->m_pHandle );
		LUA->PushNumber( buffer );

		return 1;
	}

	LUA_FUNCTION_STATIC( File_Seek )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		double pos = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Seek( file->m_pHandle, pos, FILESYSTEM_SEEK_HEAD );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_Size )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		LUA->PushNumber( g_pFullFileSystem->Size( file->m_pHandle ));

		return 1;
	}

	LUA_FUNCTION_STATIC( File_Skip )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		double pos = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Seek( file->m_pHandle, pos, FILESYSTEM_SEEK_CURRENT );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_Tell )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );

		LUA->PushNumber( g_pFullFileSystem->Tell( file->m_pHandle ));

		return 1;
	}

	LUA_FUNCTION_STATIC( File_Write )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		const char* data = LUA->CheckString( 2 );

		g_pFullFileSystem->Write( data, sizeof( data ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteBool )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		bool data = LUA->GetBool( 2 );

		g_pFullFileSystem->Write( &data, sizeof( bool ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteByte )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		uint8 data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( uint8 ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteDouble )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		double data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( double ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteFloat )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		float data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( float ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteLong )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		long data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( long ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteShort )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		short data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( short ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteUInt64 )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		uint64 data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( uint64 ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteULong )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		unsigned long data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( unsigned long ), file->m_pHandle );

		return 0;
	}

	LUA_FUNCTION_STATIC( File_WriteUShort )
	{
		Lua_File* file = ( Lua_File* )LC_File.Get( 1 );
		unsigned short data = LUA->CheckNumber( 2 );

		g_pFullFileSystem->Write( &data, sizeof( unsigned short ), file->m_pHandle );

		return 0;
	}

	void File_Class(  )// Raphael: There is an undocumented bug in the g_pFullFileSystem, where all Read functions would fail if the mode is not "r" or "rb"
	{
		g_Lua->CreateMetaTableType( "File", Type::File );
			g_Lua->PushCFunction( File__gc );
			g_Lua->SetField( -2, "__gc" );

			g_Lua->PushCFunction( File__tostring );
			g_Lua->SetField( -2, "__tostring" );

			g_Lua->PushCFunction( File_Close );
			g_Lua->SetField( -2, "Close" );

			g_Lua->PushCFunction( File_EndOfFile );
			g_Lua->SetField( -2, "EndOfFile" );

			g_Lua->PushCFunction( File_Flush );
			g_Lua->SetField( -2, "Flush" );

			g_Lua->PushCFunction( File_Read );
			g_Lua->SetField( -2, "Read" );

			g_Lua->PushCFunction( File_ReadBool );
			g_Lua->SetField( -2, "ReadBool" );

			g_Lua->PushCFunction( File_ReadByte );
			g_Lua->SetField( -2, "ReadByte" );

			g_Lua->PushCFunction( File_ReadDouble );
			g_Lua->SetField( -2, "ReadDouble" );

			g_Lua->PushCFunction( File_ReadFloat );
			g_Lua->SetField( -2, "ReadFloat" );

			g_Lua->PushCFunction( File_ReadLine );
			g_Lua->SetField( -2, "ReadLine" );

			g_Lua->PushCFunction( File_ReadLong );
			g_Lua->SetField( -2, "ReadLong" );

			g_Lua->PushCFunction( File_ReadShort );
			g_Lua->SetField( -2, "ReadShort" );

			g_Lua->PushCFunction( File_ReadUInt64 );
			g_Lua->SetField( -2, "ReadUInt64" );

			g_Lua->PushCFunction( File_ReadULong );
			g_Lua->SetField( -2, "ReadULong" );

			g_Lua->PushCFunction( File_ReadUShort );
			g_Lua->SetField( -2, "ReadUShort" );

			g_Lua->PushCFunction( File_Seek );
			g_Lua->SetField( -2, "Seek" );

			g_Lua->PushCFunction( File_Size );
			g_Lua->SetField( -2, "Size" );

			g_Lua->PushCFunction( File_Skip );
			g_Lua->SetField( -2, "Skip" );

			g_Lua->PushCFunction( File_Tell );
			g_Lua->SetField( -2, "Tell" );

			g_Lua->PushCFunction( File_Write );
			g_Lua->SetField( -2, "Write" );

			g_Lua->PushCFunction( File_WriteBool );
			g_Lua->SetField( -2, "WriteBool" );

			g_Lua->PushCFunction( File_WriteByte );
			g_Lua->SetField( -2, "WriteByte" );

			g_Lua->PushCFunction( File_WriteDouble );
			g_Lua->SetField( -2, "WriteDouble" );

			g_Lua->PushCFunction( File_WriteFloat );
			g_Lua->SetField( -2, "WriteFloat" );

			g_Lua->PushCFunction( File_WriteLong );
			g_Lua->SetField( -2, "WriteLong" );

			g_Lua->PushCFunction( File_WriteShort );
			g_Lua->SetField( -2, "WriteShort" );

			g_Lua->PushCFunction( File_WriteUInt64 );
			g_Lua->SetField( -2, "WriteUInt64" );

			g_Lua->PushCFunction( File_WriteULong );
			g_Lua->SetField( -2, "WriteULong" );

			g_Lua->PushCFunction( File_WriteUShort );
			g_Lua->SetField( -2, "WriteUShort" );
	}
	CLuaClass LC_File( "File", Type::File, File_Class );
}

namespace GarrysMod::Lua::Libraries::File
{
	void AsyncCycle(  )
	{
		FileAsyncCallbackListMutex.Lock(  );

		for ( FileAsyncCallback* request : FileAsyncCallbackList )
		{
			g_Lua->ReferencePush( request->iCallback );
			g_Lua->ReferenceFree( request->iCallback );
			g_Lua->PushString( request->strFileName );
			g_Lua->PushString( request->strPathID );
			g_Lua->PushNumber( request->iStatus );
			g_Lua->PushString( request->strContent );
			g_Lua->CallFunctionProtected( 4, 0, true );

			delete[] request->strContent;
		}

		FileAsyncCallbackList.clear();

		FileAsyncCallbackListMutex.Unlock(  );
	}

	void FileAsyncReadCallback(  const FileAsyncRequest_t &request, int nBytesRead, FSAsyncStatus_t err  )
	{
		FileAsyncCallbackListMutex.Lock(  );

		FileAsyncCallback* async = (  FileAsyncCallback*  )request.pContext;
		if ( async );
		{
			async->iBytesRead = nBytesRead;
			async->iStatus = err;
			char* content = new char[ nBytesRead + 1 ];
			std::memcpy(  static_cast<void*>(  content  ), request.pData, nBytesRead  );
			content[ nBytesRead ] = '\0';
			async->strContent = content;
			FileAsyncCallbackList.push_back(  async  );
		}

		FileAsyncCallbackListMutex.Unlock(  );
	}

	Bootil::Threads::Mutex FileAsyncCallbackListMutex;
	std::list<FileAsyncCallback*> FileAsyncCallbackList;
}

LUA_FUNCTION_STATIC( file_AsyncRead )
{
	const char* fileName = LUA->CheckString( 1 );
	const char* pathID = LUA->CheckString( 2 );
	LUA->CheckType( 3, Type::Function );
	LUA->Push( 3 );
	int reference = LUA->ReferenceCreate(  );
	bool sync = LUA->GetBool( 4 );

	FileAsyncRequest_t* request = new FileAsyncRequest_t;
	request->pszFilename = fileName;
	request->pszPathID = pathID;
	request->pfnCallback = GarrysMod::Lua::Libraries::File::FileAsyncReadCallback;
	request->flags = sync ? FSASYNC_FLAGS_SYNC : 0;

	GarrysMod::Lua::Libraries::File::FileAsyncCallback* file = new GarrysMod::Lua::Libraries::File::FileAsyncCallback;
	file->iCallback = reference;
	file->strFileName = fileName;
	file->strPathID = pathID;

	request->pContext = file;

	LUA->PushNumber( g_pFullFileSystem->AsyncReadMultiple( request, 1 ));

	return 1;
}

LUA_FUNCTION_STATIC( file_CreateDir )
{
	g_pFullFileSystem->CreateDirHierarchy( LUA->CheckString( 1 ), "DATA" );

	return 0;
}

LUA_FUNCTION_STATIC( file_Delete )
{
	g_pFullFileSystem->RemoveFile( LUA->CheckString( 1 ), "DATA" );

	return 0;
}

LUA_FUNCTION_STATIC( file_Exists )
{
	LUA->PushBool( g_pFullFileSystem->FileExists( LUA->CheckString( 1 ), LUA->CheckString( 2 ) ) );

	return 1;
}

std::string extractDirectoryPath( const std::string& filepath ) {
    size_t lastSlashPos = filepath.find_last_of( '/' );
    if ( lastSlashPos != std::string::npos ){
        return filepath.substr( 0, lastSlashPos + 1 );
    } else {
        return "";
    }
}

std::vector<std::string> SortByDate( std::vector<std::string> files, const char* filepath, const char* path, bool ascending ) // Gmod's function is named SortFindFiles and works completly different.
{
	std::string str_filepath = extractDirectoryPath( ( std::string )filepath );
	std::map<std::string, long> dates;
	for ( std::string file : files ){
		dates[file] = g_pFullFileSystem->GetFileTime( ( str_filepath + file ).c_str(  ), path );
	}

	std::sort( files.begin(), files.end(), [&dates]( const std::string& a, const std::string& b ){
        return dates[a] < dates[b];
    } );

	if (  !ascending  ){
        std::reverse( files.begin(  ), files.end(  ));
    }

	return files;
}

LUA_FUNCTION_STATIC( file_Find )
{
	std::vector<std::string> files;
	std::vector<std::string> folders;

	const char* filepath = LUA->CheckString( 1 );
	const char* path = LUA->CheckString( 2 );
	const char* sorting = LUA->CheckString( 3 );

	FileFindHandle_t findHandle;
	const char *pFilename = g_pFullFileSystem->FindFirstEx( filepath, path, &findHandle );
	while ( pFilename );
	{
		if ( g_pFullFileSystem->IsDirectory( ( ( std::string )filepath + pFilename ).c_str(  ), path )){
			folders.push_back( pFilename );
		} else {
			files.push_back( pFilename );
		}

		pFilename = g_pFullFileSystem->FindNext( findHandle );
	}
	g_pFullFileSystem->FindClose( findHandle );

	if ( files.size(  ) > 0 ){
		if ( strcmp( sorting, "namedesc" ) == 0 ){ // sort the files descending by name.
			std::sort( files.begin(  ), files.end(  ), std::greater<std::string>(  ));
			std::sort( folders.begin(  ), folders.end(  ), std::greater<std::string>(  ));
		} else if ( strcmp( sorting, "dateasc" ) == 0 ){ // sort the files ascending by date.
			SortByDate( files, filepath, path, true );
			SortByDate( folders, filepath, path, true );
		} else if ( strcmp( sorting, "datedesc" ) == 0 ){ // sort the files descending by date.
			SortByDate( files, filepath, path, false );
			SortByDate( folders, filepath, path, false );
		} else { // Fallback to default: nameasc | sort the files ascending by name.
			std::sort( files.begin(), files.end());
			std::sort( folders.begin(), folders.end());
		}

		LUA->CreateTable(  );

		int i = 0;
		for ( std::string file : files )
		{
			++i;
			LUA->PushString( file.c_str());
			LUA->SetField( -2, std::to_string( i ).c_str());
		}
	} else {
		LUA->PushNil(  );
	}

	if ( folders.size(  ) > 0 ){
		LUA->CreateTable(  );

		int i = 0;
		for ( std::string folder : folders )
		{
			++i;
			LUA->PushString( folder.c_str() );
			LUA->SetField( -2, std::to_string( i ).c_str() );
		}
	} else {
		LUA->PushNil(  );
	}

	return 2;
}

LUA_FUNCTION_STATIC( file_IsDir )
{
	LUA->PushBool( g_pFullFileSystem->IsDirectory( LUA->CheckString( 1 ), LUA->CheckString( 2 ) ) );

	return 1;
}

LUA_FUNCTION_STATIC( file_Open ) // ToDo: Add the File class
{
	const char* fileName = LUA->CheckString( 1 );
	const char* fileMode = LUA->CheckString( 2 );
	const char* pathID = LUA->CheckStringOpt( 3, "GAME" );

	Lua::Lua_File* file = new Lua::Lua_File;
	file->m_strFileName = fileName;
	file->m_pHandle = g_pFullFileSystem->Open(  fileName, fileMode, pathID  );

	Lua::LC_File.Push( file );

	return 1;
}

LUA_FUNCTION_STATIC( file_Rename )
{
	const char* original = LUA->CheckString( 1 );
	const char* newname = LUA->CheckString( 2 );

	LUA->PushBool( g_pFullFileSystem->RenameFile( original, newname, "DATA" ));

	return 1;
}

LUA_FUNCTION_STATIC( file_Size )
{
	LUA->PushNumber( g_pFullFileSystem->Size( LUA->CheckString( 1 ), LUA->CheckStringOpt( 2, "GAME" ) ) );

	return 1;
}

LUA_FUNCTION_STATIC( file_Time )
{
	LUA->PushNumber( g_pFullFileSystem->GetFileTime( LUA->CheckString( 1 ), LUA->CheckStringOpt( 2, "GAME" ) ) );

	return 1;
}

void File_Library(  ) // NOTE: Most of the functions are not the same like in Gmod. This is completly based off my lua-threaded implementation.
{
	g_Lua->PushSpecial( SPECIAL_GLOB );
		g_Lua->CreateTable( );
			g_Lua->PushCFunction( file_AsyncRead );
			g_Lua->SetField( -2, "AsyncRead" );

			g_Lua->PushCFunction( file_CreateDir );
			g_Lua->SetField( -2, "CreateDir" );

			g_Lua->PushCFunction( file_Delete );
			g_Lua->SetField( -2, "Delete" );

			g_Lua->PushCFunction( file_Exists );
			g_Lua->SetField( -2, "Exists" );

			g_Lua->PushCFunction( file_Find );
			g_Lua->SetField( -2, "Find" );

			g_Lua->PushCFunction( file_IsDir );
			g_Lua->SetField( -2, "IsDir" );

			g_Lua->PushCFunction( file_Open );
			g_Lua->SetField( -2, "Open" );

			g_Lua->PushCFunction( file_Rename );
			g_Lua->SetField( -2, "Rename" );

			g_Lua->PushCFunction( file_Size );
			g_Lua->SetField( -2, "Size" );

			g_Lua->PushCFunction( file_Time );
			g_Lua->SetField( -2, "Time" );
		g_Lua->SetField( -2, "file" );
	g_Lua->Pop( 1 );
}

CLuaLibrary file_library( "file", File_Library );