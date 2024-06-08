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

class C_GMOD_Player;
#include "c_basehlplayer.h"
#include "c_hl2mp_player.h"
#include "beamdraw.h"

//=============================================================================
// >> HL2MP_Player
//=============================================================================
class C_GMOD_Player : public C_HL2MP_Player
{
public:
	DECLARE_CLASS( C_GMOD_Player, C_HL2MP_Player );

	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();


	C_GMOD_Player();
	~C_GMOD_Player( void );

	// ToDo
};

inline C_GMOD_Player *ToGMODPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

	return dynamic_cast<C_GMOD_Player*>( pEntity );
}

#endif //HL2MP_PLAYER_H
