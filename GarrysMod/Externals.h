// This probably doesn't exist in Gmod but, oh well, these values does exist.

#include <GarrysMod/IGet.h>
#ifndef GAME_DLL
#include <GarrysMod/IGarrysMod.h>
#else
#include <../GarrysMod/Lua/garrysmod.h>
#endif
#include <GarrysMod/Lua/LuaGameCallback.h>
#include <globalvars_base.h>

extern IGet* get;
#ifndef GAME_DLL
extern IGarrysMod* igarrysmod;
#else
extern CGarrysMod* igarrysmod;
#endif
extern GarrysMod::Lua::ILuaGameCallback* g_LuaCallback;
extern GarrysMod::Lua::ILuaInterface* g_Lua;

#ifdef MENUSYSTEM
extern CGlobalVarsBase* gpGlobals;
#endif