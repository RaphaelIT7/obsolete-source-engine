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
#include "Externals.h"
#include "vprof.h"

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

bool CGMOD_Player::ShouldCollide( int collisionGroup0, int collisionGroup1 )
{
	// ToDo
	return false;
}

int CGMOD_Player::ShouldTransmit( const CCheckTransmitInfo* pInfo )
{
	// ToDo
	// Does Gmod do the Player updates here?
	return FL_EDICT_PVSCHECK;
}

void CGMOD_Player::Spawn()
{
	// ToDo

	CHL2MP_Player::Spawn();

	// ToDo

	// Make Gamemode call. (Probably: GM:PlayerSpawn)

	// ToDo
}

void CGMOD_Player::Precache()
{
	//CHL2_Player::Precache();
	CHL2MP_Player::Precache(); // Gmod uses the one above.
	PrecacheModel( "sprites/glow01.vmt" );
	PrecacheModel( "model/player.mdl" );
}

void CGMOD_Player::TraceAttack( const CTakeDamageInfo& info, const Vector& vecDir, CGameTrace* tr, CDmgAccumulator* pAccumulator )
{
	// Make gamemode call. (Probably: GM:PlayerTraceAttack)

	// PushTableFromTrace

	BaseClass::TraceAttack( info, vecDir, tr, pAccumulator ); // Use this for now.
}

int CGMOD_Player::OnTakeDamage( const CTakeDamageInfo& info )
{
	return CHL2MP_Player::OnTakeDamage( info );
}

void CGMOD_Player::Event_Killed( const CTakeDamageInfo& info )
{
	// ToDo

	// Make a gamemode call. (Unknown)

	// ToDo

	CHL2_Player::Event_Killed( info );

	// ToDo

	// Make a gamemode call. (Unknown)
}

void CGMOD_Player::ChangeTeam( int iTeamNum )
{
	// ToDo
}

void CGMOD_Player::ViewPunch( const QAngle& angleOffset )
{
	CBasePlayer::ViewPunch( angleOffset );
}

void CGMOD_Player::FireBullets( const FireBulletsInfo_t& info )
{
	// ToDo

	// Do some check with gpGlobals and make a dynamic cast to something

	CHL2MP_Player::FireBullets( info );
}

void CGMOD_Player::DoImpactEffect( CGameTrace& tr, int nDamageType )
{
	CBaseCombatWeapon *pWeapon = GetActiveWeapon();
	if ( pWeapon )
	{
		pWeapon->DoImpactEffect( tr, nDamageType );
	} else
	{
		CBasePlayer::DoImpactEffect( tr, nDamageType );
	}
}

int CGMOD_Player::GiveAmmo( int nCount, int nAmmoIndex, bool bSuppressSound )
{
	// ToDo

	int nAdd = CHL2_Player::GiveAmmo( nCount, nAmmoIndex, bSuppressSound );

	// Send some UserMessage

	return nAdd;
}

void CGMOD_Player::Weapon_Equip( CBaseCombatWeapon* pWeapon )
{
	// Todo

	// Make gamemode call. (Probably: PlayerCanPickupWeapon)

	CHL2_Player::Weapon_Equip( pWeapon );

	// Send a usermessage
}

void CGMOD_Player::Weapon_Drop( CBaseCombatWeapon* pWeapon, const Vector* pvecTarget, const Vector* pVelocity )
{
	CHL2MP_Player::Weapon_Drop( pWeapon, pvecTarget, pVelocity );
}

void CGMOD_Player::CreateViewModel( int viewmodelindex )
{
	if ( GetViewModel( viewmodelindex ) )
		return;

	CBaseViewModel *vm = ( CBaseViewModel * )CreateEntityByName( "viewmodel" );
	if ( vm )
	{
		vm->SetAbsOrigin( GetAbsOrigin() );
		vm->SetOwner( this );
		vm->SetIndex( viewmodelindex );
		DispatchSpawn( vm );
		vm->FollowEntity( this );
		m_hViewModel.Set( viewmodelindex, vm );
	}
}

void CGMOD_Player::SetupVisibility( CBaseEntity* pViewEntity, unsigned char* pvs, int pvssize )
{
	CHL2_Player::SetupVisibility( pViewEntity, pvs, pvssize );

	// Call gamemode hook. (GM:SetupPlayerVisibility)
}

void CGMOD_Player::InitialSpawn()
{
	// ToDo

	CBasePlayer::InitialSpawn();

	// Call gamemode hook. (GM:PlayerInitialSpawn)

	g_LuaNetworkedVars->PlayerInsert( this );
}

