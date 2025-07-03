//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "voice_gamemgr.h"
#include <string.h>
#include <stdarg.h>
#include "player.h"
#include "ivoiceserver.h"
#include "usermessages.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


// These are stored off as CVoiceGameMgr is created and deleted.
CPlayerBitVec	g_PlayerModEnable;		// Set to 1 for each player if the player wants to use voice in this mod.
										// (If it's zero, then the server reports that the game rules are saying the
										// player can't hear anyone).

CPlayerBitVec	g_BanMasks[VOICE_MAX_PLAYERS];	// Tells which players don't want to hear each other.
												// These are indexed as clients and each bit represents a client
												// (so player entity is bit+1).

CPlayerBitVec	g_SentGameRulesMasks[VOICE_MAX_PLAYERS];	// These store the masks we last sent to each client so we can determine if
CPlayerBitVec	g_SentBanMasks[VOICE_MAX_PLAYERS];			// we need to resend them.
CPlayerBitVec	g_bWantModEnable;

bool			g_bIsPlayerTalking[VOICE_MAX_PLAYERS];		// If the player is currently considered talking
double			g_fLastPlayerTalked[VOICE_MAX_PLAYERS];		// When the player last talked
double			g_fLastPlayerUpdated[VOICE_MAX_PLAYERS];	// When the player was last updated

ConVar voice_serverdebug( "voice_serverdebug", "0" );

// Set game rules to allow all clients to talk to each other.
// Muted players still can't talk to each other.
ConVar sv_alltalk( "sv_alltalk", "0", FCVAR_NOTIFY | FCVAR_REPLICATED, "Players can hear all other players, no team restrictions" );

ConVar voicemgr_managerupdateinterval( "voicemgr_managerupdateinterval", "0.3", FCVAR_ARCHIVE, "How often the voice manager tries to update all players" );
ConVar voicemgr_updateinterval( "voicemgr_updateinterval", "0.1", FCVAR_ARCHIVE, "How often a player can be updated" );
ConVar voicemgr_stopdelay( "voicemgr_stopdelay", "1", FCVAR_ARCHIVE, "How many seconds have to pass before a player is considered to have stopped talking" );

CVoiceGameMgr g_VoiceGameMgr;


// ------------------------------------------------------------------------ //
// Static helpers.
// ------------------------------------------------------------------------ //

// Find a player with a case-insensitive name search.
#if 0
static CBasePlayer* FindPlayerByName(const char *pTestName)
{
	for(int i=1; i <= gpGlobals->maxClients; i++)
	{
		edict_t *pEdict = engine->PEntityOfEntIndex(i);
		if(pEdict)
		{
			CBaseEntity *pEnt = CBaseEntity::Instance(pEdict);
			if(pEnt && pEnt->IsPlayer())
			{			
				const char *pNetName = STRING(pEnt->GetEntityName());
				if(stricmp(pNetName, pTestName) == 0)
				{
					return (CBasePlayer*)pEnt;
				}
			}
		}
	}

	return NULL;
}
#endif

static void VoiceServerDebug( const char *pFmt, ... )
{
	char msg[4096];
	va_list marker;

	if( !voice_serverdebug.GetInt() )
		return;

	va_start( marker, pFmt );
	_vsnprintf( msg, sizeof(msg), pFmt, marker );
	va_end( marker );

	Msg( "%s", msg );
}


CVoiceGameMgr* GetVoiceGameMgr()
{
	return &g_VoiceGameMgr;
}



// ------------------------------------------------------------------------ //
// CVoiceGameMgr.
// ------------------------------------------------------------------------ //

CVoiceGameMgr::CVoiceGameMgr()
{
	m_UpdateInterval = 0;
	m_nMaxPlayers = 0;
	m_iProximityDistance = -1;
}


CVoiceGameMgr::~CVoiceGameMgr()
{
}

bool CVoiceGameMgr::Init(
	IVoiceGameMgrHelper *pHelper,
	int maxClients)
{		  
	m_pHelper = pHelper;
	m_nMaxPlayers = VOICE_MAX_PLAYERS < maxClients ? VOICE_MAX_PLAYERS : maxClients;

	return true;
}


void CVoiceGameMgr::SetHelper(IVoiceGameMgrHelper *pHelper)
{
	m_pHelper = pHelper;
}


void CVoiceGameMgr::Update(double frametime)
{
	// Only update periodically.
	m_UpdateInterval += frametime;
	if(m_UpdateInterval < voicemgr_managerupdateinterval.GetFloat())
		return;

	UpdateMasks();
}

