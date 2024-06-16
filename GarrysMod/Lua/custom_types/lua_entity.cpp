#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "tier1/strtools.h"

inline CBaseEntity* Get_ValidEntity(int index) // Helper function
{
	CBaseEntity* ent = Get_Entity(index);
	if (!ent)
		g_Lua->ArgError(index, "Tried to use a NULL Entity!");

	return ent;
}

LUA_FUNCTION_STATIC(Entity__tostring)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	char szBuf[128] = {};
	CBaseEntity* ent = Get_Entity(1);
	if (ent) {
		V_snprintf(szBuf, sizeof(szBuf),"Entity [%i][%s]", ent->entindex(), ent->GetClassname());
	} else {
		V_snprintf(szBuf, sizeof(szBuf),"[NULL Entity]");
	}

	LUA->PushString(szBuf);
	return 1;
}

LUA_FUNCTION_STATIC(Entity__index)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	LUA->GetMetaTable(1);
	LUA->Push(2);
	LUA->RawGet(-2);

	return 1;
}

LUA_FUNCTION_STATIC(Entity__newindex)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	// ToDo: Add Entity->GetLuaObject

	return 0;
}

LUA_FUNCTION_STATIC(Entity__eq)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Entity);

	LUA->PushBool(Get_Entity(1) == Get_Entity(2));
	return 1;
}

LUA_FUNCTION_STATIC(Entity__concat)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	// ToDo

	return 0;
}

LUA_FUNCTION_STATIC(Entity_IsValid)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	LUA->PushBool(Get_Entity(1) != NULL);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_IsWorld)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushBool(ent->IsWorld());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetPos)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
#ifndef CLIENT_DLL
	ent->CalcAbsolutePosition();
#endif
	Push_Vector(new Vector(ent->GetAbsOrigin()));

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetNetworkOrigin)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Push_Vector(new Vector(ent->GetAbsOrigin()));

	return 1;
}

LUA_FUNCTION_STATIC(Entity_SetNetworkOrigin)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Vector);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = Get_Vector(2);
	ent->SetAbsOrigin(*vec);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetPos)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Vector);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = Get_Vector(2);
	ent->SetAbsOrigin(*vec);

	// Do some checks. (Why aren't these also done inside SetNetworkOrigin xd)
	// %s[%i]:SetPos( %f %f %f ): Ignoring unreasonable position.

	return 0;
}

LUA_FUNCTION_STATIC(Entity_EntIndex)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->entindex());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_SetAttachment)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(3, GarrysMod::Lua::Type::Number);

	CBaseEntity* ent1 = Get_ValidEntity(1);
	CBaseEntity* ent2 = Get_ValidEntity(2);
	// ToDo: Finish it. use CSprite::SetAttachment

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetParent)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent1 = Get_ValidEntity(1);
	CBaseEntity* ent2 = Get_ValidEntity(2);
	ent1->SetParent(ent2, g_Lua->CheckNumberOpt(3, -1));

	return 0;
}

LUA_FUNCTION_STATIC(Entity_FollowBone)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent1 = Get_ValidEntity(1);
	CBaseEntity* ent2 = Get_ValidEntity(2);
	int boneid = g_Lua->CheckNumberOpt(3, -1);
	// ToDo

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetParent)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
#ifndef CLIENT_DLL
	Push_Entity(ent->GetParent());
#else
	LUA->PushNil(); // ToDo: FIXME
#endif

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetChildren)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int idx = 0;
	LUA->CreateTable();

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

LUA_FUNCTION_STATIC(Entity_SetBodygroup)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int bodygroup = LUA->CheckNumber(2);
	int value = LUA->CheckNumber(3);
	CBaseAnimating* anim = ent->GetBaseAnimating();
	if (anim)
		anim->SetBodygroup(bodygroup, value);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetBodygroup)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int id = LUA->CheckNumber(2);
	CBaseAnimating* anim = ent->GetBaseAnimating();
	if (anim) {
		LUA->PushNumber(anim->GetBodygroup(id));
	} else {
		LUA->PushNumber(0);
	}

	return 1;
}

