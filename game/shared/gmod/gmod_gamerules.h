//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef GMOD_GAMERULES_H
#define GMOD_GAMERULES_H
#pragma once

#include "hl2mp_gamerules.h"
#include "teamplay_gamerules.h"
#include "gamevars_shared.h"
#include "Lua/CLuaClass.h"

#ifndef CLIENT_DLL
#include "gmod_player.h"
#endif

#define VEC_CROUCH_TRACE_MIN	GMODRules()->GetGMODViewVectors()->m_vCrouchTraceMin
#define VEC_CROUCH_TRACE_MAX	GMODRules()->GetGMODViewVectors()->m_vCrouchTraceMax


#ifdef CLIENT_DLL
	#define CGMODRules C_GMODRules
	#define CGMODGameRulesProxy C_GMODGameRulesProxy
#endif

class CGMODGameRulesProxy : public CHL2MPGameRulesProxy
{
public:
	DECLARE_CLASS( CGMODGameRulesProxy, CHL2MPGameRulesProxy );
	DECLARE_NETWORKCLASS();
};

class CGMODRules : public CTeamplayRules
{
public:
	DECLARE_CLASS( CGMODRules, CTeamplayRules );

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif
	
	CGMODRules();
	virtual ~CGMODRules();

	virtual void LevelInitPreEntity();
	virtual bool ShouldCollide( int, int );
	virtual const unsigned char* GetEncryptionKey();
	virtual void Precache();
	virtual void Think();
	virtual void OnSkillLevelChanged( int );
	virtual bool IsTeamplay();
	virtual const char* GetGameDescription();
	virtual void ClientDisconnected( edict_t *pClient );
	virtual float FlPlayerFallDamage( CBasePlayer *pPlayer );
	virtual bool FPlayerCanTakeDamage( CBasePlayer *pPlayer, CBaseEntity *pAttacker );
	virtual bool ShouldAutoAim( CBasePlayer *pPlayer, edict_t *target );
	virtual bool FlPlayerFallDeathDoesScreenFade( CBasePlayer *pl );
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual bool CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem );
	virtual bool CanHaveItem( CBasePlayer *pPlayer, CItem *pItem );
	virtual int ItemShouldRespawn( CItem *pItem );
	virtual void InitDefaultAIRelationships();
	virtual const char* AIClassText( int classType );
	virtual void CreateStandardEntities();
	virtual bool ShouldBurningPropsEmitLight();
	virtual void CleanUpMap( CLuaClass& obj );
};

inline CGMODRules* GMODRules()
{
	return static_cast<CGMODRules*>(g_pGameRules);
}

#endif //GMOD_GAMERULES_H
