#include "cbase.h"
#include "gmod_stringtable.h"

INetworkStringTable* GarrysMod::StringTable::g_StringTable_GameInfo;

void GarrysMod::StringTable::Reset()
{
	// ToDo
}

#ifdef CLIENT_DLL
#include "networkstringtable_clientdll.h"
void GarrysMod::StringTable::Install( const char* tableName )
{
	if ( V_stricmp( tableName, "GModGameInfo" ) )
	{
		g_StringTable_GameInfo = networkstringtable->FindTable( tableName );
	}
}
#else
#include "networkstringtable_gamedll.h"
void GarrysMod::StringTable::Create()
{
	g_StringTable_GameInfo = networkstringtable->CreateStringTable( "GModGameInfo", 16 );
}
#endif