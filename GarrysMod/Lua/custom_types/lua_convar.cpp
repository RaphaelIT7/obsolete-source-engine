#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "tier1/strtools.h"

extern CLuaClass LC_ConVar;
LUA_FUNCTION_STATIC(ConVar__tostring)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);
	char szBuf[270] = {};
	V_snprintf(szBuf, sizeof(szBuf),"ConVar [%s]", var ? var->GetName() : "NULL");
	LUA->PushString(szBuf);
	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetBool)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushBool(var->GetBool());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetDefault)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushString(var->GetDefault());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetFlags)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushNumber(var->GetFlags()); // ToDo

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetFloat)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushNumber(var->GetFloat());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetHelpText)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushString(var->GetHelpText());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetInt)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushNumber(var->GetInt());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetMax)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	float max;
	var->GetMax(max);

	LUA->PushNumber(max);

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetMin)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	float min;
	var->GetMin(min);

	LUA->PushNumber(min);

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetName)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushString(var->GetName());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_GetString)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	LUA->PushString(var->GetString());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_IsFlagSet)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);
	int flag = LUA->CheckNumber(2);

	LUA->PushBool(var->IsFlagSet(flag));

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_Revert)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);

	var->Revert();

	return 0;
}

LUA_FUNCTION_STATIC(ConVar_SetBool)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);
	bool value = LUA->GetBool(2);

	var->SetValue(value ? "1" : "0");

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_SetFloat)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);
	float value = LUA->CheckNumber(2);

	var->SetValue(std::to_string(value).c_str());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_SetInt)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);
	int value = LUA->CheckNumber(2);

	var->SetValue(std::to_string(value).c_str());

	return 1;
}

LUA_FUNCTION_STATIC(ConVar_SetString)
{
	ConVar* var = (ConVar*)LC_ConVar.Get(1);
	const char* value = LUA->CheckString(2);

	var->SetValue(value);

	return 1;
}

void Push_ConVar(const ConVar* var)
{
	LC_ConVar.Push((void*)var);
}

void ConVar_Class()
{
	g_Lua->CreateMetaTableType("ConVar", GarrysMod::Lua::Type::ConVar);
		g_Lua->PushCFunction(ConVar__tostring);
		g_Lua->SetField(-2, "__tostring");

		g_Lua->PushCFunction(ConVar_GetBool);
		g_Lua->SetField(-2, "GetBool");

		g_Lua->PushCFunction(ConVar_GetDefault);
		g_Lua->SetField(-2, "GetDefault");

		g_Lua->PushCFunction(ConVar_GetFlags);
		g_Lua->SetField(-2, "GetFlags");

		g_Lua->PushCFunction(ConVar_GetHelpText);
		g_Lua->SetField(-2, "GetHelpText");

		g_Lua->PushCFunction(ConVar_GetFloat);
		g_Lua->SetField(-2, "GetFloat");

		g_Lua->PushCFunction(ConVar_GetInt);
		g_Lua->SetField(-2, "GetInt");

		g_Lua->PushCFunction(ConVar_GetMax);
		g_Lua->SetField(-2, "GetMax");

		g_Lua->PushCFunction(ConVar_GetMin);
		g_Lua->SetField(-2, "GetMin");

		g_Lua->PushCFunction(ConVar_GetName);
		g_Lua->SetField(-2, "GetName");

		g_Lua->PushCFunction(ConVar_GetString);
		g_Lua->SetField(-2, "GetString");

		g_Lua->PushCFunction(ConVar_IsFlagSet);
		g_Lua->SetField(-2, "IsFlagSet");

		g_Lua->PushCFunction(ConVar_Revert);
		g_Lua->SetField(-2, "Revert");

		g_Lua->PushCFunction(ConVar_SetBool);
		g_Lua->SetField(-2, "SetBool");

		g_Lua->PushCFunction(ConVar_SetFloat);
		g_Lua->SetField(-2, "SetFloat");

		g_Lua->PushCFunction(ConVar_SetInt);
		g_Lua->SetField(-2, "SetInt");

		g_Lua->PushCFunction(ConVar_SetString);
		g_Lua->SetField(-2, "SetString");
}

CLuaClass LC_ConVar("ConVar", GarrysMod::Lua::Type::ConVar, ConVar_Class);