#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "tier1/strtools.h"

LUA_FUNCTION_STATIC(Vector__gc)
{
	if (!LUA->IsType(1, Type::Vector))
		return 0;

	return 0;
}

LUA_FUNCTION_STATIC(Vector__tostring)
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec = Get_Vector(1);
	char szBuf[64] = {};
	V_snprintf(szBuf, sizeof(szBuf),"%f %f %f", vec->x, vec->y, vec->z);
	LUA->PushString(szBuf);
	return 1;
}

LUA_FUNCTION_STATIC(Vector__index)
{
	LUA->CheckType(1, Type::Vector);

	const char* key = LUA->GetString(2);
	if (key != NULL) 
	{
		Vector* vec1 = Get_Vector(1);
		if (strcmp(key, "x") == 0 || strcmp(key, "1") == 0) {
			LUA->PushNumber(vec1->x);
			return 1;
		} else if (strcmp(key, "y") == 0 || strcmp(key, "2") == 0) {
			LUA->PushNumber(vec1->y);
			return 1;
		} else if (strcmp(key, "z") == 0 || strcmp(key, "3") == 0) {
			LUA->PushNumber(vec1->z);
			return 1;
		}
	}

	LUA->GetMetaTable(1);
	LUA->Push(2);
	LUA->RawGet(-2);

	return 1;
}

LUA_FUNCTION_STATIC(Vector__newindex)
{
	LUA->CheckType(1, Type::Vector);

	const char* key = LUA->GetString(2);
	if (key == NULL)
		return 0;

	Vector* vec1 = Get_Vector(1);
	if (strcmp(key, "x") == 0 || strcmp(key, "1") == 0) {
		vec1->x = std::stoi(LUA->GetString(3));
	} else if (strcmp(key, "y") == 0 || strcmp(key, "2") == 0) {
		vec1->y = std::stoi(LUA->GetString(3));
	} else if (strcmp(key, "z") == 0 || strcmp(key, "3") == 0) {
		vec1->z = std::stoi(LUA->GetString(3));
	}

	return 0;
}

LUA_FUNCTION_STATIC(Vector__eq)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	LUA->PushBool(Get_Vector(1) == Get_Vector(2));

	return 1;
}

LUA_FUNCTION_STATIC(Vector__add)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	Push_Vector(new Vector(vec1->x + vec2->x, vec1->y + vec2->y, vec1->z + vec2->z));

	return 1;
}

LUA_FUNCTION_STATIC(Vector__sub)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	Vector* new_vec = new Vector(vec1 - vec2);
	Push_Vector(new_vec);

	return 1;
}

LUA_FUNCTION_STATIC(Vector__unm)
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec = Get_Vector(1);
	Push_Vector(new Vector(-vec->x, -vec->y, -vec->z));

	return 1;
}

LUA_FUNCTION_STATIC(Vector__mul)
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec1 = Get_Vector(1);

	if (LUA->IsType(2, Type::Number)) {
		int num = LUA->GetNumber(2);

		Push_Vector(new Vector(vec1->x * num, vec1->y * num, vec1->z * num));
	} else {
		LUA->CheckType(2, Type::Vector);

		Vector* vec2 = Get_Vector(2);
		Push_Vector(new Vector(vec1->x * vec2->x, vec1->y * vec2->y, vec1->z * vec2->z));
	}

	return 1;
}

LUA_FUNCTION_STATIC(Vector__div)
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	if (LUA->IsType(2, Type::Number)) {
		int num = LUA->GetNumber(2);

		Push_Vector(new Vector(vec1->x / num, vec1->y / num, vec1->z / num));
	} else {
		LUA->CheckType(2, Type::Vector);
		Vector* vec2 = Get_Vector(2);

		Push_Vector(new Vector(vec1->x / vec2->x, vec1->y / vec2->y, vec1->z / vec2->z));
	}

	return 1;
}

LUA_FUNCTION(Vector_Add)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	vec1->x += vec2->x;
	vec1->z += vec2->y;
	vec1->x += vec2->z;

	return 0;
}

LUA_FUNCTION(Vector_Length)
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec = Get_Vector(1);
	LUA->PushNumber(Vector(vec->x, vec->y, vec->z).Length());

	return 1;
}

LUA_FUNCTION(Vector_Sub)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	vec1->x -= vec2->x;
	vec1->z -= vec2->y;
	vec1->x -= vec2->z;

	return 0;
}

