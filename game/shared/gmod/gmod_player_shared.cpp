//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"

#ifdef CLIENT_DLL
#include "c_gmod_player.h"
#include "prediction.h"
#define CRecipientFilter C_RecipientFilter
#define CGMOD_Player C_GMOD_Player
#else
#include "gmod_player.h"
#endif
