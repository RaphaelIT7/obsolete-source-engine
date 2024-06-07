//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:		Player for GMOD.
//
//=============================================================================//

#include "cbase.h"
#include "c_gmod_player.h"
#include "gmod_gamerules.h"

// Don't alias here
#if defined( CGMOD_Player )
#undef CGMOD_Player	
#endif

LINK_ENTITY_TO_CLASS( player, C_GMOD_Player );

IMPLEMENT_CLIENTCLASS_DT(C_GMOD_Player, DT_GMOD_Player, CGMOD_Player)
END_RECV_TABLE()

BEGIN_PREDICTION_DATA( C_GMOD_Player )
END_PREDICTION_DATA()

C_GMOD_Player::C_GMOD_Player()
{

}

C_GMOD_Player::~C_GMOD_Player( void )
{

}