LUA_FUNCTION(Vector_Mul) // Undocumented: Second arg can be a Vector?
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	if (LUA->IsType(2, Type::Number)) {
		int num = LUA->GetNumber(2);

		vec1->x *= num;
		vec1->z *= num;
		vec1->x *= num;
	} else {
		LUA->CheckType(2, Type::Vector);
		Vector* vec2 = Get_Vector(2);

		vec1->x *= vec2->x;
		vec1->z *= vec2->y;
		vec1->x *= vec2->z;
	}

	return 0;
}

LUA_FUNCTION(Vector_Div) // Undocumented: Second arg can be a Vector?
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	if (LUA->IsType(2, Type::Number)) {
		int num = LUA->GetNumber(2);

		vec1->x /= num;
		vec1->z /= num;
		vec1->x /= num;
	} else {
		LUA->CheckType(1, Type::Vector);
		Vector* vec2 = Get_Vector(2);

		vec1->x /= vec2->x;
		vec1->z /= vec2->y;
		vec1->x /= vec2->z;
	}

	return 0;
}

LUA_FUNCTION(Vector_Normalize)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);
	
	Vector vvec = Vector(vec->x, vec->y, vec->z);
	VectorNormalize(vvec);

	vec->x = vvec.x;
	vec->y = vvec.y;
	vec->z = vvec.z;

	return 0;
}

LUA_FUNCTION(Vector_GetNormal)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);
	
	Vector* new_vec = new Vector(vec->x, vec->y, vec->z);
	VectorNormalize(*new_vec);

	Push_Vector(new_vec);

	return 1;
}

LUA_FUNCTION(Vector_Dot)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	LUA->PushNumber(Vector(vec1->x, vec1->y, vec1->z).Dot(Vector(vec2->x, vec2->y, vec2->z)));

	return 1;
}

LUA_FUNCTION(Vector_Cross)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* a = Get_Vector(1);
	Vector* b = Get_Vector(2);

	Push_Vector(new Vector(a->y*b->z - a->z*b->y, a->z*b->x - a->x*b->z, a->x*b->y - a->y*b->x));

	return 1;
}

LUA_FUNCTION(Vector_Distance)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	LUA->PushNumber(Vector(vec1->x, vec1->y, vec1->z).DistTo(Vector(vec2->x, vec2->y, vec2->z)));

	return 1;
}

LUA_FUNCTION(Vector_Angle)
{
	LUA->CheckType(1, Type::Vector);

	Vector* vec = Get_Vector(1);

	QAngle* ang = new QAngle();
	VectorAngles(Vector(vec->x, vec->y, vec->z), *ang);
	Push_Angle(ang);

	return 1;
}

LUA_FUNCTION(Vector_AngleEx)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);

	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	QAngle* ang = new QAngle();
	VectorAngles(Vector(vec1->x, vec1->y, vec1->z), Vector(vec2->x, vec2->y, vec2->z), *ang);
	Push_Angle(ang);

	return 1;
}

LUA_FUNCTION(Vector_Rotate)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Angle);

	Vector* vec = Get_Vector(1);
	QAngle* ang = Get_Angle(2);

	matrix3x4_t matrix;
	AngleMatrix(QAngle(ang->x, ang->y, ang->z), matrix);

	Vector out = Vector();
	VectorRotate(Vector(vec->x, vec->y, vec->z), matrix, out);

	vec->x = out.x;
	vec->y = out.y;
	vec->z = out.z;

	return 0;
}

LUA_FUNCTION(Vector_Length2D)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	LUA->PushNumber(Vector(vec->x, vec->y, vec->z).Length2D());

	return 1;
}

LUA_FUNCTION(Vector_LengthSqr)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	LUA->PushNumber(Vector(vec->x, vec->y, vec->z).LengthSqr());

	return 1;
}

LUA_FUNCTION(Vector_Length2DSqr)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	LUA->PushNumber(Vector(vec->x, vec->y, vec->z).Length2DSqr());

	return 1;
}

LUA_FUNCTION(Vector_DistToSqr)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);
	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	LUA->PushNumber(Vector(vec1->x, vec1->y, vec1->z).DistToSqr(Vector(vec2->x, vec2->y, vec2->z)));

	return 1;
}

LUA_FUNCTION(Vector_WithinAABox)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);
	LUA->CheckType(3, Type::Vector);
	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);
	Vector* vec3 = Get_Vector(3);

	LUA->PushBool(Vector(vec1->x, vec1->y, vec1->z).WithinAABox(Vector(vec2->x, vec2->y, vec2->z), Vector(vec3->x, vec3->y, vec3->z)));

	return 1;
}

LUA_FUNCTION(Vector_IsZero)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	LUA->PushBool(vec->x == 0 && vec->y == 0 && vec->z == 0);

	return 1;
}

