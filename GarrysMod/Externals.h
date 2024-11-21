// This probably doesn't exist in Gmod but, oh well, these values does exist.

#include <GarrysMod/IGet.h>
#include "Lua/CLuaGamemode.h"
#if !defined(GAME_DLL) && !defined(CLIENT_DLL)
#include <GarrysMod/IGarrysMod.h>
#else
#include <../GarrysMod/Lua/garrysmod.h>
#endif
#include <ILuaInterface.h>
#include <ILuaGameCallback.h>
#if defined(GAME_DLL) || defined(CLIENT_DLL)
#include "Lua/LuaNetworkedVars.h"
#endif
#include <globalvars_base.h>

#ifdef CLIENT_DLL
#define LUA_STATE State::CLIENT
#define LUA_PATH "lsc"
#define LUA_SERVER false
#define LUA_CLIENT true
#elif defined(MENUSYSTEM)
#define LUA_STATE State::MENU
#define LUA_PATH "LuaMenu"
#define LUA_SERVER false
#define LUA_CLIENT false
#else
#define LUA_STATE State::SERVER
#define LUA_PATH "lsv"
#define LUA_SERVER true
#define LUA_CLIENT false
#endif

extern IGet* get;
extern IGarrysMod* igarrysmod;
#if defined(GAME_DLL) || defined(CLIENT_DLL)
extern CGarrysMod garrysmod;
#endif
extern ILuaGameCallback* g_LuaCallback;
extern ILuaInterface* g_Lua;
extern CLuaGamemode* gGM;

#if defined(GAME_DLL) || defined(CLIENT_DLL)
extern CLuaNetworkedVars* g_LuaNetworkedVars;
#endif

#ifdef MENUSYSTEM
extern CGlobalVarsBase* gpGlobals;
#endif