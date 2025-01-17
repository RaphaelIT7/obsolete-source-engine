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
#include "Lua/gmod_lua.h"
#include "GModMaterialProxyFactory.h"
#include "CLuaClass.h"
#include "sv_autorefresh.h"
#include "stringtable_bits.h"
#include <unordered_map>
#ifdef CLIENT_DLL
#include "networkstringtable_clientdll.h"
#else
#include "networkstringtable_gamedll.h"
#endif
#include "Externals.h"

GarrysMod::Lua::ILuaShared* LuaShared()
{
	return (GarrysMod::Lua::ILuaShared*)get->LuaShared();
}

CGarrysMod garrysmod;
IGarrysMod* igarrysmod = &garrysmod;

CGarrysMod::CGarrysMod()
{
	//SteamAPI_RegisterCallback(); Register p2p stuff
}

#ifdef CLIENT_DLL
void CGarrysMod::OnGameRichPresenceJoinRequested( GameRichPresenceJoinRequested_t* )
{
	// ToDo
}

void CGarrysMod::OnGameOverlayActivated( GameOverlayActivated_t* )
{
	// ToDo
}
#endif

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

void CleanupWipeFolder( const char* folder )
{
	// ToDo
}

void CGarrysMod::InitializeMod( CreateInterfaceFn fn )
{
	// Bootil::Startup()
	// Bootil::Debug::IListener::Add()
	// CLoadTimer::CLoadTimer("CGarrysMod::InitializeMod", false);

	CleanupExploits();

	// ToDo

#ifdef CLIENT_DLL
	GarrysMod::MaterialProxyFactory::Init();
#endif

	//GarrysMod::AutoRefresh::Init();

	CleanupWipeFolder( "downloads/server/" );
}

void CGarrysMod::Shutdown()
{
	CleanupWipeFolder( "downloads/server/" );

#ifdef CLIENT_DLL
	GarrysMod::MaterialProxyFactory::Shutdown();
#endif

	// ToDo
	// Funnies with map.pack
}

#ifdef CLIENT_DLL
void CGarrysMod::FullScreenVGUI( bool unknown )
{
	// ToDo
}

void CGarrysMod::VGUIDrawMode( bool unknown, IMatRenderContext* context )
{
	// ToDo
}

void CGarrysMod::PaintVGUIOverlay( IMatRenderContext* context )
{
	// ToDo
}

void CGarrysMod::RenderView( const CViewSetup& viewSetup, int x, int y )
{
	// ToDo
}
#endif

void CGarrysMod::LevelInit( const char *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background )
{
#ifndef CLIENT_DLL
	Lua::Create();
	Lua::OnLoaded();
#endif
}

#ifdef CLIENT_DLL
void CGarrysMod::LevelShutdown()
{
	// ToDo
	// ResetNetLibrary();

	// Some other stuff

	// CleanupLuaBoundMaterials();
	// CleanupLuaCreatedMaterials();
	// CleanupLuaCreatedRenderTargets();
	// CleanupShaderParams();
}
#else
const char* CGarrysMod::GetGameDescription() // Called from CGMODRules::GetGameDescription()
{
	return "the funnies"; // ToDo
}
#endif

void CGarrysMod::PostInitialize() // Called from CServerGameDLL::DLLInit
{

}

void CGarrysMod::EntityRemoved( CBaseEntity* ent ) // Called from CBaseEntity::UpdateOnRemove
{
	// ToDo
	// Make da lua call
}

#ifdef CLIENT_DLL
void CGarrysMod::OnNewUserCmd()
{
	// ToDo
}
#endif

void CGarrysMod::Think() // Called from CLuaGameSystem::FrameUpdatePreEntityThink
{
	// CheckForFilesystemChanges();
#ifdef GAME_DLL
	g_LuaNetworkedVars->Cycle();
#endif
	// GarrysMod::Lua::Libraries::Timer::Cycle();
	GarrysMod::Lua::Libraries::File::AsyncCycle();
	// GarrysMod::Lua::Libraries::HTTP::Cycle();
	//GarrysMod::AutoRefresh::Cycle();
}

void CGarrysMod::MenuThink()
{
	// ToDo
}

#ifdef CLIENT_DLL
void CGarrysMod::ClearVertexLighting( IMatRenderContext* context, float unknown )
{
	// ToDo
}

void CGarrysMod::EndClearVertexLighting( IMatRenderContext* context )
{
	// ToDo
}
#endif

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
#ifdef CLIENT_DLL
	return engine->GetLevelName();
