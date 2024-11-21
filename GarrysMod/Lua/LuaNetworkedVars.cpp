#include "cbase.h"
#include "gmod_networkvars.h"
#include "usermessages.h"
#include "Externals.h"

ConVar lua_networkedvar_bytespertick("lua_networkvar_bytespertick", "256");

void RecieveNetworkedVarMsg( bf_read &msg )
{
	if ( !g_Lua || !g_LuaNetworkedVars )
		return;

	long iEHandle = msg.ReadLong(); // WriteEHandle -> WriteLong
	if ( iEHandle == INVALID_NETWORKED_EHANDLE_VALUE )
		return; // Invalid Entity. Ignore it. right?

	EHANDLE pEHandle = EHANDLE(iEHandle);

	char iType = msg.ReadChar();
	char* strVarName = new char[255];
	if (!msg.ReadString(strVarName, 255))
		Warning("RecieveNetworkedVarMsg: This shouldn't happen!\n"); // Unofficial Warning.

	bool bInvalid = false;
	switch( iType )
	{
		//case Type::Nil:
		//	g_Lua->PushNil();
		//	break;
		case Type::Bool:
			g_Lua->PushBool( msg.ReadOneBit() );
			break;
		case Type::Number:
			g_Lua->PushNumber( msg.ReadFloat() );
			break;
		case Type::String:
			{
				char* str = new char[255];
				if ( msg.ReadString( str, 255 ) )
					g_Lua->PushString( str );

				delete[] str;
			}
			break;
		case Type::Entity:
			{
				EHANDLE pEntHandle = EHANDLE( msg.ReadLong() );
				Push_Entity( pEntHandle.Get() );
			}
			break;
		case Type::Vector:
			{
				Vector* vec = new Vector;
				msg.ReadBitVec3Coord( *vec );
				Push_Vector( vec );
			}
			break;
		case Type::Angle:
			{
				QAngle* ang = new QAngle;
				msg.ReadBitAngles( *ang );
				Push_Angle( ang ); // This is shit. Inside Push_Angle we should create a copy of the QAngle and use that instead of expecting everyone to create it and let us manage it.
			}
			break;
		default:
			Msg("Error: Trying to network (receive) unacceptable type (%s)", g_Lua->GetActualTypeName( iType ) );
			return;
	}

	CLuaObject obj; // CLuaObject( -1, -1 );
	obj.SetFromStack( -1 );
	g_LuaNetworkedVars->SetNetworkedVar( pEHandle, strVarName, &obj );
}

CLuaNetworkedVars::CLuaNetworkedVars()
{
	// ToDo
}

void CLuaNetworkedVars::RegisterUserMessages()
{
	usermessages->Register( "NetworkedVar", -1 );

#ifdef CLIENT_DLL
	usermessages->HookMessage( "NetworkedVar", RecieveNetworkedVarMsg );
#endif
}

#ifndef CLIENT_DLL
void CLuaNetworkedVars::Cycle()
{
	
}

void CLuaNetworkedVars::UpdateEntityVars( LuaNetworkedEntity_t& ent, CRecipientFilter& filter, bool unknown3 )
{
	// ToDo
	// Do we call UpdateEntityVar on every NWVar?
}

void CLuaNetworkedVars::UpdateEntityVar( LuaNetworkedEntity_t& ent, LuaNetworkedVar_t& var, float unknown3, CRecipientFilter& filter, bool unknown4 )
{
	// ToDo: There are also some other things done here.

	if ( !var.m_pLuaValue.isNil() )
	{
		// BUG: The if statement above will be the cause of this issue https://github.com/Facepunch/garrysmod-issues/issues/3397
		// We could easily network nil since the second value we write is the value type.
		// 
		// So we would need to remove the if statement above,
		// add a case for nil below where we just break and then also add support for it in the receive function.

		UserMessageBegin( filter, "NetworkedVar" );
			MessageWriteEHandle( ent.m_pHandle.Get() );
			MessageWriteChar( var.m_pLuaValue.GetType() );
			MessageWriteString( NetworkString::Convert( var.m_iNetworkStringID ) ); // NOTE: Why don't we network the ID xd

			switch( var.m_pLuaValue.GetType() )
			{
			//case Type::Nil:
			//	break;
			case Type::Bool:
				MessageWriteBool( var.m_pLuaValue.GetBool() );
				break;
			case Type::Number:
				MessageWriteFloat( var.m_pLuaValue.GetFloat() );
				break;
			case Type::String:
				MessageWriteString( var.m_pLuaValue.GetString() );
				break;
			case Type::Entity:
				{
					CBaseHandle* handle = (CBaseHandle*)var.m_pLuaValue.GetUserData();
					MessageWriteEHandle( ( handle ? gEntList.GetBaseEntity( *handle ) : NULL ) );
				}
				break;
			case Type::Vector:
				MessageWriteVec3Coord( *var.m_pLuaValue.GetVector() );
				break;
			case Type::Angle:
				MessageWriteAngles( *var.m_pLuaValue.GetAngle() );
				break;
			default:
				Msg("Error: Trying to network unacceptable type (%s)", g_Lua->GetActualTypeName(var.m_pLuaValue.GetType()) );
			}
		MessageEnd();
	}

	var.m_flLastUpdate = gpGlobals->curtime;
}

void CLuaNetworkedVars::ClearEntity( EHANDLE& handle )
{

}
#endif

LuaNetworkedVar_t& CLuaNetworkedVars::FindEntityVar( EHANDLE& handle, const char* var, bool unknown )
{
	LuaNetworkedVar_t luavar;
	return luavar;
}

void CLuaNetworkedVars::PushNetworkedVar( EHANDLE& handle, const char* var )
{

}

void CLuaNetworkedVars::SetNetworkedVar( EHANDLE& handle, const char* var, ILuaObject* obj )
{

}

void CLuaNetworkedVars::SetNetworkedVarProxy( EHANDLE& handle, const char* var, ILuaObject* obj )
{

}

#ifndef CLIENT_DLL
void CLuaNetworkedVars::PlayerInsert( CBasePlayer* ent )
{

}
#endif

CLuaObject* CLuaNetworkedVars::BuildNetworkVarTables()
{
	return NULL;
}

CLuaObject* CLuaNetworkedVars::BuildEntityNetworkVarTable( CBaseEntity* ent )
{
	return NULL;
}