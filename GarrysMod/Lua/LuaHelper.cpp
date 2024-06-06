#include "cbase.h"
#include "LuaHelper.h"
#include "CLuaClass.h"

bool LuaHelper::PushFunction( GarrysMod::Lua::ILuaInterface* LUA, const char* library, const char* function )
{
	if ( !LUA )
		return false;

	CLuaObject obj;
	obj.Init( LUA );
	obj.SetFromGlobal( library );

	if ( obj.isTable() )
	{
		CLuaObject func;
		obj.GetMember( function, &func );
		if ( func.isFunction() )
		{
			func.Push();
			return true;
		}
	}

	return false;
}

void LuaHelper::CallOnLuaErrorHook( GarrysMod::Lua::ILuaGameCallback::CLuaError* error, const char* unknown, unsigned long long wsid )
{
	// ToDo
}