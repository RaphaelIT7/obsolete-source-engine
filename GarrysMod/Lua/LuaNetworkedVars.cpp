#include "cbase.h"
#include "LuaNetworkedVars.h"
#include "usermessages.h"

void RecieveNetworkedVarMsg( bf_read &msg )
{

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

void CLuaNetworkedVars::UpdateEntityVars( LuaNetworkedEntity_t& unknown1, CRecipientFilter& unknown2, bool unknown3 )
{
	
}

void CLuaNetworkedVars::UpdateEntityVar( LuaNetworkedEntity_t& unknown1, LuaNetworkedVar_t& unknown2, float unknown3, CRecipientFilter&, bool unknown4 )
{

}

void CLuaNetworkedVars::ClearEntity( EHANDLE& handle )
{

}
#endif

LuaNetworkedVar_t& CLuaNetworkedVars::FindEntityVar( EHANDLE& handle, const char* var, bool unknown )
{

}

void CLuaNetworkedVars::PushNetworkedVar( EHANDLE& handle, const char* var )
{

}

void CLuaNetworkedVars::SetNetworkedVar( EHANDLE& handle, const char* var, GarrysMod::Lua::ILuaObject* obj )
{

}

void CLuaNetworkedVars::SetNetworkedVarProxy( EHANDLE& handle, const char* var, GarrysMod::Lua::ILuaObject* obj )
{

}

#ifndef CLIENT_DLL
void CLuaNetworkedVars::PlayerInsert( CBasePlayer* ent )
{

}
#endif

CLuaObject* CLuaNetworkedVars::BuildNetworkVarTables()
{

}

CLuaObject* CLuaNetworkedVars::BuildEntityNetworkVarTable( CBaseEntity* ent )
{

}