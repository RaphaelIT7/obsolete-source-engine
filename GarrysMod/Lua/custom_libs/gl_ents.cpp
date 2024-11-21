#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"

LUA_FUNCTION_STATIC(ents_Create)
{
	// ToDo: Add a check for rendering -> ents.Create cannot be called while rendering
	const char* classname = LUA->CheckString(1);
	if (g_PhysWorldObject == NULL) {
		Warning("Trying to create entities too early! (%s)", classname);
		Push_Entity(NULL);
		return 1;
	}

	CBaseEntity* ent = CreateEntityByName(classname);
	Push_Entity(ent);

	return 1;
}

LUA_FUNCTION_STATIC(ents_GetAll)
{
	LUA->CreateTable();
	int idx = 0;
	const CEntInfo *pInfo = g_pEntityList->FirstEntInfo(); // ToDo: Look into it later again
	for ( ;pInfo; pInfo = pInfo->m_pNext ) {
		CBaseEntity *ent2 = (CBaseEntity*)pInfo->m_pEntity;
		if ( !ent2 ) {
			DevWarning( "NULL entity in global entity list!\n" );
			continue;
		}

		++idx;
		LUA->PushNumber(idx);
		Push_Entity(ent2);
		LUA->SetTable(-3);
	}

	return 1;
}

LUA_FUNCTION_STATIC(ents_FindByClass)
{
	const char* pClass = LUA->CheckString(1);
	LUA->CreateTable();
	int idx = 0;
	const CEntInfo *pInfo = g_pEntityList->FirstEntInfo();
	for ( ;pInfo; pInfo = pInfo->m_pNext ) {
		CBaseEntity *ent = (CBaseEntity*)pInfo->m_pEntity;
		if ( !ent ) {
			DevWarning( "NULL entity in global entity list!\n" );
			continue;
		}

#ifdef GAME_DLL
		if ( Q_strcmp(ent->GetClassname(), pClass) != 0 )
			continue;
#endif

		++idx;
		LUA->PushNumber(idx);
		Push_Entity(ent);
		LUA->SetTable(-3);
	}

	return 1;
}

LUA_FUNCTION_STATIC(ents_FindByName)
{
	const char* pName = LUA->CheckString(1);
	LUA->CreateTable();
	int idx = 0;
	const CEntInfo *pInfo = g_pEntityList->FirstEntInfo();
	for ( ;pInfo; pInfo = pInfo->m_pNext ) {
		CBaseEntity *ent = (CBaseEntity*)pInfo->m_pEntity;
		if ( !ent ) {
			DevWarning( "NULL entity in global entity list!\n" );
			continue;
		}

#ifdef GAME_DLL
		if ( Q_strcmp(ent->GetEntityName().ToCStr(), pName) != 0 )
			continue;
#endif

		++idx;
		LUA->PushNumber(idx);
		Push_Entity(ent);
		LUA->SetTable(-3);
	}

	return 1;
}

LUA_FUNCTION_STATIC(ents_GetByIndex)
{
	int entIndex = LUA->CheckNumber(1);
#ifdef CLIENT_DLL
	Push_Entity( cl_entitylist->GetBaseEntity(entIndex) );
#else
	Push_Entity( GMEntityByIndex(entIndex) );
#endif

	return 1;
}

LUA_FUNCTION_STATIC(ents_GetCount)
{
#ifdef CLIENT_DLL
	LUA->PushNumber(cl_entitylist->NumberOfEntities());
#else
	LUA->PushNumber(gEntList.NumberOfEntities());
#endif

	return 1;
}

LUA_FUNCTION_STATIC(ents_GetEdictCount)
{
#ifdef CLIENT_DLL // ToDo: Verify
	LUA->PushNumber(0);
#else
	LUA->PushNumber(gEntList.NumberOfEdicts());
#endif

	return 1;
}

void Ents_Library()
{
	g_Lua->PushSpecial(SPECIAL_GLOB);
		g_Lua->CreateTable();
			g_Lua->PushCFunction(ents_Create);
			g_Lua->SetField(-2, "Create");

			//g_Lua->PushCFunction(ents_FindAlongRay);
			//g_Lua->SetField(-2, "FindAlongRay");

			g_Lua->PushCFunction(ents_FindByClass);
			g_Lua->SetField(-2, "FindByClass");

			//g_Lua->PushCFunction(ents_FindByClassAndParent);
			//g_Lua->SetField(-2, "FindByClassAndParent");

			//g_Lua->PushCFunction(ents_FindByModel);
			//g_Lua->SetField(-2, "FindByModel");

			g_Lua->PushCFunction(ents_FindByName);
			g_Lua->SetField(-2, "FindByName");

			//g_Lua->PushCFunction(ents_FindInBox);
			//g_Lua->SetField(-2, "FindInBox");

			//g_Lua->PushCFunction(ents_FindInCone);
			//g_Lua->SetField(-2, "FindInCone");

			//g_Lua->PushCFunction(ents_FindInPVS);
			//g_Lua->SetField(-2, "FindInPVS");

			//g_Lua->PushCFunction(ents_FindInSphere);
			//g_Lua->SetField(-2, "FindInSphere");

			//g_Lua->PushCFunction(ents_FireTargets);
			//g_Lua->SetField(-2, "FireTargets");

			g_Lua->PushCFunction(ents_GetAll);
			g_Lua->SetField(-2, "GetAll");

			g_Lua->PushCFunction(ents_GetByIndex);
			g_Lua->SetField(-2, "GetByIndex");

			g_Lua->PushCFunction(ents_GetCount);
			g_Lua->SetField(-2, "GetCount");

			g_Lua->PushCFunction(ents_GetEdictCount);
			g_Lua->SetField(-2, "GetEdictCount");

			//g_Lua->PushCFunction(ents_GetMapCreatedEntity);
			//g_Lua->SetField(-2, "GetMapCreatedEntity");

		g_Lua->SetField(-2, "ents");

	g_Lua->Pop(1);
}

CLuaLibrary ents_library("ents", Ents_Library);