LUA_FUNCTION_STATIC(Entity_Health)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->GetHealth());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_SetHealth)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int hp = LUA->CheckNumber(2);
	ent->SetHealth(hp);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetClass)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushString(ent->GetClassname());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetBaseVelocity)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Push_Vector(new Vector(ent->GetBaseVelocity()));

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetAbsVelocity)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Push_Vector(new Vector(ent->GetAbsVelocity()));

	return 1;
}

LUA_FUNCTION_STATIC(Entity_SetAbsVelocity)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Vector);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = Get_Vector(2);
	//ent->SetAbsVelocity(*vec);
	ent->SetBaseVelocity(*vec);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetVelocity)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
#ifndef CLIENT_DLL
	ent->CalcAbsolutePosition();
#endif
	Push_Vector(new Vector(ent->GetAbsVelocity()));

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetForward)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = new Vector();
	AngleVectors(ent->GetAbsAngles(), vec, nullptr, nullptr);
	Push_Vector(vec);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetRight)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = new Vector();
	AngleVectors(ent->GetAbsAngles(), nullptr, vec, nullptr);
	Push_Vector(vec);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetUp)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = new Vector();
	AngleVectors(ent->GetAbsAngles(), nullptr, nullptr, vec);
	Push_Vector(vec);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_SetModel)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	const char* mdl = LUA->CheckString(2);
#ifndef CLIENT_DLL // ToDo: Fix it later.
	UTIL_SetModel(ent, mdl);
#endif

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetModelName)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	const char* mdl = LUA->CheckString(2);
#ifndef CLIENT_DLL // ToDo: Fix it later.
	ent->SetModelName(AllocPooledString(mdl));
#endif

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetModel)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
#ifndef CLIENT_DLL // ToDo: Fix it later.
	LUA->PushString(ent->GetModelName().ToCStr());
#else
	LUA->PushNil();
#endif

	return 1;
}

LUA_FUNCTION_STATIC(Entity_SetOwner)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent1 = Get_ValidEntity(1);
	CBaseEntity* ent2 = Get_ValidEntity(2);
	ent1->SetOwnerEntity(ent2);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetOwner)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Push_Entity(ent->GetOwnerEntity());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetGroundEntity)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Push_Entity(ent->GetGroundEntity());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetMoveType)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->GetMoveType());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetMoveCollide)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->GetMoveCollide());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetSolid)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->GetSolid());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetCollisionGroup)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->GetCollisionGroup());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_WaterLevel)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->GetWaterLevel());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_GetMaxHealth)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	LUA->PushNumber(ent->GetMaxHealth());

	return 1;
}

LUA_FUNCTION_STATIC(Entity_EyePos)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Push_Vector(new Vector(ent->EyePosition()));

	return 1;
}

LUA_FUNCTION_STATIC(Entity_PhysicsInitShadow)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	bool allowPhysicsMovement = LUA->IsType(2, GarrysMod::Lua::Type::Bool) ? LUA->GetBool(2) : true;
	bool allowPhysicsRotation = LUA->IsType(3, GarrysMod::Lua::Type::Bool) ? LUA->GetBool(3) : true;

	if (ent->VPhysicsInitShadow(allowPhysicsMovement, allowPhysicsRotation) != NULL) {
		LUA->PushBool(true);
	} else {
		LUA->PushNil();
	}

	return 1;
}

LUA_FUNCTION_STATIC(Entity_MakePhysicsObjectAShadow)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	bool allowPhysicsMovement = LUA->IsType(2, GarrysMod::Lua::Type::Bool) ? LUA->GetBool(2) : true;
	bool allowPhysicsRotation = LUA->IsType(3, GarrysMod::Lua::Type::Bool) ? LUA->GetBool(3) : true;

