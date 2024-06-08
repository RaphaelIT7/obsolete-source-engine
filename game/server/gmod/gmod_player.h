//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef GMOD_PLAYER_H
#define GMOD_PLAYER_H
#pragma once

#include "basemultiplayerplayer.h"
#include "hl2_playerlocaldata.h"
#include "hl2mp_player.h"
#include "simtimer.h"
#include "soundenvelope.h"
#include "gmod_gamerules.h"
#include "utldict.h"

//=============================================================================
// >> GMOD_Player
//=============================================================================
class CGMOD_Player;
class CGMODPlayerStateInfo
{
public:
	HL2MPPlayerState m_iPlayerState;
	const char *m_pStateName;

	void (CGMOD_Player::*pfnEnterState)();	// Init and deinit the state.
	void (CGMOD_Player::*pfnLeaveState)();

	void (CGMOD_Player::*pfnPreThink)();	// Do a PreThink() in this state.
};

class CGMOD_Player : public CHL2MP_Player
{
public:
	DECLARE_CLASS( CGMOD_Player, CHL2MP_Player );

	CGMOD_Player();
	~CGMOD_Player( void );
	
	static CGMOD_Player *CreatePlayer( const char *className, edict_t *ed )
	{
		CGMOD_Player::s_PlayerEdict = ed;
		return (CGMOD_Player*)CreateEntityByName( className );
	}

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	virtual bool ShouldCollide( int collisionGroup0, int collisionGroup1 );
	virtual int ShouldTransmit( const CCheckTransmitInfo *pInfo );
	virtual void Spawn();
	virtual void Precache();
	virtual void TraceAttack( const CTakeDamageInfo &info, const Vector &vecDir, CGameTrace *tr, CDmgAccumulator *pAccumulator );
	virtual int OnTakeDamage( const CTakeDamageInfo &info );
	virtual void Event_Killed( const CTakeDamageInfo &info );
	virtual void ChangeTeam( int iTeamNum );
	virtual void ViewPunch( const QAngle &angleOffset );
	virtual void FireBullets( const FireBulletsInfo_t &info );
	virtual void DoImpactEffect( CGameTrace &tr, int nDamageType );
	virtual int GiveAmmo( int nCount, int nAmmoIndex, bool bSuppressSound );
	virtual void Weapon_Equip( CBaseCombatWeapon *pWeapon );
	virtual void Weapon_Drop( CBaseCombatWeapon *pWeapon, const Vector *pvecTarget = NULL, const Vector *pVelocity = NULL );
	virtual void CreateViewModel( int viewmodelindex = 0 );
	virtual void SetupVisibility( CBaseEntity *pViewEntity, unsigned char *pvs, int pvssize );
	virtual void InitialSpawn();
	virtual void PreThink();
	virtual void PostThink();
	virtual void OnDamagedByExplosion( const CTakeDamageInfo &info );
	virtual void PlayStepSound( Vector &vecOrigin, surfacedata_t *pSurface, float fVol, bool force );
	virtual void DeathSound( const CTakeDamageInfo &info );
	virtual void SetAnimation( PLAYER_ANIM playerAnim );
	virtual bool ClientCommand( const CCommand &args );
	virtual bool StartObserverMode( int mode );
	virtual CBaseEntity *EntSelectSpawnPoint();
	virtual bool GetInVehicle( IServerVehicle *pVehicle, int nRole );
	virtual bool BumpWeapon( CBaseCombatWeapon *pWeapon, bool unknown );
	virtual void PlayerUse();
	virtual CBaseEntity *FindUseEntity();
	virtual void PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize );
	virtual void PlayerRunCommand( CUserCmd *ucmd, IMoveHelper *moveHelper );
	virtual int GetFOV();
	virtual int GetFOVForNetworking();
	virtual void UpdateButtonState( int nUserCmdButtonMask );
	virtual void OnPlayerSay( const char *pMsg );
	virtual float RestrictPlayerPitch();
	virtual bool CanAttack();
	virtual int GetMaxArmor(); 
	virtual void StartSprinting();
	virtual void StopSprinting();
	virtual bool CanSprint();
	virtual void HandleSpeedChanges();
	virtual void GiveAllItems();
	virtual void SetPlayerColor( Vector );
	virtual void SetWeaponColor( Vector );

	// ToDo: Add GMod's m_iGModFlags
};

inline CGMOD_Player *ToGMODPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CGMOD_Player*>( pEntity );
}

#endif //GMOD_PLAYER_H