// GMOD_OpenURLInSteam
// GMOD_AskConnect
// CleanupExploits
// CleanupUserTempFiles
// CleanupWipeFolder
// LUA_ReloadLegacyAddons
// CallLuaHook(const std::string&)

// dumpstringtables_new
// stringtabletotals

#include "cbase.h"
#include "garrysmod.h"
#include "Externals.h"
#include "Lua/gmod_lua.h"

GarrysMod::Lua::ILuaShared* LuaShared()
{
	return (GarrysMod::Lua::ILuaShared*)get->LuaShared();
}

CGarrysMod garrysmod;
CGarrysMod* igarrysmod = &garrysmod;

CGarrysMod::CGarrysMod()
{
	//SteamAPI_RegisterCallback(); Register p2p stuff
}

void CGarrysMod::OnP2PSessionRequest( P2PSessionRequest_t* request )
{
	// ToDo
}

void CGarrysMod::OnP2PSessionConnectFail( P2PSessionConnectFail_t* request )
{
	// ToDo
}

void CleanupExploits()
{
	// Not needed.
}

void CGarrysMod::InitializeMod( CreateInterfaceFn fn )
{
	// Bootil::Startup()
	// Bootil::Debug::IListener::Add()
	// CLoadTimer::CLoadTimer("CGarrysMod::InitializeMod", false);

	CleanupExploits();

	// ToDo
}

void CleanupWipeFolder( const char* folder )
{
	// ToDo
}

void CGarrysMod::Shutdown()
{
	CleanupWipeFolder( "downloads/server/" );

	// ToDo
	// Funnies with map.pack
}

void CGarrysMod::LevelInit( const char *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background )
{
	Lua::Create();
	Lua::OnLoaded();
}

const char* CGarrysMod::GetGameDescription() // Called from CGMODRules::GetGameDescription()
{
	return "the funnies"; // ToDo
}

void CGarrysMod::PostInitialize() // Called from CServerGameDLL::DLLInit
{

}

void CGarrysMod::EntityRemoved( CBaseEntity* ent ) // Called from CBaseEntity::UpdateOnRemove
{
	// ToDo
	// Make da lua call
}

void CGarrysMod::Think() // Called from CLuaGameSystem::FrameUpdatePreEntityThink
{
	// ToDo
}

void CGarrysMod::MenuThink()
{
	// ToDo
}

// NOTE: Since we fixed the bug in the Physics engine that broke it entirely if CollisionRulesChanged was never called properly, we don't need any safety
bool CGarrysMod::ShouldCollide( CBaseEntity* ent1, CBaseEntity* ent2 ) // Called from PassServerEntityFilter(IHandleEntity const*,IHandleEntity const*) and CCollisionEvent::ShouldCollide(IPhysicsObject *,IPhysicsObject *,void *,void *)
{
	return true; // ToDo
}

void CGarrysMod::MD5String( char* out, const char*, const char*, const char* )
{
	// ToDo
}

void CGarrysMod::PlaySound( const char* sound )
{
	// ToDo
}

const char* CGarrysMod::GetMapName()
{
	return gpGlobals->mapname.ToCStr();
}

void CGarrysMod::RunConsoleCommand( const char* cmd )
{
	// engine->GMOD_RawConsoleCommand( cmd );
}

void CGarrysMod::StartVideoScale( int x, int y )
{
	// Unused
}

void CGarrysMod::EndVideoScale( int x, int y )
{
	// Unused
}

void CGarrysMod::FireGameEvent( IGameEvent* event )
{
	// if name == client_disconnect
	// GarrysMod::StringTable::Reset()

	// if name == server_spawn
	// Do some print
}