LUA_FUNCTION(Vector_IsEqualTol)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);
	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);
	double tolerance = LUA->CheckNumber(3);

	LUA->PushBool(
		(abs(vec1->x - vec2->x) <= tolerance) &&
        (abs(vec1->y - vec2->y) <= tolerance) &&
        (abs(vec1->z - vec2->z) <= tolerance)
	);

	return 1;
}

LUA_FUNCTION(Vector_Zero)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	vec->x = 0;
	vec->y = 0;
	vec->z = 0;

	return 0;
}

LUA_FUNCTION(Vector_Set)
{
	LUA->CheckType(1, Type::Vector);
	LUA->CheckType(2, Type::Vector);
	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	vec1->x = vec2->x;
	vec1->y = vec2->y;
	vec1->z = vec2->z;

	return 0;
}

LUA_FUNCTION(Vector_Unpack)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	LUA->PushNumber(vec->x);
	LUA->PushNumber(vec->y);
	LUA->PushNumber(vec->z);

	return 3;
}

LUA_FUNCTION(Vector_SetUnpacked)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);
	double x = LUA->CheckNumber(2);
	double y = LUA->CheckNumber(3);
	double z = LUA->CheckNumber(4);

	vec->x = x;
	vec->y = y;
	vec->z = z;

	return 0;
}

LUA_FUNCTION(Vector_ToTable)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	LUA->CreateTable();
		LUA->PushNumber(vec->x);
		LUA->SetField(-2, "1");

		LUA->PushNumber(vec->y);
		LUA->SetField(-2, "2");

		LUA->PushNumber(vec->z);
		LUA->SetField(-2, "3");

	return 1;
}

LUA_FUNCTION(Vector_Random)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);
	int min = LUA->CheckNumber(2);
	int max = LUA->CheckNumber(3);

	vec->x = std::rand() % max + min;
	vec->y = std::rand() % max + min;
	vec->z = std::rand() % max + min;

	return 0;
}

LUA_FUNCTION(Vector_Negate)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	vec->x = -vec->x;
	vec->y = -vec->y;
	vec->z = -vec->z;

	return 0;
}

LUA_FUNCTION(Vector_GetNegated)
{
	LUA->CheckType(1, Type::Vector);
	Vector* vec = Get_Vector(1);

	Vector* new_vec = new Vector(-vec->x, -vec->y, -vec->z);
	Push_Vector(new_vec);

	return 1;
}

LUA_FUNCTION(Global_Vector)
{
	double x = LUA->CheckNumber(1);
	double y = LUA->CheckNumber(2);
	double z = LUA->CheckNumber(3);

	Push_Vector(new Vector(x, y, z));

	return 1;
}

LUA_FUNCTION(Global_LerpVector)
{
	double delta = LUA->GetNumber(1);
	Vector* start = Get_Vector(2);
	Vector* end = Get_Vector(3);

	Vector* vec = new Vector(Lerp(delta, start->x, end->x), Lerp(delta, start->y, end->y), Lerp(delta, start->z, end->z));
	Push_Vector(vec);

	return 1;
}

LUA_FUNCTION(Global_OrderVectors)
{
	Vector* vec1 = Get_Vector(1);
	Vector* vec2 = Get_Vector(2);

	if (vec1->x > vec2->x)
	{
		double x = vec1->x;
		vec1->x = vec2->x;
		vec2->x = x;
	}

	if (vec1->y > vec2->y)
	{
		double y = vec1->y;
		vec1->y = vec2->y;
		vec2->y = y;
	}

	if (vec1->z > vec2->z)
	{
		double z = vec1->z;
		vec1->z = vec2->z;
		vec2->z = z;
	}

	return 0;
}

extern CLuaClass vector_class;

struct Lua_Vector {
	const Vector* vec;
};
Vector* Get_Vector(int index)
{
	return (Vector*)vector_class.Get(index);
}

void Push_Vector(const Vector* vec)
{
	vector_class.Push((void*)vec);
}

