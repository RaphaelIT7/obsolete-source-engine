// Lua::Create
// CServerGameDLL::LevelShutdown -> Lua::Kill
// Lua::OnLoaded -> GarrysMod::Ammo::Refresh()

#include "cbase.h"
#include "gmod_lua.h"
#include "garrysmod.h"
#include "GarrysMod/Lua/LuaInterface.h"
#include "CLuaManager.h"
#include "Externals.h"
#include "GarrysMod/Lua/LuaObject.h"
#include "Lua/CLuaClass.h"

void CLuaManager::Startup() // ToDo: use definitions late for Client / Server stuff
{
	g_LuaNetworkedVars = new CLuaNetworkedVars();

	LuaShared()->MountLua(LUA_PATH);

	g_Lua = LuaShared()->GetLuaInterface(LUA_STATE);
	g_Lua->Init(g_LuaCallback, false);
	g_Lua->SetPathID(LUA_PATH);

	GarrysMod::Lua::ILuaObject* global = g_Lua->Global();
	g_Lua->PushNumber(get->Version());
	g_Lua->SetMember(global, "VERSION");

	g_Lua->PushString(get->VersionStr());
	g_Lua->SetMember(global, "VERSIONSTR");

	g_Lua->PushString("unknown");
	g_Lua->SetMember(global, "BRANCH");

	g_Lua->PushBool(LUA_CLIENT);
	g_Lua->SetMember(global, "CLIENT");

	g_Lua->PushBool(LUA_SERVER);
	g_Lua->SetMember(global, "SERVER");

	InitLuaClasses(g_Lua);
	InitLuaLibraries(g_Lua);

	g_Lua->FindAndRunScript("includes/init.lua", true, true, "", true);

	// ToDo
}

void Lua::Create()
{
	Kill();

	LuaShared()->CreateLuaInterface(LUA_STATE, true);

	// new CLuaSWEPManager();
	// new CLuaSENTManager();
	// new CLuaEffectManager();
	gGM = new CLuaGamemode();
	CLuaManager::Startup();
	// CLuaGamemode::LoadCurrentlyActiveGamemode();
	// DataPack()->BuildSearchPaths();
}

void Lua::Kill()
{
	// ShutdownLuaClasses( g_Lua );
	// Error( "!g_LuaNetworkedVars" )
	LuaShared()->UnMountLua(LUA_PATH);
	if (gGM)
		delete gGM;

	gGM = NULL;
	// GarrysMod::Lua::Libraries::Timer::Shutdown();
}

void Lua::OnLoaded()
{
	// GarrysMod::Ammo::Refresh();
}

#ifndef CLIENT_DLL
void lua_filestats_command( const CCommand &args )
{
	UTIL_IsCommandIssuedByServerAdmin();
	LuaShared()->DumpStats();
}
ConCommand lua_filestats( "lua_filestats", lua_filestats_command, "Lua File Stats", 0);

void CC_OpenScript( const CCommand &args )
{
	// IsGModAdmin( true );

	Msg("Running script %s...\n", args.ArgS());
}
ConCommand lua_openscript( "lua_openscript", CC_OpenScript, "Open a Lua script", 0);

void CC_LuaRun( const CCommand &args )
{
	// IsGModAdmin( true );

	Msg("> %s...\n", args.ArgS());
 
	auto interface = LuaShared()->GetLuaInterface(GarrysMod::Lua::State::SERVER);
	if (!interface) {
		Warning("Failed to find Server ILuaInterface!");
	} else {
		interface->RunString("lua_run", "", args.ArgS(), true, true);
	}
}
ConCommand lua_run( "lua_run", CC_LuaRun, "Run a Lua command", 0);

void CC_LuaRun_cl( const CCommand &args )
{
	// Check sv_allowcslua

	CBasePlayer* player = UTIL_GetCommandClient();
	CRecipientFilter filter;
	filter.AddRecipient(player);

	UserMessageBegin(filter, "LuaCmd");
		MessageWriteString(args.ArgS());
	MessageEnd();
}
ConCommand lua_run_cl( "lua_run_cl", CC_LuaRun_cl, "", 0);
#endif