#include "cbase.h"
#include "LuaHelper.h"
#include "CLuaClass.h"
#include <Externals.h>

// There is probably a better way to do this.
void LuaHelper::TableToKeyValues(ILuaObject* obj, KeyValues* root) {
	obj->Push();
	g_Lua->PushNil();  // Push nil to -1 to start at first key
	while (g_Lua->Next(-2)) {
		const char* key = g_Lua->GetString(-2);
		if (key == NULL) throw "LuaHelper::TableToKeyValues: key == NULL or key string parsing failure";

		auto type = g_Lua->GetType(-1);

		switch (type) {
			case Type::Table: {
				CLuaObject obj;
				obj.Init(g_Lua);
				obj.SetFromStack(-1);
				KeyValues* kvp = new KeyValues(key);
				TableToKeyValues(&obj, kvp);
				root->AddSubKey(kvp);
				obj.UnReference();
				break;
			}
			case Type::String: {
				root->SetString(key, g_Lua->GetString(-1));
				break;
			}
			case Type::Number: {
				root->SetFloat(key, static_cast<float>(g_Lua->GetNumber(-1)));
				break;
			}
			case Type::Bool: {
				root->SetInt(key, g_Lua->GetBool(-1) ? 1 : 0);
				break;
			}
		}

		g_Lua->Pop(1);
	}
	g_Lua->Pop();  // Pop the reference
}

bool LuaHelper::PushFunction(ILuaInterface* LUA, const char* library, const char* function) {
	if ( !LUA ) return false;

	CLuaObject obj;
	obj.Init(LUA);
	obj.SetFromGlobal(library);

	if (obj.isTable()) {
		CLuaObject func;
		obj.GetMember(function, &func);
		if (func.isFunction()) {
			func.Push();
			return true;
		}
	}

	return false;
}

void LuaHelper::CallOnLuaErrorHook(CLuaError* error, const char* unknown,
								   unsigned long long wsid) {
	// ToDo
}