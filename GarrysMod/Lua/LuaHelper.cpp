#include "cbase.h"
#include "LuaHelper.h"
#include "CLuaClass.h"

void LuaHelper::KeyValuesToTable( KeyValues* kv, ILuaObject* obj, bool idk )
{

}

void LuaHelper::TableToKeyValues( ILuaObject* obj, KeyValues* kv )
{

}

void LuaHelper::KeyValuesToTablePreserveOrder(KeyValues* kv, ILuaObject* obj, bool idk)
{
}

bool LuaHelper::PushFunction( ILuaInterface* LUA, const char* library, const char* function )
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

void LuaHelper::CallOnLuaErrorHook( CLuaError* error, const char* unknown, unsigned long long wsid )
{
	// ToDo
}