#ifndef CLIENT_DLL
	ent->CalcAbsolutePosition();
#endif
	IPhysicsObject* obj = ent->VPhysicsGetObject();
	if (obj)
	{
		obj->SetShadow(1e4, 1e4, allowPhysicsMovement, allowPhysicsRotation);
		obj->UpdateShadow(ent->GetAbsOrigin(), ent->GetAbsAngles(), false, 0);
	}

	return 0;
}

LUA_FUNCTION_STATIC(Entity_PhysicsInit)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int solidType = LUA->CheckNumber(2);

	LUA->PushBool(ent->VPhysicsInitNormal((SolidType_t)solidType, 1, false) != NULL);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_PhysicsInitStatic)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int solidType = LUA->CheckNumber(2);

	IPhysicsObject* phys = ent->VPhysicsInitStatic();
	LUA->PushBool(phys != NULL);

	if (phys)
		ent->SetSolid((SolidType_t)solidType);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_PhysicsDestroy)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	ent->VPhysicsDestroyObject();

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetCollisionBounds)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Vector);
	LUA->CheckType(3, GarrysMod::Lua::Type::Vector);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* mins = Get_Vector(2);
	Vector* maxs = Get_Vector(3);
	ent->SetCollisionBounds(*mins, *maxs);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetSurroundingBounds)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Vector);
	LUA->CheckType(3, GarrysMod::Lua::Type::Vector);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* mins = Get_Vector(2);
	Vector* maxs = Get_Vector(3);

	CCollisionProperty* property = ent->CollisionProp();
	if (property) {
		property->SetSurroundingBoundsType((SurroundingBoundsType_t)3, mins, maxs);
	}

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetSurroundingBoundsType)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int bounds = LUA->CheckNumber(2); 

	CCollisionProperty* property = ent->CollisionProp();
	if (property) {
		property->SetSurroundingBoundsType((SurroundingBoundsType_t)bounds, NULL, NULL);
	}

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetSurroundingBounds)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);

	CCollisionProperty* property = ent->CollisionProp();
	if (property) {
		Vector* mins = new Vector();
		Vector* maxs = new Vector();
		property->WorldSpaceSurroundingBounds(mins, maxs);

		Push_Vector(mins);
		Push_Vector(maxs);
	} else {
		return 0;
	}

	return 2;
}

LUA_FUNCTION_STATIC(Entity_DrawShadow)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int shouldDraw = LUA->GetBool(2);
	if (shouldDraw) {
		ent->RemoveEffects(EF_NOSHADOW);
	} else {
		ent->AddEffects(EF_NOSHADOW);
	}

	return 0;
}

LUA_FUNCTION_STATIC(Entity_NextThink)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	double think = LUA->CheckNumber(2);

	ent->SetNextThink(think);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetPhysicsObjectCount)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);

	IPhysicsObject* physObjects[100];
	LUA->PushNumber(ent->VPhysicsGetObjectList(physObjects, 100));

	return 1;
}

LUA_FUNCTION_STATIC(Entity_SetSolid)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int solidType = LUA->CheckNumber(2);
	
	ent->SetSolid((SolidType_t)solidType);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetRenderMode)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int renderType = LUA->CheckNumber(2);
	
	ent->SetRenderMode((RenderMode_t)renderType);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_Remove)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	ent->Remove();

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetNotSolid)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	bool isNotSolid = LUA->GetBool(2);

	if (isNotSolid) {
		ent->SetSolidFlags(FSOLID_NOT_SOLID);
	} else {
		ent->RemoveSolidFlags(FSOLID_NOT_SOLID);
	}

	return 0;
}

LUA_FUNCTION_STATIC(Entity_SetMoveType)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	int moveType = LUA->CheckNumber(2);

	ent->SetMoveType((MoveType_t)moveType);

	return 0;
}

