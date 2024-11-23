#include "ILuaInterface.h"
#include "ILuaGameCallback.h"

class KeyValues;
namespace LuaHelper
{
	extern void KeyValuesToTable( KeyValues* kv, ILuaObject* obj, bool idk );
	extern void TableToKeyValues( ILuaObject* obj, KeyValues* kv );
	extern void KeyValuesToTablePreserveOrder( KeyValues* kv, ILuaObject* obj, bool idk );
	extern bool PushFunction( ILuaInterface* LUA, const char* library, const char* function );
	extern void CallOnLuaErrorHook( CLuaError* error, const char* unknown, unsigned long long wsid );
}