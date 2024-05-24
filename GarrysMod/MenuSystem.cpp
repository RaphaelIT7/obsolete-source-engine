#include <GarrysMod/IMenuSystem.h>
#include "Externals.h"
#include "tier3/tier3.h"
#include "steam/steam_api.h"

class CMenuSystem : public IMenuSystem
{
public:
	virtual ~CMenuSystem() {};
	virtual int Init( CreateInterfaceFn, IGet*, IGarrysMod*, CGlobalVarsBase* );
	virtual void Shutdown();
	virtual void SetupNetworkString( INetworkStringTableContainer* );
	virtual void Think();
	virtual void StartLua();
	virtual void ServerDetails( const char*, const char*, const char*,int, const char* );
	virtual void OnLuaError( CLuaError*, IAddonSystem::Information* );
	virtual void SendProblemToMenu( const char* id, int severity, const char* params );
	virtual bool IsServerBlacklisted( const char* address, const char* hostname, const char* description, const char* gamemode, const char* map );
};

IGet* get = nullptr;
IGarrysMod* igarrysmod = nullptr;
CGlobalVarsBase* gpGlobals = nullptr;
CSteamAPIContext g_SteamAPIContext;
INetworkStringTableContainer* networkstringtable = nullptr;
int CMenuSystem::Init( CreateInterfaceFn fn, IGet* gt, IGarrysMod* igmod, CGlobalVarsBase* globals )
{
	get = gt;
	igarrysmod = igmod;
	gpGlobals = globals;

	ConnectTier1Libraries( &fn, 1 );
	ConnectTier2Libraries( &fn, 1 );
	ConnectTier3Libraries( &fn, 1 );

	SteamAPI_Init();
	SteamAPI_SetTryCatchCallbacks( false );
	if (!g_SteamAPIContext.Init())
	{
		Error("No Steam API. Try restarting Steam.");
	}

	// ToDo: Get all the other interfaces

	ConVar_Register(0, 0);

	return 1;
}

void CMenuSystem::Shutdown()
{
	// ToDo: Shutdown lua

	DisconnectTier3Libraries();
	DisconnectTier2Libraries();

	ConVar_Unregister();

	DisconnectTier1Libraries();
}

void CMenuSystem::SetupNetworkString(INetworkStringTableContainer* container)
{
	networkstringtable = container;
}

void CMenuSystem::StartLua()
{
	// ToDO
}

void CMenuSystem::Think()
{
	// ToDo

	// Cycle Timer

	// Call Think hook. If Call function doesn't exist, return.

	// Check for Filesystem changes

	// Get DownloadUGCManager and call Think
}

void CMenuSystem::ServerDetails( const char*, const char*, const char*, int, const char* )
{
	// ToDo
}

void CMenuSystem::OnLuaError( CLuaError* error, IAddonSystem::Information* addon )
{
	// ToDo
}

void CMenuSystem::SendProblemToMenu( const char* id, int severity, const char* params )
{
	// ToDo
}

bool CMenuSystem::IsServerBlacklisted( const char* address, const char* hostname, const char* description, const char* gamemode, const char* map )
{
	// ToDo

	// Call IsServerBlacklisted

	// Check if return is nil

	return false;
}