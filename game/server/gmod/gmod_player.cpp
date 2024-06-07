//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Player for HL2.
//
//=============================================================================//

#include "cbase.h"
#include "weapon_gmodbasehlmpcombatweapon.h"
#include "gmod_player.h"
#include "globalstate.h"
#include "game.h"
#include "gamerules.h"
#include "predicted_viewmodel.h"
#include "in_buttons.h"
#include "gmod_gamerules.h"
#include "KeyValues.h"
#include "team.h"
#include "weapon_gmodbase.h"
#include "grenade_satchel.h"
#include "eventqueue.h"
#include "gamestats.h"

#include "engine/IEngineSound.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"

#include "ilagcompensationmanager.h"

int g_iLastCitizenModel = 0;
int g_iLastCombineModel = 0;

CBaseEntity	 *g_pLastCombineSpawn = NULL;
CBaseEntity	 *g_pLastRebelSpawn = NULL;
extern CBaseEntity				*g_pLastSpawn;

#define HL2MP_COMMAND_MAX_RATE 0.3

void DropPrimedFragGrenade( CGMOD_Player *pPlayer, CBaseCombatWeapon *pGrenade );

LINK_ENTITY_TO_CLASS( player, CGMOD_Player );

LINK_ENTITY_TO_CLASS( info_player_combine, CPointEntity );
LINK_ENTITY_TO_CLASS( info_player_rebel, CPointEntity );

IMPLEMENT_SERVERCLASS_ST(CGMOD_Player, DT_GMOD_Player)
	
END_SEND_TABLE()

BEGIN_DATADESC( CGMOD_Player )
END_DATADESC()

#define MAX_COMBINE_MODELS 4
#define MODEL_CHANGE_INTERVAL 5.0f
#define TEAM_CHANGE_INTERVAL 5.0f

#define HL2MPPLAYER_PHYSDAMAGE_SCALE 4.0f

#pragma warning( disable : 4355 )

CGMOD_Player::CGMOD_Player() : BaseClass()
{

}

CGMOD_Player::~CGMOD_Player( void )
{

}