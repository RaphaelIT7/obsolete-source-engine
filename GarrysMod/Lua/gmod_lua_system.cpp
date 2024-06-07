#include "cbase.h"
#include "igamesystem.h"
#include "gmod_lua.h"
#include "usermessages.h"
#include "Lua/custom_libs/gl_message.h"

void MsgFunc_BreakModel( bf_read& msg )
{
	// ToDo
}

void MsgFunc_CheapBreakModel( bf_read& msg )
{
	// ToDo
}

void RegisterGModMessages()
{
	RegisterLuaUserMessages();

	usermessages->Register( "LuaCmd", -1 );
	usermessages->Register( "SWEPCmd", -1 );
	usermessages->Register( "AmmoPickup", -1 );
	usermessages->Register( "WeaponPickup", -1 );
	usermessages->Register( "BreakModel", -1 );
	usermessages->Register( "CheapBreakModel", -1 );

#ifdef CLIENT_DLL
	usermessages->HookMessage( "BreakModel", MsgFunc_BreakModel );
	usermessages->HookMessage( "CheapBreakModel", MsgFunc_CheapBreakModel );
#endif
}

#ifdef CLIENT_DLL
class CLuaGameSystem : public CAutoGameSystem
{
	virtual void LevelInitPreEntity()
	{
		Lua::Create();
		Lua::OnLoaded();
	}
};
CLuaGameSystem g_LuaGameSystem;
#endif