#else
	return gpGlobals->mapname.ToCStr();
#endif
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

void dumpstringtables_newCmd( const CCommand &args )
{
	if ( args.ArgC() > 0 )
	{
		INetworkStringTable* pTable = networkstringtable->FindTable( args.Arg( 1 ) );
		if ( !pTable )
		{
			Msg( "Table %s not found.\n", args.Arg( 1 ) );
			return;
		}

		for ( int i=0; i<pTable->GetNumStrings(); ++i )
		{
			int iLength;
			const void* pUserData = pTable->GetStringUserData( i, &iLength );
			if (iLength > 0)
				Msg( "%i: '%s'(data %i bytes)\n", i, pTable->GetString( i ), iLength );
			else
				Msg( "%i: '%s'\n", i, pTable->GetString( i ) );
		}

		Msg( "Max Strings: %i\n", pTable->GetMaxStrings() );
		Msg( "Num Strings: %i\n", pTable->GetNumStrings() );
		Msg( "Total String Bytes: %i\n", 0 );  // ToDo: Finish this later.
		Msg( "Total Data Bytes: %i\n", 0 );
		return;
	}

	for ( int i=0; i<networkstringtable->GetNumTables(); ++i )
	{
		INetworkStringTable* pTable = networkstringtable->GetTable( i );
		Msg( "%i: %s\n	strings: %i\n	strdata: %i\n	databts: %i\n", i, pTable->GetTableName(), pTable->GetNumStrings(), 0, pTable->GetEntryBits() ); // ToDo: get strdata properly
	}
}

#ifdef CLIENT_DLL
ConCommand dumpstringtables_new( "cl_dumpstringtables_new", dumpstringtables_newCmd, "", 0 );
#else
ConCommand dumpstringtables_new( "dumpstringtables_new", dumpstringtables_newCmd, "", 0 );
#endif

#ifdef CLIENT_DLL
static std::unordered_map<unsigned short, unsigned short> pNewIndexes;
void PostFullUpdateCallback( INetworkStringTable* modelprecache )
{
	engine->ResetModelPrecache();

	const CEntInfo* pInfo = g_pEntityList->FirstEntInfo();
	for ( ;pInfo; pInfo = pInfo->m_pNext ) {
		CBaseEntity *pEnt = ( CBaseEntity* )pInfo->m_pEntity;

		int index = pEnt->GetModelIndex();
		if ( index == 0 ) // Entity with no model :^
			continue;

		auto it = pNewIndexes.find( index );
		if ( it == pNewIndexes.end() )
		{
			DevMsg( 2, "[OnClearModelPrecache]: Missing model: %i (%s, %s)\n", index, STRING( pEnt->GetModelName() ), modelprecache->GetString( index ) );
			continue;
		}

		pEnt->SetRawModelIndex( it->second ); // Set all the new model indexes without doing anything else
	}

	pNewIndexes.clear();
	modelprecache->SetFullUpdateCallback( NULL );
	//physics->DestroyAllCollisionSets(); // CollisionSets use the model index.... (Unsure if it's safe to just destroy all)
}

