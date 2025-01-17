#include "cbase.h" // Out of all the things I hate that this is required for client/server while the engine doesn't even have it...
#include "stringtable_bits.h"
#include "networkstringtabledefs.h"
#include <convar.h>

int g_nMaxModelIndexBits;
int g_nMaxModels;

int g_nMaxGenericIndexBits;
int g_nMaxGenerics;

int g_nMaxDecalIndexBits;
int g_nMaxPrecacheDecals;

int g_nMaxSoundIndexBits;
int g_nMaxSounds;

void SV_SetupNetworkStringTableBits()
{
	ConVarRef sv_precache_modelbits("sv_precache_modelbits");
	if (sv_precache_modelbits.IsValid())
	{
		g_nMaxModelIndexBits = sv_precache_modelbits.GetInt();
		g_nMaxModels = 1 << g_nMaxModelIndexBits;
	}

	ConVarRef sv_precache_generalbits("sv_precache_generalbits");
	if (sv_precache_generalbits.IsValid())
	{
		g_nMaxGenericIndexBits = sv_precache_generalbits.GetInt();
		g_nMaxGenerics = 1 << g_nMaxGenericIndexBits;
	}

	ConVarRef sv_precache_soundbits("sv_precache_soundbits");
	if (sv_precache_soundbits.IsValid())
	{
		g_nMaxSoundIndexBits = sv_precache_soundbits.GetInt();
		g_nMaxSounds = 1 << g_nMaxSoundIndexBits;
	}

	ConVarRef sv_precache_decalbits("sv_precache_decalbits");
	if (sv_precache_decalbits.IsValid())
	{
		g_nMaxDecalIndexBits = sv_precache_decalbits.GetInt();
		g_nMaxPrecacheDecals = 1 << g_nMaxDecalIndexBits;
	}
}

void CL_SetupNetworkStringTableBits( INetworkStringTableContainer* container, const char* tableName )
{
	if ( V_stricmp( tableName, "modelprecache" ) )
	{
		g_nMaxModelIndexBits = container->FindTable( tableName )->GetEntryBits();
		g_nMaxModels = 1 << g_nMaxModelIndexBits;
	} else if ( V_stricmp( tableName, "genericprecache" ) )
	{
		g_nMaxGenericIndexBits = container->FindTable( tableName )->GetEntryBits();
		g_nMaxGenerics = 1 << g_nMaxGenericIndexBits;
	} else if ( V_stricmp( tableName, "soundprecache" ) )
	{
		g_nMaxSoundIndexBits = container->FindTable( tableName )->GetEntryBits();
		g_nMaxSounds = 1 << g_nMaxSoundIndexBits;
	} else if ( V_stricmp( tableName, "decalprecache" ) )
	{
		g_nMaxDecalIndexBits = container->FindTable( tableName )->GetEntryBits();
		g_nMaxPrecacheDecals = 1 << g_nMaxDecalIndexBits;
	}
}