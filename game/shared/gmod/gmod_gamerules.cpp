//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "gmod_gamerules.h"
#include "viewport_panel_names.h"
#include "gameeventdefs.h"
#include <KeyValues.h>
#include "ammodef.h"
#include "Externals.h"

#ifdef CLIENT_DLL
	#include "c_gmod_player.h"
#else

	#include "eventqueue.h"
	#include "player.h"
	#include "gamerules.h"
	#include "game.h"
	#include "items.h"
	#include "entitylist.h"
	#include "mapentities.h"
	#include "in_buttons.h"
	#include <ctype.h>
	#include "voice_gamemgr.h"
	#include "iscorer.h"
	#include "weapon_gmodbasehlmpcombatweapon.h"
	#include "team.h"
	#include "voice_gamemgr.h"
	#include "gmod_gameinterface.h"
	#include "gmod_cvars.h"
	#include <gmod/gmod_bot_temp.h>
	#include "gmod_player.h"

extern void respawn(CBaseEntity *pEdict, bool fCopyCorpse);

extern bool FindInList( const char **pStrings, const char *pToFind );

extern ConVar mp_chattime;

extern CBaseEntity	 *g_pLastCombineSpawn;
extern CBaseEntity	 *g_pLastRebelSpawn;

#define WEAPON_MAX_DISTANCE_FROM_SPAWN 64

#endif


REGISTER_GAMERULES_CLASS( CGMODRules );

BEGIN_NETWORK_TABLE_NOBASE( CGMODRules, DT_GMODRules )

	//#ifdef CLIENT_DLL
	//	RecvPropBool( RECVINFO( m_bTeamPlayEnabled ) ),
	//#else
	//	SendPropBool( SENDINFO( m_bTeamPlayEnabled ) ),
	//#endif

END_NETWORK_TABLE()


LINK_ENTITY_TO_CLASS( gmod_gamerules, CGMODGameRulesProxy );
IMPLEMENT_NETWORKCLASS_ALIASED( GMODGameRulesProxy, DT_GMODGameRulesProxy )

static const char *s_PreserveEnts[] =
{
	"ai_network",
	"ai_hint",
	"hl2mp_gamerules",
	"team_manager",
	"player_manager",
	"env_soundscape",
	"env_soundscape_proxy",
	"env_soundscape_triggerable",
	"env_sun",
	"env_wind",
	"env_fog_controller",
	"func_brush",
	"func_wall",
	"func_buyzone",
	"func_illusionary",
	"infodecal",
	"info_projecteddecal",
	"info_node",
	"info_target",
	"info_node_hint",
	"info_player_deathmatch",
	"info_player_combine",
	"info_player_rebel",
	"info_map_parameters",
	"keyframe_rope",
	"move_rope",
	"info_ladder",
	"player",
	"point_viewcontrol",
	"scene_manager",
	"shadow_control",
	"sky_camera",
	"soundent",
	"trigger_soundscape",
	"viewmodel",
	"predicted_viewmodel",
	"worldspawn",
	"point_devshot_camera",
	"", // END Marker
};



#ifdef CLIENT_DLL
	void RecvProxy_GMODRules( const RecvProp *pProp, void **pOut, void *pData, int objectID )
	{
		CGMODRules *pRules = GMODRules();
		Assert( pRules );
		*pOut = pRules;
	}

	BEGIN_RECV_TABLE( CGMODGameRulesProxy, DT_GMODGameRulesProxy )
		RecvPropDataTable( "gmod_gamerules_data", 0, 0, &REFERENCE_RECV_TABLE( DT_GMODRules ), RecvProxy_GMODRules )
	END_RECV_TABLE()
#else
	void* SendProxy_GMODRules( const SendProp *pProp, const void *pStructBase, const void *pData, CSendProxyRecipients *pRecipients, int objectID )
	{
		CGMODRules *pRules = GMODRules();
		Assert( pRules );
		return pRules;
	}

	BEGIN_SEND_TABLE( CGMODGameRulesProxy, DT_GMODGameRulesProxy )
		SendPropDataTable( "gmod_gamerules_data", 0, &REFERENCE_SEND_TABLE( DT_GMODRules ), SendProxy_GMODRules )
	END_SEND_TABLE()
#endif

