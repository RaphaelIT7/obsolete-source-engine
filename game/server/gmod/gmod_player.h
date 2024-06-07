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

	// ToDo
};

inline CGMOD_Player *ToGMODPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<CGMOD_Player*>( pEntity );
}

#endif //GMOD_PLAYER_H