void Vector_Class()
{
	g_Lua->PushSpecial(SPECIAL_GLOB);
		g_Lua->PushCFunction(Global_Vector);
		g_Lua->SetField(-2, "Vector");

		g_Lua->PushCFunction(Global_LerpVector);
		g_Lua->SetField(-2, "LerpVector");

		g_Lua->PushCFunction(Global_OrderVectors);
		g_Lua->SetField(-2, "OrderVectors");
	g_Lua->Pop(1);

	g_Lua->CreateMetaTableType("Vector", Type::Vector);
		g_Lua->PushCFunction(Vector__gc); // Gmod uses this format -> Vector____gc, Vector__Forward and so on.
		g_Lua->SetField(-2, "__gc");

		g_Lua->PushCFunction(Vector__index);
		g_Lua->SetField(-2, "__index");

		g_Lua->PushCFunction(Vector__newindex);
		g_Lua->SetField(-2, "__newindex");

		g_Lua->PushCFunction(Vector__tostring);
		g_Lua->SetField(-2, "__tostring");

		g_Lua->PushCFunction(Vector__eq);
		g_Lua->SetField(-2, "__eq");
		
		g_Lua->PushCFunction(Vector__add);
		g_Lua->SetField(-2, "__add");

		g_Lua->PushCFunction(Vector__sub);
		g_Lua->SetField(-2, "__sub");

		g_Lua->PushCFunction(Vector__unm);
		g_Lua->SetField(-2, "__unm");

		g_Lua->PushCFunction(Vector__mul);
		g_Lua->SetField(-2, "__mul");

		g_Lua->PushCFunction(Vector__div);
		g_Lua->SetField(-2, "__div");


		g_Lua->PushCFunction(Vector_Length);
		g_Lua->SetField(-2, "Length");

		g_Lua->PushCFunction(Vector_Add);
		g_Lua->SetField(-2, "Add");

		g_Lua->PushCFunction(Vector_Sub);
		g_Lua->SetField(-2, "Sub");

		g_Lua->PushCFunction(Vector_Mul);
		g_Lua->SetField(-2, "Mul");

		g_Lua->PushCFunction(Vector_Div);
		g_Lua->SetField(-2, "Div");

		g_Lua->PushCFunction(Vector_Normalize);
		g_Lua->SetField(-2, "Normalize");

		g_Lua->PushCFunction(Vector_GetNormal);
		g_Lua->SetField(-2, "GetNormal");

		g_Lua->PushCFunction(Vector_GetNormal);
		g_Lua->SetField(-2, "GetNormalized");

		g_Lua->PushCFunction(Vector_Dot);
		g_Lua->SetField(-2, "Dot");

		g_Lua->PushCFunction(Vector_Dot);
		g_Lua->SetField(-2, "DotProduct");

		g_Lua->PushCFunction(Vector_Cross);
		g_Lua->SetField(-2, "Cross");

		g_Lua->PushCFunction(Vector_Distance);
		g_Lua->SetField(-2, "Distance");

		g_Lua->PushCFunction(Vector_Angle);
		g_Lua->SetField(-2, "Angle");

		g_Lua->PushCFunction(Vector_AngleEx);
		g_Lua->SetField(-2, "AngleEx");

		g_Lua->PushCFunction(Vector_Rotate);
		g_Lua->SetField(-2, "Rotate");

		g_Lua->PushCFunction(Vector_Length2D);
		g_Lua->SetField(-2, "Length2D");

		g_Lua->PushCFunction(Vector_LengthSqr);
		g_Lua->SetField(-2, "LengthSqr");

		g_Lua->PushCFunction(Vector_Length2DSqr);
		g_Lua->SetField(-2, "Length2DSqr");

		g_Lua->PushCFunction(Vector_DistToSqr);
		g_Lua->SetField(-2, "DistToSqr");

		g_Lua->PushCFunction(Vector_WithinAABox);
		g_Lua->SetField(-2, "WithinAABox");

		g_Lua->PushCFunction(Vector_IsZero);
		g_Lua->SetField(-2, "IsZero");

		g_Lua->PushCFunction(Vector_IsEqualTol);
		g_Lua->SetField(-2, "IsEqualTol");

		g_Lua->PushCFunction(Vector_Zero);
		g_Lua->SetField(-2, "Zero");

		g_Lua->PushCFunction(Vector_Set);
		g_Lua->SetField(-2, "Set");

		g_Lua->PushCFunction(Vector_Unpack);
		g_Lua->SetField(-2, "Unpack");

		g_Lua->PushCFunction(Vector_SetUnpacked);
		g_Lua->SetField(-2, "SetUnpacked");

		g_Lua->PushCFunction(Vector_ToTable);
		g_Lua->SetField(-2, "ToTable");

		g_Lua->PushCFunction(Vector_Random);
		g_Lua->SetField(-2, "Random");

		g_Lua->PushCFunction(Vector_Negate);
		g_Lua->SetField(-2, "Negate");

		g_Lua->PushCFunction(Vector_GetNegated);
		g_Lua->SetField(-2, "GetNegated");
}

CLuaClass vector_class("Vector", Type::Vector, Vector_Class);