#ifndef CLIENT_DLL

	/*class CVoiceGameMgrHelper : public IVoiceGameMgrHelper
	{
	public:
		virtual bool		CanPlayerHearPlayer( CBasePlayer *pListener, CBasePlayer *pTalker, bool &bProximity )
		{
			return ( pListener->GetTeamNumber() == pTalker->GetTeamNumber() );
		}
	};
	CVoiceGameMgrHelper g_VoiceGameMgrHelper;
	IVoiceGameMgrHelper *g_pVoiceGameMgrHelper = &g_VoiceGameMgrHelper;*/

#endif

CGMODRules::CGMODRules()
{
#ifndef CLIENT_DLL
	
#endif
}
	
CGMODRules::~CGMODRules( void )
{
#ifndef CLIENT_DLL
	// Note, don't delete each team since they are in the gEntList and will 
	// automatically be deleted from there, instead.
	g_Teams.Purge();
#endif
}

void CGMODRules::LevelInitPreEntity()
{
	// Episodic Behavior
	// ToDo
}

bool CGMODRules::ShouldCollide( int idx1, int idx2 )
{
	// ToDo
	return true;
}

const unsigned char* CGMODRules::GetEncryptionKey()
{
	return "x9Ke0BY7";
}

void CGMODRules::Precache()
{
	BaseClass::Precache();
}

void CGMODRules::Think()
{
	CGameRules::Think();

	// ToDo
}

void CGMODRules::OnSkillLevelChanged( int level )
{
	// ToDo
}

bool CGMODRules::IsTeamplay()
{
	return false;
}

const char* CGMODRules::GetGameDescription()
{
	return igarrysmod->GetGameDescription();
}

void CGMODRules::ClientDisconnected( edict_t* edict )
{
	// Call Gamemode hook (Probably PlayerDisconnect)

	// Call FireTargets

	// Call CBasePlayer::DestroyViewModels
}

float CGMODRules::FlPlayerFallDamage( CBasePlayer *pPlayer )
{
	// Call gamemode hook (Probably GM:GetFallDamage)
	return 10.0f;
}

bool CGMODRules::FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker )
{
	// Call gamemode hook (Probably GM:PlayerShouldTakeDamage)
	return true;
}

bool CGMODRules::ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target )
{
	return false;
}

bool CGMODRules::FlPlayerFallDeathDoesScreenFade( CBasePlayer *pl )
{
	return false;
}

bool CGMODRules::ClientCommand( CBaseEntity *pEdict, const CCommand &args )
{
	return BaseClass::ClientCommand( pEdict, args );
}

void CGMODRules::ClientSettingsChanged( CBasePlayer *pPlayer )
{
	CGameRules::ClientSettingsChanged( pPlayer );
}

void CGMODRules::DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info )
{
	// Call gamemode hook. (Maybe GM:PlayerDeath?)
}

bool CGMODRules::CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem )
{
	// what is "weaponstay" used for?
	// ToDo: Verify this entire function.

	return !pPlayer->Weapon_OwnsThisType( pItem->GetClassname(), pItem->GetSubType() );
}

bool CGMODRules::CanHaveItem( CBasePlayer *pPlayer, CItem *pItem )
{
	// Call gamemode hook. (Probably GM:PlayerCanPickupItem)
	return true;
}

int CGMODRules::ItemShouldRespawn( CItem *pItem )
{
	return 6;
}

void CGMODRules::InitDefaultAIRelationships()
{
	// Holy jesus.
	// ToDo (A big function.)
}

const char* CGMODRules::AIClassText( int classType )
{
	// ToDo
	// "MISSING CLASS in ClassifyText()" does it really return this or am I dumb?

	return "CLASS_NONE";
}

void CGMODRules::CreateStandardEntities()
{
	CGameRules::CreateStandardEntities();

	CBaseEntity::Create( "gmod_gamerules", vec3_origin, vec3_angle );
}

bool CGMODRules::ShouldBurningPropsEmitLight()
{
	return true;
}

void CGMODRules::CleanUpMap( CLuaClass& obj )
{
	// ToDo
	// Set g_bCleaningUpMap to true

	// Loop thru all Entities

		// Check weapon Owner
		// Check if the class is in the Lua filter
		// Delete if everything says so "UTIL_Remove"

	// Loop end

	// Set g_bCleaningUpMap to false

	// CGlobalEntityList::CleanupDeleteList

	// g_EventQueue::Clear()

	// CAI_DynamicLink::InitDynamicLinks
}