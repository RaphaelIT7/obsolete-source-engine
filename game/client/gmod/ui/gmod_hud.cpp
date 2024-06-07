#include "cbase.h"
#include "gmod_hud.h"
#include "usermessages.h"
#include "Externals.h"

void MsgFunc_CHudGMod_LuaCmd( bf_read &msg )
{
	///gHUD.FindElement("CHudGMod");

	char* code = new char[255];
	if (!msg.ReadString(code, 255))
	{
		DevWarning("SendLua/BroadcastLua/lua_run_cl failed with code: %s\n", ""); // What is the code?
	} else {
		g_Lua->RunString("LuaCmd", "", code, true, true);
	}

	delete[] code;
}

void MsgFunc_CHudGMod_SWEPCmd( bf_read &msg )
{
	CHudGMod* gmod = (CHudGMod*)gHUD.FindElement("CHudGMod");
	if (gmod)
		gmod->MsgFunc_SWEPCmd( msg );
}

void CHudGMod::MsgFunc_SWEPCmd( bf_read& msg )
{
	// ToDo
}

void MsgFunc_CHudGMod_ItemPickup( bf_read &msg )
{
	// ToDo
}

void MsgFunc_CHudGMod_AmmoPickup( bf_read &msg )
{
	CHudGMod* gmod = (CHudGMod*)gHUD.FindElement("CHudGMod");
	if (gmod)
		gmod->MsgFunc_AmmoPickup( msg );
}

void CHudGMod::MsgFunc_AmmoPickup( bf_read& msg )
{
	// ToDo
}

void MsgFunc_CHudGMod_WeaponPickup( bf_read &msg )
{
	CHudGMod* gmod = (CHudGMod*)gHUD.FindElement("CHudGMod");
	if (gmod)
		gmod->MsgFunc_WeaponPickup( msg );
}

void CHudGMod::MsgFunc_WeaponPickup( bf_read& msg )
{
	// ToDo
}

DECLARE_HUDELEMENT( CHudGMod );

CHudGMod::CHudGMod( const char* pElementName ) : BaseClass( pElementName )
{
	// ToDo
}

void CHudGMod::Init()
{
	usermessages->HookMessage("LuaCmd", MsgFunc_CHudGMod_LuaCmd);
	usermessages->HookMessage("SWEPCmd", MsgFunc_CHudGMod_SWEPCmd);
	usermessages->HookMessage("ItemPickup", MsgFunc_CHudGMod_ItemPickup);
	usermessages->HookMessage("AmmoPickup", MsgFunc_CHudGMod_AmmoPickup);
	usermessages->HookMessage("WeaponPickup", MsgFunc_CHudGMod_WeaponPickup);

	// ToDo
}

void CHudGMod::VidInit()
{
	// ToDo
}

void CHudGMod::Reset()
{
	// ToDo
}

void CHudGMod::OnThink()
{
	// ToDO
}

void CHudGMod::Paint()
{
	// ToDO
	// Call Gamemode hook. (Probably HUDPaint)
}

void CHudGMod::PaintBackground()
{
	// ToDo
	// Call Gamemode hook. (Probably HUDPaintBackground)
}