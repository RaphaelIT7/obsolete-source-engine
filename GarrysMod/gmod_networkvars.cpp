#include "cbase.h"
#include "gmod_networkvars.h"

INetworkStringTable* NetworkString::pStringTable;
INetworkStringTable* NetworkVarNames::pStringTable;

#ifdef CLIENT_DLL
#include "networkstringtable_clientdll.h"
void NetworkString::Install()
{
	pStringTable = networkstringtable->FindTable( "networkstring" );
}
#else
#include "networkstringtable_gamedll.h"
void NetworkString::Create()
{
	pStringTable = networkstringtable->CreateStringTable( "networkstring", 4096 );
}
#endif

void NetworkString::Add( const char* strVar )
{
	if ( pStringTable->FindStringIndex( strVar ) != INVALID_STRING_INDEX )
		return;

	pStringTable->AddString( true, strVar );
}

int NetworkString::Get( const char* strVar )
{
	return pStringTable->FindStringIndex( strVar );
}

const char* NetworkString::Convert( int iIdx )
{
	return pStringTable->GetString( iIdx );
}

#ifdef CLIENT_DLL
void NetworkVarNames::Install()
{
	pStringTable = networkstringtable->FindTable( "networkvars" );
}
#else
void NetworkVarNames::Create()
{
	pStringTable = networkstringtable->CreateStringTable( "networkvars", 4096 );
}
#endif

void NetworkVarNames::Add( const char* strVar )
{
	if ( pStringTable->FindStringIndex( strVar ) != INVALID_STRING_INDEX )
		return;

	int idx = pStringTable->AddString( true, strVar );
	if ( idx == INVALID_STRING_INDEX )
		Warning( "Too many NWVar names, could not add name %s", strVar );
}

int NetworkVarNames::Get( const char* strVar )
{
	return pStringTable->FindStringIndex( strVar );
}

const char* NetworkVarNames::Convert( int iIdx )
{
	return pStringTable->GetString( iIdx );
}