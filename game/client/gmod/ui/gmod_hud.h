//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef GMOD_HUD_H
#define GMOD_HUD_H
#ifdef _WIN32
#pragma once
#endif

#include <hud_basechat.h>

class CHudGMod : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudGMod, CHudElement );

public:
	CHudGMod( const char *pElementName );

	virtual void Init();
	virtual void VidInit();
	virtual void Reset();

	virtual void OnThink();
	virtual void Paint();
	virtual void PaintBackground();
public:
	void MsgFunc_SWEPCmd( bf_read& msg );
	void MsgFunc_AmmoPickup( bf_read& msg );
	void MsgFunc_WeaponPickup( bf_read& msg );
};

#endif	//GMOD_HUD_H