// NOTE: When we added this, pPlayer became the sender and NOT the receiver, so you gotta flip the first two arguments of: m_pHelper->CanPlayerHearPlayer | g_pVoiceServer->SetClientListening | g_pVoiceServer->SetClientProximity
void CVoiceGameMgr::UpdatePlayer(CBasePlayer* pPlayer, bool bIsTalking)
{
	int iClient = pPlayer->edict()->m_EdictIndex - 1;
	if (bIsTalking)
	{
		g_fLastPlayerTalked[iClient] = gpGlobals->curtime;
	} else {
		// We update anyways, just to ensure that the code won't break, but we won't call the lua hook since we know their not talking and don't need it.
		if (g_bIsPlayerTalking[iClient] && (g_fLastPlayerTalked[iClient] + voicemgr_stopdelay.GetFloat()) > gpGlobals->curtime) // They are talking, and we have no reason to update so just skip it.
			return;
	}

	if ((g_fLastPlayerUpdated[iClient] + voicemgr_updateinterval.GetFloat()) > gpGlobals->curtime)
		return;

	CSingleUserRecipientFilter user( pPlayer );

	// Request the state of their "VModEnable" cvar.
	if(g_bWantModEnable[iClient])
	{
		UserMessageBegin( user, "RequestState" );
		MessageEnd();
		// Since this is reliable, only send it once
		g_bWantModEnable[iClient] = false;
	}

	CPlayerBitVec gameRulesMask;
	CPlayerBitVec ProximityMask;
	bool		bProximity = false;
	if( bIsTalking && g_PlayerModEnable[iClient] ) // We check for bIsTalking since we don't need to check anything when their not talking anyways. We call this function again anyways before we send out the voice packet so were completely fine like this
	{
		// Build a mask of who they can hear based on the game rules.
		for(int iOtherClient=0; iOtherClient < m_nMaxPlayers; iOtherClient++)
		{
			CBaseEntity *pEnt = UTIL_PlayerByIndex(iOtherClient+1);
			if(pEnt && pEnt->IsPlayer() && 
				((!!sv_alltalk.GetInt()) || m_pHelper->CanPlayerHearPlayer((CBasePlayer*)pEnt, pPlayer, bProximity )) )
			{
				gameRulesMask[iOtherClient] = true;
				ProximityMask[iOtherClient] = bProximity;
			}
		}
	}

	// If this is different from what the client has, send an update. 
	if(gameRulesMask != g_SentGameRulesMasks[iClient] || 
		g_BanMasks[iClient] != g_SentBanMasks[iClient])
	{
		g_SentGameRulesMasks[iClient] = gameRulesMask;
		g_SentBanMasks[iClient] = g_BanMasks[iClient];

		UserMessageBegin( user, "VoiceMask" );
			int dw;
			for(dw=0; dw < VOICE_MAX_PLAYERS_DW; dw++)
			{
				WRITE_LONG(gameRulesMask.GetDWord(dw));
				WRITE_LONG(g_BanMasks[iClient].GetDWord(dw));
			}
			WRITE_BYTE( !!g_PlayerModEnable[iClient] );
		MessageEnd();
	}

	// Tell the engine.
	for(int iOtherClient=0; iOtherClient < m_nMaxPlayers; iOtherClient++)
	{
		bool bCanHear = gameRulesMask[iOtherClient] && !g_BanMasks[iClient][iOtherClient];
		g_pVoiceServer->SetClientListening( iOtherClient+1, iClient+1, bCanHear );

		if ( bCanHear )
		{
			g_pVoiceServer->SetClientProximity( iOtherClient+1, iClient+1, !!ProximityMask[iOtherClient] );
		}
	}

	g_fLastPlayerUpdated[iClient] = gpGlobals->curtime;
	if ((g_fLastPlayerTalked[iClient] + voicemgr_stopdelay.GetFloat()) > gpGlobals->curtime)
	{
		g_bIsPlayerTalking[iClient] = true;
	} else {
		g_bIsPlayerTalking[iClient] = bIsTalking;
	}
}


void CVoiceGameMgr::ClientConnected(struct edict_t *pEdict)
{
	int index = ENTINDEX(pEdict) - 1;
	
	// Clear out everything we use for deltas on this guy.
	g_bWantModEnable[index] = true;
	g_SentGameRulesMasks[index].Init(0);
	g_SentBanMasks[index].Init(0);
}


bool CVoiceGameMgr::ClientCommand( CBasePlayer *pPlayer, const CCommand &args )
{
	int playerClientIndex = pPlayer->entindex() - 1;
	if(playerClientIndex < 0 || playerClientIndex >= m_nMaxPlayers)
	{
		VoiceServerDebug( "CVoiceGameMgr::ClientCommand: cmd %s from invalid client (%d)\n", args[0], playerClientIndex );
		return true;
	}

	bool bBan = stricmp( args[0], "vban" ) == 0;
	if( bBan && args.ArgC() >= 2 )
	{
		for(int i=1; i < args.ArgC(); i++)
		{
			uint32 mask = 0;
			sscanf( args[i], "%x", &mask);

			if( i <= VOICE_MAX_PLAYERS_DW )
			{
				VoiceServerDebug( "CVoiceGameMgr::ClientCommand: vban (0x%x) from %d\n", mask, playerClientIndex );
				g_BanMasks[playerClientIndex].SetDWord(i-1, mask);
			}
			else
			{
				VoiceServerDebug( "CVoiceGameMgr::ClientCommand: invalid index (%d)\n", i );
			}
		}

		// Force it to update the masks now.
		//UpdateMasks();		
		return true;
	}
	else if(stricmp( args[0], "VModEnable") == 0 && args.ArgC() >= 2)
	{
		VoiceServerDebug( "CVoiceGameMgr::ClientCommand: VModEnable (%d)\n", !!atoi( args[1] ) );
		g_PlayerModEnable[playerClientIndex] = !!atoi( args[1] );
		g_bWantModEnable[playerClientIndex] = false;
		//UpdateMasks();		
		return true;
	}
	else
	{
		return false;
	}
}


void CVoiceGameMgr::UpdateMasks()
{
	m_UpdateInterval = 0;

	for(int iClient=0; iClient < m_nMaxPlayers; iClient++)
	{
		CBaseEntity *pEnt = UTIL_PlayerByIndex(iClient+1);
		if(!pEnt || !pEnt->IsPlayer())
			continue;

		CBasePlayer *pPlayer = (CBasePlayer*)pEnt;

		UpdatePlayer(pPlayer, false);
	}
}

bool CVoiceGameMgr::IsPlayerIgnoringPlayer( int iTalker, int iListener )
{
	return !!g_BanMasks[iListener-1][iTalker-1];
}

void CVoiceGameMgr::SetProximityDistance( int iDistance )
{
	m_iProximityDistance = iDistance;
}

bool CVoiceGameMgr::CheckProximity( int iDistance )
{
	if ( m_iProximityDistance >= iDistance )
		return true;

	return false;
}
