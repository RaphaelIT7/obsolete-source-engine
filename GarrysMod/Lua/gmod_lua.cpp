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
	// new CLuaNetworkedVars()

#ifdef CLIENT_DLL
	g_Lua = LuaShared()->GetLuaInterface(GarrysMod::Lua::State::CLIENT);
#else
	g_Lua = LuaShared()->GetLuaInterface(GarrysMod::Lua::State::SERVER);
#endif
	g_Lua->Init(g_LuaCallback, false);
#ifdef CLIENT_DLL
	g_Lua->SetPathID("lsc");
#else
	g_Lua->SetPathID("lsv");
#endif

	GarrysMod::Lua::ILuaObject* global = g_Lua->Global();
	g_Lua->PushNumber(get->Version());
	g_Lua->SetMember(global, "VERSION");

	g_Lua->PushString(get->VersionStr());
	g_Lua->SetMember(global, "VERSIONSTR");

	g_Lua->PushString("unknown");
	g_Lua->SetMember(global, "BRANCH");

#ifdef CLIENT_DLL
	g_Lua->PushBool(true);
	g_Lua->SetMember(global, "CLIENT");

	g_Lua->PushBool(false);
	g_Lua->SetMember(global, "SERVER");
#else
	g_Lua->PushBool(false);
	g_Lua->SetMember(global, "CLIENT");

	g_Lua->PushBool(true);
	g_Lua->SetMember(global, "SERVER");
#endif

	InitLuaClasses(g_Lua);
	InitLuaLibraries(g_Lua);

	g_Lua->FindAndRunScript("includes/init.lua", true, true, "", true);

	// ToDo
}

void Lua::Create()
{
	Kill();

#ifdef CLIENT_DLL
	LuaShared()->CreateLuaInterface(GarrysMod::Lua::State::CLIENT, true);
#else
	LuaShared()->CreateLuaInterface(GarrysMod::Lua::State::SERVER, true);
#endif

	// new CLuaSWEPManager();
	// new CLuaSENTManager();
	// new CLuaEffectManager();
	// new CLuaGamemode();
	CLuaManager::Startup();
	// CLuaGamemode::LoadCurrentlyActiveGamemode();
	// DataPack()->BuildSearchPaths();
}

void Lua::Kill()
{
	// ShutdownLuaClasses( g_Lua );
	// Error( "!g_LuaNetworkedVars" )
	// delete gGM;
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