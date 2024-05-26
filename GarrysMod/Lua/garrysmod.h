#pragma once

#include <GarrysMod/Lua/LuaShared.h>
#include <GarrysMod/IGarrysMod.h>
#include <steam/isteamnetworking.h>
#include "player.h"

extern GarrysMod::Lua::ILuaShared* LuaShared();

class CGarrysMod : public IGarrysMod
{
public:
	virtual void MD5String( char* out, const char*, const char*, const char* );
	virtual void PlaySound( const char* sound );
	virtual const char* GetMapName();
	virtual void RunConsoleCommand( const char* cmd );
	virtual void StartVideoScale( int x, int y ) ;
	virtual void EndVideoScale( int x, int y );
	virtual void FireGameEvent( IGameEvent* event );

public:
	CGarrysMod();
	void OnP2PSessionRequest( P2PSessionRequest_t* );
	void OnP2PSessionConnectFail( P2PSessionConnectFail_t* );
	void InitializeMod( CreateInterfaceFn fn );
	void Shutdown();
	void LevelInit( const char *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background );
	const char* GetGameDescription();
	void PostInitialize();
	void EntityRemoved( CBaseEntity* ent );
	void Think();
	void MenuThink();
	bool ShouldCollide( CBaseEntity* ent1, CBaseEntity* ent2 );
};