LUA_FUNCTION_STATIC(Entity_Activate)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	ent->Activate();

	return 0;
}

LUA_FUNCTION_STATIC(Entity_WorldToLocal)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = Get_Vector(2);

#ifndef CLIENT_DLL
	ent->CalcAbsolutePosition();
#endif
	Vector pos = ent->GetAbsOrigin();

	Vector* local_vec = new Vector();
	local_vec->x = vec->x - pos.x;
	local_vec->y = vec->y - pos.y;
	local_vec->z = vec->z - pos.z;

	Push_Vector(local_vec);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_LocalToWorld)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);
	LUA->CheckType(2, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Vector* vec = Get_Vector(2);

#ifndef CLIENT_DLL
	ent->CalcAbsolutePosition();
#endif
	Vector pos = ent->GetAbsOrigin();

	Vector* world_vec = new Vector();
	world_vec->x = vec->x + pos.x;
	world_vec->y = vec->y + pos.y;
	world_vec->z = vec->z + pos.z;

	Push_Vector(world_vec);

	return 1;
}

LUA_FUNCTION_STATIC(Entity_Spawn)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	ent->Spawn();

	return 0;
}

LUA_FUNCTION_STATIC(Entity_GetEyeAngles)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Entity);

	CBaseEntity* ent = Get_ValidEntity(1);
	Push_Angle(new QAngle(ent->EyeAngles()));

	return 1;
}

LUA_FUNCTION_STATIC(Global_Entity)
{
	int index = LUA->CheckNumber(1);
	CBaseEntity* ent = NULL;

#ifdef CLIENT_DLL
	if (index > 0 && index < MAX_EDICTS)
		ent = cl_entitylist->GetBaseEntity(index);
#else
	if (index > 0 && index < MAX_EDICTS)
		ent = gEntList.GetBaseEntity(gEntList.GetNetworkableHandle(index));
#endif

	Push_Entity(ent);

	return 1;
}

extern CLuaClass entity_class;
CBaseEntity* Get_Entity(int index)
{
	void* udata = entity_class.Get(index);
	if (!udata)
		return NULL;

#ifdef CLIENT_DLL
	return cl_entitylist->GetBaseEntityFromHandle(*(CBaseHandle*)udata);
#else
	return gEntList.GetBaseEntity(*(CBaseHandle*)udata);
#endif
}

void Push_Entity(CBaseEntity* ent)
{
	entity_class.Push(ent ? (void*)&ent->GetRefEHandle() : NULL);
}

