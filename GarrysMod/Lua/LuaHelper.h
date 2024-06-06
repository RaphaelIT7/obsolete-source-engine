#include "GarrysMod/Lua/LuaInterface.h"
#include "GarrysMod/Lua/LuaGameCallback.h"

namespace LuaHelper
{
	// extern void KeyValuesToTable( KeyValues* kv, ILuaObject* obj, bool idk );
	// extern void TableToKeyValues( ILuaObject* obj, KeyValues* kv );
	// extern void KeyValuesToTablePreserveOrder( KeyValues* kv, ILuaObject* obj, bool idk );
	extern bool PushFunction( GarrysMod::Lua::ILuaInterface* LUA, const char* library, const char* function );
	extern void CallOnLuaErrorHook( GarrysMod::Lua::ILuaGameCallback::CLuaError* error, const char* unknown, unsigned long long wsid );
}