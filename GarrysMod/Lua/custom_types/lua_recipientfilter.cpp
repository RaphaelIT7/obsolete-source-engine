#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "iserver.h"
#include <team.h>

LUA_FUNCTION_STATIC( RecipientFilter__gc )
{
	if ( !LUA->IsType( 1, Type::RecipientFilter ) )
		return 0;

	// ToDo
	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter__tostring )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	char szBuf[64] = {};
	V_snprintf( szBuf, sizeof(szBuf), "RecipientFilter []", filter != nullptr ? std::to_string( filter->GetRecipientCount() ).c_str() : "NULL" );
	LUA->PushString( szBuf );
	return 1;
}

LUA_FUNCTION_STATIC( RecipientFilter_AddAllPlayers )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	filter->AddAllPlayers();

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_AddPAS )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	Vector* origin = Get_Vector( 2 );
	if ( filter == nullptr )
		LUA->ArgError(1, "Tried to use a NULL CRecipientFilter!");

	filter->AddRecipientsByPAS( *origin ); // ToDo: Do we need a safety check for origin?

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_AddPVS )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	Vector* origin = Get_Vector( 2 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	filter->AddRecipientsByPVS( *origin );

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_AddPlayer )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	CBaseEntity* pEnt = Get_Entity( 2 );
	if ( pEnt == NULL )
		LUA->ArgError( 1, "Tried to use a NULL Entity!" );

	if ( !pEnt->IsPlayer() )
		LUA->ArgError( 1, "Tried to use a non player Entity!" ); // Unofficial Error

	filter->AddRecipient( (CBasePlayer*)pEnt );

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_AddPlayers )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	switch ( LUA->GetType(2) )
	{
		case Type::RecipientFilter:
			{
				CRecipientFilter* pFilter2 = Get_CRecipientFilter( 2 );
				if ( !pFilter2 )
					LUA->ArgError( 2, "Tried to use a NULL CRecipientFilter!" );

				for (int i=0; i<gpGlobals->maxClients; ++i)
				{
					int idx = pFilter2->GetRecipientIndex( i );
					if ( idx != -1 )
						filter->AddRecipient( UTIL_PlayerByIndex( i ) ); // GMPlayerByIndex -> UTIL_PlayerByIndex
				}

				break;
			}
		case Type::Table:
			{
				LUA->Push( 2 );
				LUA->PushNumber( 1 );
				LUA->RawGet( -1 );
				int i = 1;
				while ( !LUA->IsType( -1, Type::Nil ) )
				{
					CBaseEntity* pEnt = Get_Entity( -1 );
					if ( pEnt->IsPlayer() )
						filter->AddRecipient( (CBasePlayer*)pEnt );

					LUA->Pop( 1 );
					++i;
					LUA->PushNumber( i );
					LUA->RawGet( -1 );
				}
				break;
			}
		default:
			break;
	}

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_AddRecipientsByTeam )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	double teamid = LUA->GetNumber( 2 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	// TODO
	// Gmod loops over all players and checks their team.
	filter->AddRecipientsByTeam( GetGlobalTeam( teamid ) );

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_GetCount )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	LUA->PushNumber( filter->GetRecipientCount() );

	return 1;
}

LUA_FUNCTION_STATIC( RecipientFilter_GetPlayers )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	LUA->CreateTable();
		int idx = 0;
		for (int i=0; i<gpGlobals->maxClients; ++i)
		{
			if ( filter->GetRecipientIndex( i ) != -1 )
			{
				++i;
				LUA->PushNumber( i );
				Push_Entity( UTIL_PlayerByIndex( i ) );
				LUA->RawSet( -3 );
			}
		}

	return 1;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemoveAllPlayers )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	filter->RemoveAllRecipients();

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemoveMismatchedPlayers )
{
	CRecipientFilter* filter1 = Get_CRecipientFilter( 1 );
	if ( filter1 == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	CRecipientFilter* filter2 = Get_CRecipientFilter( 2 );
	if ( filter2 == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	for (int i=0; i<gpGlobals->maxClients; ++i)
	{
		int idx = filter1->GetRecipientIndex( i );
		if ( idx != -1 && filter2->GetRecipientIndex( i ) == -1 )
			filter1->RemoveRecipientByPlayerIndex( idx );
	}

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemovePAS )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	Vector* origin = Get_Vector( 2 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	filter->GMOD_RemoveRecipientsByPAS( *origin ); // ToDo: Do we need a safety check for origin?

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemovePVS )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	Vector* origin = Get_Vector( 2 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	filter->RemoveRecipientsByPVS( *origin ); // ToDo: Do we need a safety check for origin?

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemovePlayer )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	CBaseEntity* pEnt = Get_Entity( 2 );
	if ( pEnt->IsPlayer() )
		filter->RemoveRecipient( (CBasePlayer*)pEnt );

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemovePlayers )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	switch ( LUA->GetType(2) )
	{
		case Type::RecipientFilter:
			{
				CRecipientFilter* pFilter2 = Get_CRecipientFilter( 2 );
				if ( !pFilter2 )
					LUA->ArgError( 2, "Tried to use a NULL CRecipientFilter!" );

				for (int i=0; i<gpGlobals->maxClients; ++i)
				{
					int idx = pFilter2->GetRecipientIndex( i );
					if ( idx != -1 )
						filter->RemoveRecipientByPlayerIndex( i );
				}

				break;
			}
		case Type::Table:
			{
				LUA->Push( 2 );
				LUA->PushNumber( 1 );
				LUA->RawGet( -1 );
				int i = 1;
				while ( !LUA->IsType( -1, Type::Nil ) )
				{
					CBaseEntity* pEnt = Get_Entity( -1 );
					if ( pEnt->IsPlayer() )
						filter->RemoveRecipient( (CBasePlayer*)pEnt );

					LUA->Pop( 1 );
					++i;
					LUA->PushNumber( i );
					LUA->RawGet( -1 );
				}
				break;
			}
		default:
			break;
	}

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemoveRecipientsByTeam )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	int teamnum = LUA->CheckNumber( 2 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	filter->RemoveRecipientsByTeam( GetGlobalTeam( teamnum ) );

	return 0;
}

