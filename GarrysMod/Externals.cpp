// Doesn't exist in Gmod.
#include "cbase.h"
#include "Externals.h"

IGet* get;
GarrysMod::Lua::ILuaInterface* g_Lua;
CLuaGamemode* gGM;

#if defined(GAME_DLL) || defined(CLIENT_DLL)
CLuaNetworkedVars* g_LuaNetworkedVars;
#endif

#ifdef MENUSYSTEM
CGlobalVarsBase* gpGlobals;
#endif