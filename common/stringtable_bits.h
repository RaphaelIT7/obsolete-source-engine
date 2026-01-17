#ifndef STRINGTABLE_BITS
#define STRINGTABLE_BITS

extern int g_nMaxModelIndexBits;
extern int g_nMaxModels;

extern int g_nMaxGenericIndexBits;
extern int g_nMaxGenerics;

extern int g_nMaxDecalIndexBits;
// Why Precache here? Because g_nMaxDecals is already used in r_decal.cpp :/
extern int g_nMaxPrecacheDecals;

extern int g_nMaxSoundIndexBits;
extern int g_nMaxSounds;

extern void SV_SetupNetworkStringTableBits();

class INetworkStringTableContainer;
extern void CL_SetupNetworkStringTableBits( INetworkStringTableContainer* container, const char* tableName );

#endif