void CGarrysMod::OnClearModelPrecache( bf_read* pBF ) // Called before stringtable & modelindex updates are received
{
	int iEntires = pBF->GetNumBitsLeft() / g_nMaxModelIndexBits / 2;

	INetworkStringTable* modelprecache = networkstringtable->FindTable( "modelprecache" );
	for ( int i=0; i<iEntires; ++i )
	{
		int oldIndex = pBF->ReadUBitLong( g_nMaxModelIndexBits );
		int newIndex = pBF->ReadUBitLong( g_nMaxModelIndexBits );
		pNewIndexes[ oldIndex ] = newIndex;
		if ( oldIndex != newIndex )
			DevMsg( 2, "[OnClearModelPrecache]: New Index: %i -> %i (%s)\n", oldIndex, newIndex, modelprecache->GetString( oldIndex ) );
	}

	modelprecache->SetFullUpdateCallback( &PostFullUpdateCallback ); // Wait for the stringtable update to be received, or else sprites will crash since they use the modelindex for rendering.
}
#else
// Idea: Adding a Lua bind like engine.ClearModelPrecache could be useful.
void CGarrysMod::ClearModelPrecache()
{
	std::unordered_map<std::string, unsigned short> pModels;
	std::unordered_map<unsigned short, std::string> pRevModels; // This should only be used for debugging and it has no other purpose than that.
	INetworkStringTable* modelprecache = networkstringtable->FindTable( "modelprecache" ); // Why is MODEL_PRECACHE_TABLENAME defined in precache.h? Maybe we should move it to a file that the game dll can access
	int iWorldBushCount = 0;
	int iNumStrings = modelprecache->GetNumStrings();
	for ( int i=0; i<iNumStrings; ++i)
	{
		const char* pString = modelprecache->GetString( i );
		pModels[ pString ] = i;
		pRevModels[ i ] = pString;

		if (pString[0] == '*')
			iWorldBushCount = i;
	}

	std::string strWorldModel = modelprecache->GetString( 1 );
	modelprecache->DeleteAllStrings( true ); // Yes I exposed it.
	engine->ResetModelPrecache();
	engine->PrecacheModel( "" ); // Engine...
	engine->PrecacheModel( strWorldModel.c_str() ); // Precache world since CWorld::Precache doesn't do this :<

	for ( int i=1; i<iWorldBushCount; ++i ) // NOTE: My engine version also precaches world bushes, but I don't think gmod does this, so this could probably be removed?
	{
		char localmodel[5]; // inline model names "*1", "*2" etc
		Q_snprintf( localmodel, sizeof( localmodel ), "*%i", i );
		engine->PrecacheModel( localmodel );
	}

	const CEntInfo *pInfo = g_pEntityList->FirstEntInfo();
	for ( ;pInfo; pInfo = pInfo->m_pNext ) {
		CBaseEntity *pEnt = (CBaseEntity*)pInfo->m_pEntity;

		// It's a death sentence... Don't call CWorld::Precache if you value your physics environment.
		// It will break physics props and possibly other things since more than just precache stuff is done in that function.
		if (pEnt->IsWorld())
			continue;

		pEnt->Precache(); // Not the best Idea but it should work and solve any potential issues.
	}

	std::unordered_map<unsigned short, unsigned short> pNewIndexes;
	iNumStrings = modelprecache->GetNumStrings();
	for ( int i=0; i<iNumStrings; ++i )
	{
		auto it = pModels.find( modelprecache->GetString(i) );
		if ( it == pModels.end() )
		{
			DevMsg(2, "[ClearModelPrecache] Failed to find new precache index? (%i, %s)\n", i, modelprecache->GetString(i));
			continue;
		}

		pNewIndexes[ it->second ] = i;
		if ( it->second != i )
			DevMsg( 2, "[ClearModelPrecache] New Index: %i -> %i (%s)\n", it->second, i, modelprecache->GetString(i) );
	}

	pInfo = g_pEntityList->FirstEntInfo();
	for ( ;pInfo; pInfo = pInfo->m_pNext ) {
		CBaseEntity *pEnt = ( CBaseEntity* )pInfo->m_pEntity;

		int index = pEnt->GetModelIndex();
		if (index == 0) // Entity with no model :^
			continue;

		auto it = pNewIndexes.find(index);
		if (it == pNewIndexes.end())
		{
			auto it2 = pRevModels.find(index);
			DevMsg( 2, "[ClearModelPrecache] Missing model: %i (%s, %s, %s)\n", index, STRING( pEnt->GetModelName() ), pEnt->GetClassname(), it2 != pRevModels.end() ? it2->second.c_str() : "NULL" );
			continue;
		}

		pEnt->SetRawModelIndex( it->second ); // Setting the new model index
	}

	//physics->DestroyAllCollisionSets(); // CollisionSets use the model index.... (Unsure if it's safe to just destroy all)

	bf_write pBF;
	int size = 1 << (g_nMaxModelIndexBits + 2);
	char* strData = new char[ size ]; // Should always be big enouth in the case that the stringtable is huge.
	pBF.StartWriting( strData, size );
	pBF.WriteUBitLong( 1, 4 ); // Write GMOD Net message type or so... idk

	for ( const auto&[oldIndex, newIndex] : pNewIndexes )
	{
		pBF.WriteUBitLong( oldIndex, g_nMaxModelIndexBits );
		pBF.WriteUBitLong( newIndex, g_nMaxModelIndexBits );
	}

	CRecipientFilter filter;
	filter.AddAllPlayers();
	engine->GMOD_SendToClient( &filter, strData, pBF.GetNumBytesWritten() ); // This will be received before the new ModelIndexes are networked, so we can prepare stuff
}

void ClearModelPrecache( const CCommand &args )
{
	garrysmod.ClearModelPrecache();
}
 
ConCommand sv_clearmodelprecache( "sv_clearmodelprecache", ClearModelPrecache, "", 0 );
#endif