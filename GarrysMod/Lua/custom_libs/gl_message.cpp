#include "cbase.h"
#include "gl_message.h"
#include "usermessages.h"
#include "Lua/CLuaClass.h"
#include "Externals.h"
#include "GarrysMod/Lua/Interface.h"
#include "gmod_networkvars.h"
#include "UserMessageManager.h"

void RecieveLuaUserMessage( bf_read& msg )
{
	// ToDo

	// call usermessage.IncomingMessage
}

void RegisterLuaUserMessages()
{
	usermessages->Register( "LuaUserMessage", -1 );

#ifdef CLIENT_DLL
	usermessages->HookMessage( "LuaUserMessage", RecieveLuaUserMessage );
#endif
}

#ifdef GAME_DLL
static bf_write* CurrentMessage = NULL;
LUA_FUNCTION_STATIC( umsg_Start ) // Redo it later on to better match gmod's version.
{
	const char* pName = LUA->CheckString( 1 );
	CRecipientFilter* filter = NULL;
	switch( LUA->GetType( 2 ) )
	{
		case GarrysMod::Lua::Type::Entity:
			{
				CBaseEntity* pEnt = Get_Entity( 1 ); // Use Get_Player later on. Also if we got it, Use it inside the CRecipientFilter class.
				if ( pEnt->IsPlayer() )
				{
					filter = new CRecipientFilter;
					filter->AddRecipient( (CBasePlayer*)pEnt );
					filter->MakeReliable();
				}
				break;
			}
		case GarrysMod::Lua::Type::RecipientFilter:
			{
				CRecipientFilter* pFilter = Get_CRecipientFilter( 1 );
				if ( pFilter )
				{
					filter = new CRecipientFilter;
					filter->CopyFrom( *pFilter );
					filter->MakeReliable();
				} else {
					g_Lua->ErrorFromLua( "umsg.Start: Error! Invalid Recipient Filter!" );
				}
				break;
			}
		default:
			break;
	}
	
	if ( !filter )
	{
		g_Lua->ErrorNoHalt( "umsg.Start: Recipient Filter Is NULL!? - making it BROADCAST" );

		filter = new CRecipientFilter;
		filter->AddAllPlayers();
		filter->MakeReliable();
	}

	if ( CurrentMessage )
	{
		g_Lua->ErrorNoHalt( "umsg.Start: Starting new message without ending the old one!" );
		delete CurrentMessage;
	}

	CurrentMessage = new bf_write;
	CurrentMessage->StartWriting( NULL, 0 );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_End )
{
	if ( !CurrentMessage )
		LUA->ThrowError( "Calling umsg.End without a valid message!" );

	// ToDo: Implement Gmod's CUserMessageManager.
	// g_UserMessageManager->AddToQueue();

	return 0;
}

LUA_FUNCTION_STATIC( umsg_PoolString )
{
	const char* pString = LUA->CheckString( 1 );

	NetworkString::Add( pString ); // It doesn't write it? Lies :<

	return 0;
}

LUA_FUNCTION_STATIC( umsg_String )
{
	CurrentMessage->WriteString( LUA->CheckString( 1 ) );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Short )
{
	CurrentMessage->WriteShort( LUA->CheckNumber( 1 ) );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Long )
{
	CurrentMessage->WriteLong( LUA->CheckNumber( 1 ) );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Bool )
{
	CurrentMessage->WriteOneBit( LUA->GetBool( 1 ) );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Char )
{
	CurrentMessage->WriteChar( LUA->CheckNumber( 1 ) );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Vector )
{
	Vector* pVec = Get_Vector( 1 );
	if ( pVec )
		CurrentMessage->WriteBitVec3Coord( *pVec );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Angle )
{
	QAngle* pAng = Get_Angle( 1 );
	if ( pAng )
		CurrentMessage->WriteBitAngles( *pAng );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_VectorNormal )
{
	Vector* pVec = Get_Vector( 1 );
	if ( pVec )
		CurrentMessage->WriteBitVec3Normal( *pVec );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Float )
{
	CurrentMessage->WriteFloat( LUA->CheckNumber( 1 ) );

	return 0;
}

LUA_FUNCTION_STATIC( umsg_Entity )
{
	CBaseEntity* pEnt = Get_Entity( 1 );
	long iEncodedEHandle;
	if( pEnt ) { // Yoinked from gameinferface.cpp -> MessageWriteEHandle
		EHANDLE hEnt = pEnt;

		int iSerialNum = hEnt.GetSerialNumber() & ( (1 << NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS) - 1 );
		iEncodedEHandle = hEnt.GetEntryIndex() | (iSerialNum << MAX_EDICT_BITS);
	} else {
		iEncodedEHandle = INVALID_NETWORKED_EHANDLE_VALUE;
	}
	
	CurrentMessage->WriteLong( iEncodedEHandle );

	return 0;
}

void umsg_Library()
{
	g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->CreateTable();
			g_Lua->PushCFunction( umsg_Start );
			g_Lua->SetField( -2, "Start" );

			g_Lua->PushCFunction( umsg_End );
			g_Lua->SetField( -2, "End" );

			g_Lua->PushCFunction( umsg_PoolString );
			g_Lua->SetField( -2, "PoolString" );

			g_Lua->PushCFunction( umsg_String );
			g_Lua->SetField( -2, "String" );

			g_Lua->PushCFunction( umsg_Short );
			g_Lua->SetField( -2, "Short" );

			g_Lua->PushCFunction( umsg_Long );
			g_Lua->SetField( -2, "Long" );

			g_Lua->PushCFunction( umsg_Bool );
			g_Lua->SetField( -2, "Bool" );

			g_Lua->PushCFunction( umsg_Char );
			g_Lua->SetField( -2, "Char" );

			g_Lua->PushCFunction( umsg_Vector );
			g_Lua->SetField( -2, "Vector" );

			g_Lua->PushCFunction( umsg_Angle );
			g_Lua->SetField( -2, "Angle" );

			g_Lua->PushCFunction( umsg_VectorNormal );
			g_Lua->SetField( -2, "VectorNormal" );

			g_Lua->PushCFunction( umsg_Float );
			g_Lua->SetField( -2, "Float" );

			g_Lua->PushCFunction( umsg_Entity );
			g_Lua->SetField( -2, "Entity" );
		g_Lua->SetField(-2, "umsg");
	g_Lua->Pop(1);
}

CLuaLibrary umsg_library("umsg", umsg_Library);
#endif