LUA_FUNCTION_STATIC( RecipientFilter_RemoveRecipientsNotOnTeam )
{
	CRecipientFilter* filter = Get_CRecipientFilter( 1 );
	int teamnum = LUA->CheckNumber( 2 );
	if ( filter == nullptr )
		LUA->ArgError( 1, "Tried to use a NULL CRecipientFilter!" );

	filter->RemoveRecipientsNotOnTeam( GetGlobalTeam( teamnum ) );

	return 0;
}

LUA_FUNCTION_STATIC( Global_RecipientFilter )
{
	bool bUnreliable = LUA->GetBool( 1 );
	CRecipientFilter* filter = new CRecipientFilter;
	
	if ( !bUnreliable )
		filter->MakeReliable();

	Push_CRecipientFilter( filter );

	return 1;
}

extern CLuaClass recipientfilter_class;
CRecipientFilter* Get_CRecipientFilter( int index )
{
	void* udata = recipientfilter_class.Get( index );
	if ( !udata )
		return NULL;

	return ( CRecipientFilter* )udata;
}

void Push_CRecipientFilter( CRecipientFilter* filter )
{
	recipientfilter_class.Push( filter );
}

void RecipientFilter_Class()
{
	g_Lua->PushSpecial( SPECIAL_GLOB );
		g_Lua->PushCFunction( Global_RecipientFilter );
		g_Lua->SetField( -2, "RecipientFilter" );
	g_Lua->Pop( 1 );

	g_Lua->CreateMetaTableType( "CRecipientFilter", Type::RecipientFilter );
		g_Lua->PushCFunction( RecipientFilter__gc );
		g_Lua->SetField( -2, "__gc" );
		
		g_Lua->PushCFunction(RecipientFilter__tostring );
		g_Lua->SetField( -2, "__tostring" );

		g_Lua->PushCFunction( RecipientFilter_AddAllPlayers );
		g_Lua->SetField( -2, "AddAllPlayers" );

		g_Lua->PushCFunction( RecipientFilter_AddPAS );
		g_Lua->SetField( -2, "AddPAS" );

		g_Lua->PushCFunction( RecipientFilter_AddPVS );
		g_Lua->SetField( -2, "AddPVS" );

		g_Lua->PushCFunction( RecipientFilter_AddPlayer );
		g_Lua->SetField( -2, "AddPlayer" );

		g_Lua->PushCFunction( RecipientFilter_AddPlayers );
		g_Lua->SetField( -2, "AddPlayers" );

		g_Lua->PushCFunction( RecipientFilter_AddRecipientsByTeam );
		g_Lua->SetField( -2, "AddRecipientsByTeam" );

		g_Lua->PushCFunction( RecipientFilter_GetCount );
		g_Lua->SetField( -2, "GetCount" );

		g_Lua->PushCFunction( RecipientFilter_GetPlayers );
		g_Lua->SetField( -2, "GetPlayers" );

		g_Lua->PushCFunction( RecipientFilter_RemoveAllPlayers );
		g_Lua->SetField( -2, "RemoveAllPlayers" );

		g_Lua->PushCFunction( RecipientFilter_RemoveMismatchedPlayers );
		g_Lua->SetField( -2, "RemoveMismatchedPlayers" );

		g_Lua->PushCFunction( RecipientFilter_RemovePAS );
		g_Lua->SetField( -2, "RemovePAS" );

		g_Lua->PushCFunction( RecipientFilter_RemovePVS );
		g_Lua->SetField( -2, "RemovePVS" );

		g_Lua->PushCFunction( RecipientFilter_RemovePlayer );
		g_Lua->SetField( -2, "RemovePlayer" );

		g_Lua->PushCFunction( RecipientFilter_RemovePlayers );
		g_Lua->SetField( -2, "RemovePlayers" );

		g_Lua->PushCFunction( RecipientFilter_RemoveRecipientsByTeam );
		g_Lua->SetField( -2, "RemoveRecipientsByTeam" );

		g_Lua->PushCFunction( RecipientFilter_RemoveRecipientsNotOnTeam );
		g_Lua->SetField( -2, "RemoveRecipientsNotOnTeam" );
}

CLuaClass recipientfilter_class( "CRecipientFilter", Type::RecipientFilter, RecipientFilter_Class );