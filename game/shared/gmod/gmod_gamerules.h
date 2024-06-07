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

	// ToDo
};

inline CGMODRules* GMODRules()
{
	return static_cast<CGMODRules*>(g_pGameRules);
}

#endif //GMOD_GAMERULES_H