void Entity_Class()
{
	g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->PushCFunction(Global_Entity);
		g_Lua->SetField(-2, "Entity");
	g_Lua->Pop(1);

	g_Lua->CreateMetaTableType("Entity", GarrysMod::Lua::Type::Entity);
		g_Lua->PushCFunction(Entity__index); // Gmod uses this format -> Angle____gc, Angle__Forward and so on.
		g_Lua->SetField(-2, "__index");

		g_Lua->PushCFunction(Entity__eq);
		g_Lua->SetField(-2, "__eq");

		g_Lua->PushCFunction(Entity__newindex);
		g_Lua->SetField(-2, "__newindex");

		g_Lua->PushCFunction(Entity__tostring);
		g_Lua->SetField(-2, "__tostring");

		//g_Lua->PushCFunction(Entity__concat); // ToDo
		//g_Lua->SetField(-2, "__concat");

		g_Lua->PushCFunction(Entity_IsValid);
		g_Lua->SetField(-2, "IsValid");

		g_Lua->PushCFunction(Entity_IsWorld);
		g_Lua->SetField(-2, "IsWorld");

		g_Lua->PushCFunction(Entity_GetPos);
		g_Lua->SetField(-2, "GetPos");

		g_Lua->PushCFunction(Entity_GetNetworkOrigin);
		g_Lua->SetField(-2, "GetNetworkOrigin");

		g_Lua->PushCFunction(Entity_SetNetworkOrigin);
		g_Lua->SetField(-2, "SetNetworkOrigin");

		g_Lua->PushCFunction(Entity_SetPos);
		g_Lua->SetField(-2, "SetPos");

		g_Lua->PushCFunction(Entity_EntIndex);
		g_Lua->SetField(-2, "EntIndex");
		
		//g_Lua->PushCFunction(Entity_SetAttachment);
		//g_Lua->SetField(-2, "SetAttachment");

		g_Lua->PushCFunction(Entity_SetParent);
		g_Lua->SetField(-2, "SetParent");

		g_Lua->PushCFunction(Entity_FollowBone);
		g_Lua->SetField(-2, "FollowBone");

		g_Lua->PushCFunction(Entity_GetParent);
		g_Lua->SetField(-2, "GetParent");

		g_Lua->PushCFunction(Entity_GetChildren);
		g_Lua->SetField(-2, "GetChildren");

		//g_Lua->PushCFunction(Entity_SetParentPhysNum);
		//g_Lua->SetField(-2, "SetParentPhysNum");

		//g_Lua->PushCFunction(Entity_GetParentPhysNum);
		//g_Lua->SetField(-2, "GetParentPhysNum");

		//g_Lua->PushCFunction(Entity_SetSkin);
		//g_Lua->SetField(-2, "SetSkin");

		//g_Lua->PushCFunction(Entity_GetSkin);
		//g_Lua->SetField(-2, "GetSkin");

		//g_Lua->PushCFunction(Entity_SkinCount);
		//g_Lua->SetField(-2, "SkinCount");

		g_Lua->PushCFunction(Entity_SetBodygroup);
		g_Lua->SetField(-2, "SetBodygroup");

		g_Lua->PushCFunction(Entity_GetBodygroup);
		g_Lua->SetField(-2, "GetBodygroup");

		g_Lua->PushCFunction(Entity_Health);
		g_Lua->SetField(-2, "Health");

		//g_Lua->PushCFunction(Entity_SetEntity);
		//g_Lua->SetField(-2, "SetEntity");

		g_Lua->PushCFunction(Entity_SetHealth);
		g_Lua->SetField(-2, "SetHealth");

		g_Lua->PushCFunction(Entity_GetClass);
		g_Lua->SetField(-2, "GetClass");

		//g_Lua->PushCFunction(Entity_EmitSound);
		//g_Lua->SetField(-2, "EmitSound");

		//g_Lua->PushCFunction(Entity_StopSound);
		//g_Lua->SetField(-2, "StopSound");

		//g_Lua->PushCFunction(Entity_StartLoopingSound);
		//g_Lua->SetField(-2, "StartLoopingSound");

		//g_Lua->PushCFunction(Entity_StopLoopingSound);
		//g_Lua->SetField(-2, "StopLoopingSound");

		//g_Lua->PushCFunction(Entity_SetColor); // ToDo: Implement ILuaInterface::GetColor properly
		//g_Lua->SetField(-2, "SetColor");

		//g_Lua->PushCFunction(Entity_GetColor);
		//g_Lua->SetField(-2, "GetColor");

		//g_Lua->PushCFunction(Entity_SetColor4Part);
		//g_Lua->SetField(-2, "SetColor4Part");

		//g_Lua->PushCFunction(Entity_GetColor4Part);
		//g_Lua->SetField(-2, "GetColor4Part");

		g_Lua->PushCFunction(Entity_GetBaseVelocity);
		g_Lua->SetField(-2, "GetBaseVelocity");

		g_Lua->PushCFunction(Entity_GetAbsVelocity);
		g_Lua->SetField(-2, "GetAbsVelocity");

		g_Lua->PushCFunction(Entity_SetAbsVelocity);
		g_Lua->SetField(-2, "SetAbsVelocity");

		g_Lua->PushCFunction(Entity_GetVelocity);
		g_Lua->SetField(-2, "GetVelocity");

		g_Lua->PushCFunction(Entity_GetForward);
		g_Lua->SetField(-2, "GetForward");

		g_Lua->PushCFunction(Entity_GetRight);
		g_Lua->SetField(-2, "GetRight");

		g_Lua->PushCFunction(Entity_GetUp);
		g_Lua->SetField(-2, "GetUp");

		g_Lua->PushCFunction(Entity_SetModel);
		g_Lua->SetField(-2, "SetModel");

		g_Lua->PushCFunction(Entity_SetModelName);
		g_Lua->SetField(-2, "SetModelName");

		g_Lua->PushCFunction(Entity_GetModel);
		g_Lua->SetField(-2, "GetModel");

		g_Lua->PushCFunction(Entity_SetOwner);
		g_Lua->SetField(-2, "SetOwner");

		g_Lua->PushCFunction(Entity_GetOwner);
		g_Lua->SetField(-2, "GetOwner");

		g_Lua->PushCFunction(Entity_GetGroundEntity);
		g_Lua->SetField(-2, "GetGroundEntity");

		g_Lua->PushCFunction(Entity_GetMoveType);
		g_Lua->SetField(-2, "GetMoveType");

		g_Lua->PushCFunction(Entity_GetMoveType);
		g_Lua->SetField(-2, "GetMoveCollide");

		g_Lua->PushCFunction(Entity_GetSolid);
		g_Lua->SetField(-2, "GetSolid");

		g_Lua->PushCFunction(Entity_GetCollisionGroup);
		g_Lua->SetField(-2, "GetCollisionGroup");

		g_Lua->PushCFunction(Entity_WaterLevel);
		g_Lua->SetField(-2, "WaterLevel");

		g_Lua->PushCFunction(Entity_GetMaxHealth);
		g_Lua->SetField(-2, "GetMaxHealth");

		g_Lua->PushCFunction(Entity_EyePos);
		g_Lua->SetField(-2, "EyePos");

		g_Lua->PushCFunction(Entity_PhysicsInitShadow);
		g_Lua->SetField(-2, "PhysicsInitShadow");

		g_Lua->PushCFunction(Entity_MakePhysicsObjectAShadow);
		g_Lua->SetField(-2, "MakePhysicsObjectAShadow");

		g_Lua->PushCFunction(Entity_PhysicsInit);
		g_Lua->SetField(-2, "PhysicsInit");

		g_Lua->PushCFunction(Entity_PhysicsInitStatic);
		g_Lua->SetField(-2, "PhysicsInitStatic");

		g_Lua->PushCFunction(Entity_PhysicsDestroy);
		g_Lua->SetField(-2, "PhysicsDestroy");

		//g_Lua->PushCFunction(Entity_PhysicsInitMultiConvex);
		//g_Lua->SetField(-2, "PhysicsInitMultiConvex");

		//g_Lua->PushCFunction(Entity_PhysicsInitConvex);
		//g_Lua->SetField(-2, "PhysicsInitConvex");

		//g_Lua->PushCFunction(Entity_PhysicsFromMesh);
		//g_Lua->SetField(-2, "PhysicsFromMesh");

		//g_Lua->PushCFunction(Entity_PhysicsInitSphere);
		//g_Lua->SetField(-2, "PhysicsInitSphere");

		//g_Lua->PushCFunction(Entity_PhysicsInitBox);
		//g_Lua->SetField(-2, "PhysicsInitBox");

		g_Lua->PushCFunction(Entity_SetCollisionBounds);
		g_Lua->SetField(-2, "SetCollisionBounds");

		g_Lua->PushCFunction(Entity_SetSurroundingBounds);
		g_Lua->SetField(-2, "SetSurroundingBounds");

		g_Lua->PushCFunction(Entity_SetSurroundingBoundsType);
		g_Lua->SetField(-2, "SetSurroundingBoundsType");

		g_Lua->PushCFunction(Entity_GetSurroundingBounds); // BUG: It's probably in worldspace :/
		g_Lua->SetField(-2, "GetSurroundingBounds");

		//g_Lua->PushCFunction(Entity_GetCollisionBounds);
		//g_Lua->SetField(-2, "GetCollisionBounds");

		//g_Lua->PushCFunction(Entity_SetCollisionBoundsWS);
		//g_Lua->SetField(-2, "SetCollisionBoundsWS");

		g_Lua->PushCFunction(Entity_DrawShadow);
		g_Lua->SetField(-2, "DrawShadow");

		//g_Lua->PushCFunction(Entity_LookupAttachment);
		//g_Lua->SetField(-2, "LookupAttachment");

		//g_Lua->PushCFunction(Entity_TranslatePhysBoneToBone);
		//g_Lua->SetField(-2, "TranslatePhysBoneToBone");

		//g_Lua->PushCFunction(Entity_GetAttachment);
		//g_Lua->SetField(-2, "GetAttachment");

		g_Lua->PushCFunction(Entity_NextThink);
		g_Lua->SetField(-2, "NextThink");

		//g_Lua->PushCFunction(Entity_Weapon_SetActivity);
		//g_Lua->SetField(-2, "Weapon_SetActivity");

		//g_Lua->PushCFunction(Entity_Weapon_TranslateActivity);
		//g_Lua->SetField(-2, "Weapon_TranslateActivity");

		//g_Lua->PushCFunction(Entity_GetSequence);
		//g_Lua->SetField(-2, "GetSequence");

		//g_Lua->PushCFunction(Entity_LookupSequence);
		//g_Lua->SetField(-2, "LookupSequence");

		//g_Lua->PushCFunction(Entity_SetSequence);
		//g_Lua->SetField(-2, "SetSequence");

		//g_Lua->PushCFunction(Entity_SetPlaybackRate);
		//g_Lua->SetField(-2, "SetPlaybackRate");

		//g_Lua->PushCFunction(Entity_GetPlaybackRate);
		//g_Lua->SetField(-2, "GetPlaybackRate");

		//g_Lua->PushCFunction(Entity_ResetSequenceInfo);
		//g_Lua->SetField(-2, "ResetSequenceInfo");

		//g_Lua->PushCFunction(Entity_SetCycle);
		//g_Lua->SetField(-2, "SetCycle");

		//g_Lua->PushCFunction(Entity_ResetSequence);
		//g_Lua->SetField(-2, "ResetSequence");

		//g_Lua->PushCFunction(Entity_SequenceDuration);
		//g_Lua->SetField(-2, "SequenceDuration");

		//g_Lua->PushCFunction(Entity_SelectWeightedSequence);
		//g_Lua->SetField(-2, "SelectWeightedSequence");

		//g_Lua->PushCFunction(Entity_SelectWeightedSequenceSeeded);
		//g_Lua->SetField(-2, "SelectWeightedSequenceSeeded");

		//g_Lua->PushCFunction(Entity_GetPhysicsObject); // ToDo: Implement IPhysicsObject class
		//g_Lua->SetField(-2, "GetPhysicsObject");

		//g_Lua->PushCFunction(Entity_GetPhysicsObjectNum); // ToDo: Implement IPhysicsObject class
		//g_Lua->SetField(-2, "GetPhysicsObjectNum");

		g_Lua->PushCFunction(Entity_GetPhysicsObjectCount);
		g_Lua->SetField(-2, "GetPhysicsObjectCount");

		g_Lua->PushCFunction(Entity_SetSolid);
		g_Lua->SetField(-2, "SetSolid");

		g_Lua->PushCFunction(Entity_SetRenderMode);
		g_Lua->SetField(-2, "SetRenderMode");

		g_Lua->PushCFunction(Entity_Remove);
		g_Lua->SetField(-2, "Remove");

		g_Lua->PushCFunction(Entity_SetNotSolid);
		g_Lua->SetField(-2, "SetNotSolid");

		g_Lua->PushCFunction(Entity_SetMoveType);
		g_Lua->SetField(-2, "SetMoveType");

		g_Lua->PushCFunction(Entity_Activate);
		g_Lua->SetField(-2, "Activate");

		g_Lua->PushCFunction(Entity_WorldToLocal);
		g_Lua->SetField(-2, "WorldToLocal");

		g_Lua->PushCFunction(Entity_LocalToWorld);
		g_Lua->SetField(-2, "LocalToWorld");

		//g_Lua->PushCFunction(Entity_WorldToLocalAngles);
		//g_Lua->SetField(-2, "WorldToLocalAngles");

		//g_Lua->PushCFunction(Entity_LocalToWorldAngles);
		//g_Lua->SetField(-2, "LocalToWorldAngles");

		g_Lua->PushCFunction(Entity_Spawn);
		g_Lua->SetField(-2, "Spawn");

		//g_Lua->PushCFunction(Entity_SetMaterial);
		//g_Lua->SetField(-2, "SetMaterial");

		//g_Lua->PushCFunction(Entity_GetMaterial);
		//g_Lua->SetField(-2, "GetMaterial");

		//g_Lua->PushCFunction(Entity_SetSubMaterial);
		//g_Lua->SetField(-2, "SetSubMaterial");

		//g_Lua->PushCFunction(Entity_GetSubMaterial);
		//g_Lua->SetField(-2, "GetSubMaterial");

		//g_Lua->PushCFunction(Entity_SetKeyValue);
		//g_Lua->SetField(-2, "SetKeyValue");

		//g_Lua->PushCFunction(Entity_ClearPoseParameters);
		//g_Lua->SetField(-2, "ClearPoseParameters");

		//g_Lua->PushCFunction(Entity_GetNumPoseParameters);
		//g_Lua->SetField(-2, "GetNumPoseParameters");

		//g_Lua->PushCFunction(Entity_GetPoseParameterName);
		//g_Lua->SetField(-2, "GetPoseParameterName");

		//g_Lua->PushCFunction(Entity_GetPoseParameterRange);
		//g_Lua->SetField(-2, "GetPoseParameterRange");

		//g_Lua->PushCFunction(Entity_SetPoseParameter);
		//g_Lua->SetField(-2, "SetPoseParameter");

		//g_Lua->PushCFunction(Entity_LookupPoseParameter);
		//g_Lua->SetField(-2, "LookupPoseParameter");

		//g_Lua->PushCFunction(Entity_GetPoseParameter);
		//g_Lua->SetField(-2, "GetPoseParameter");

		//g_Lua->PushCFunction(Entity_GetFlexScale);
		//g_Lua->SetField(-2, "GetFlexScale");

		//g_Lua->PushCFunction(Entity_GetFlexWeight);
		//g_Lua->SetField(-2, "GetFlexWeight");

		//g_Lua->PushCFunction(Entity_SetFlexWeight);
		//g_Lua->SetField(-2, "SetFlexWeight");

		//g_Lua->PushCFunction(Entity_SetFlexScale);
		//g_Lua->SetField(-2, "SetFlexScale");

		g_Lua->PushCFunction(Entity_GetEyeAngles);
		g_Lua->SetField(-2, "GetEyeAngles");

		//g_Lua->PushCFunction(Entity_SetEyeAngles);
		//g_Lua->SetField(-2, "SetEyeAngles");

		//g_Lua->PushCFunction(Entity_SetBodygroupByName);
		//g_Lua->SetField(-2, "SetBodygroupByName");

		//g_Lua->PushCFunction(Entity_GetBodygroupByName);
		//g_Lua->SetField(-2, "GetBodygroupByName");
}

CLuaClass entity_class( "Entity", GarrysMod::Lua::Type::Entity, Entity_Class );