void CGMOD_Player::PreThink()
{
	CHL2_Player::PreThink();

	CHL2MP_Player::State_PreThink();

	// ToDo
}

void CGMOD_Player::PostThink()
{
	VPROF( "CGMOD_Player::PostThink", VPROF_BUDGETGROUP_GMOD );

	// ToDo

	// Call gamemode hook. (GM:PlayerPostThink)

	// Call another gamemode hook? (Unknown)

	CHL2MP_Player::PostThink();
}

void CGMOD_Player::OnDamagedByExplosion( const CTakeDamageInfo& info )
{
	// ToDo
	// Call gamemode hook. (GM:OnDamagedByExplosion)
}

void CGMOD_Player::PlayStepSound( Vector& vecOrigin, surfacedata_t* pSurface, float fVol, bool force )
{
	CBasePlayer::PlayStepSound( vecOrigin, pSurface, fVol, force );
}

void CGMOD_Player::DeathSound( const CTakeDamageInfo& info )
{
	// Call gamemode hook. (GM:PlayerDeathSound)

	CBasePlayer::DeathSound( info );
}

void CGMOD_Player::SetAnimation( PLAYER_ANIM playerAnim )
{
	// Nothing?
}

bool CGMOD_Player::ClientCommand( const CCommand& args )
{
	return CHL2_Player::ClientCommand( args );
}

bool CGMOD_Player::StartObserverMode( int mode )
{
	// ToDo

	return BaseClass::StartObserverMode( mode );
}

CBaseEntity* CGMOD_Player::EntSelectSpawnPoint()
{
	// Call gamemode hook. (GM:PlayerSelectSpawn)
	// ToDo

	return BaseClass::EntSelectSpawnPoint();
}

bool CGMOD_Player::GetInVehicle( IServerVehicle* pVehicle, int nRole )
{
	// ToDo

	// Call gamemode hook. (Probably GM:CanPlayerEnterVehicle)

	// Call gamemode hook. (Probably GM:PlayerEnteredVehicle)

	return BaseClass::GetInVehicle( pVehicle, nRole );
}

bool CGMOD_Player::BumpWeapon( CBaseCombatWeapon* pWeapon, bool unknown )
{
	return CHL2MP_Player::BumpWeapon( pWeapon );
}

void CGMOD_Player::PlayerUse()
{
	// Call gamemode hook. (GM:PlayerUse)

	BaseClass::PlayerUse(); // Temp function.
}

CBaseEntity* CGMOD_Player::FindUseEntity()
{
	CBaseEntity* pUseEnt = CBasePlayer::FindUseEntity();

	// Call gamemode hook. (GM:FindUseEntity)

	return pUseEnt;
}

void CGMOD_Player::PickupObject( CBaseEntity* pObject, bool bLimitMassAndSize )
{
	// Call gamemode hook. (Probably: GM:AllowPlayerPickup)

	CHL2_Player::PickupObject( pObject, bLimitMassAndSize );
}

void CGMOD_Player::PlayerRunCommand( CUserCmd* ucmd, IMoveHelper* moveHelper )
{
	CHL2_Player::PlayerRunCommand( ucmd, moveHelper );
}

int CGMOD_Player::GetFOV()
{
	return CBasePlayer::GetFOV(); // ToDo
}

int CGMOD_Player::GetFOVForNetworking()
{
	return CBasePlayer::GetFOVForNetworking(); // ToDo
}

void CGMOD_Player::UpdateButtonState( int nUserCmdButtonMask )
{
	CBasePlayer::UpdateButtonState( nUserCmdButtonMask );

	// Look throu it and call GM:PlayerButtonDown & GM:PlayerButtonUp
}

void CGMOD_Player::OnPlayerSay( const char* pMsg )
{
	//if ( V_stristr( pMsg, "bloxwich") )
		// Award Achievement (ToDo)
}

float CGMOD_Player::RestrictPlayerPitch()
{
	return 0.0f; // ToDo
}

bool CGMOD_Player::CanAttack()
{
	return true; // ToDo
}

int CGMOD_Player::GetMaxArmor()
{
	return 100; // ToDo
}

void CGMOD_Player::StartSprinting()
{
	// ToDo
}

void CGMOD_Player::StopSprinting()
{
	// ToDo
}

bool CGMOD_Player::CanSprint()
{
	return true; // ToDo 
}

void CGMOD_Player::HandleSpeedChanges()
{
	// ToDo
}

void CGMOD_Player::GiveAllItems()
{
}

void CGMOD_Player::SetPlayerColor( Vector )
{
	// ToDo
}

void CGMOD_Player::SetWeaponColor( Vector )
{
	// ToDo
}