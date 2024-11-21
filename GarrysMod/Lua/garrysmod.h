#pragma once

#include <ILuaShared.h>
#include <GarrysMod/IGarrysMod.h>
#include <steam/isteamnetworking.h>
#ifdef CLIENT_DLL
class CBaseEntity;
#include <iviewrender.h>
#else
#include "player.h"
#endif
#include <steam/isteamfriends.h>

extern ILuaShared* LuaShared();

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
#ifdef CLIENT_DLL
	void OnGameRichPresenceJoinRequested( GameRichPresenceJoinRequested_t* );
	void OnGameOverlayActivated( GameOverlayActivated_t* );
#endif
	void OnP2PSessionRequest( P2PSessionRequest_t* );
	void OnP2PSessionConnectFail( P2PSessionConnectFail_t* );
	void InitializeMod( CreateInterfaceFn fn );
	void Shutdown();
#ifdef CLIENT_DLL
	void FullScreenVGUI( bool unknown );
	void VGUIDrawMode( bool unknown, IMatRenderContext* context );
	void PaintVGUIOverlay( IMatRenderContext* context );
	void RenderView( const CViewSetup& viewSetup, int x, int y ); // Called from CViewRender::Render(vrect_t *)
#endif
	void LevelInit( const char *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background );
#ifdef CLIENT_DLL
	void LevelShutdown();
#else
	const char* GetGameDescription();
#endif
	void PostInitialize();
	void EntityRemoved( CBaseEntity* ent );
#ifdef CLIENT_DLL
	void OnNewUserCmd(); // Called from C_GMOD_Player::CreateMove(float,CUserCmd *)
#endif
	void Think();
	void MenuThink();
#ifdef CLIENT_DLL
	void ClearVertexLighting( IMatRenderContext* context, float unknown ); // Used by SpawniconGenerator::Render(void)
	void EndClearVertexLighting( IMatRenderContext* context ); // Used by SpawniconGenerator::Render(void)
#endif
	bool ShouldCollide( CBaseEntity* ent1, CBaseEntity* ent2 );
};