// This probably doesn't exist in Gmod but, oh well, these values does exist.

#include <GarrysMod/IGet.h>
#include "Lua/CLuaGamemode.h"
#if !defined(GAME_DLL) && !defined(CLIENT_DLL)
#include <GarrysMod/IGarrysMod.h>
#else
#include <../GarrysMod/Lua/garrysmod.h>
#endif
#include <GarrysMod/Lua/LuaInterface.h>
#include <GarrysMod/Lua/LuaGameCallback.h>
#include <globalvars_base.h>

#ifdef CLIENT_DLL
#define LUA_STATE GarrysMod::Lua::State::CLIENT
#define LUA_PATH "lsc"
#define LUA_SERVER false
#define LUA_CLIENT true
#elif defined(MENUSYSTEM)
#define LUA_STATE GarrysMod::Lua::State::MENU
#define LUA_PATH "LuaMenu"
#define LUA_SERVER false
#define LUA_CLIENT false
#else
#define LUA_STATE GarrysMod::Lua::State::SERVER
#define LUA_PATH "lsv"
#define LUA_SERVER true
#define LUA_CLIENT false
#endif

extern IGet* get;
#if !defined(GAME_DLL) && !defined(CLIENT_DLL)
extern IGarrysMod* igarrysmod;
#else
extern CGarrysMod* igarrysmod;
#endif
extern GarrysMod::Lua::ILuaGameCallback* g_LuaCallback;
extern GarrysMod::Lua::ILuaInterface* g_Lua;
extern CLuaGamemode* gGM;

#ifdef MENUSYSTEM
extern CGlobalVarsBase* gpGlobals;
#endif