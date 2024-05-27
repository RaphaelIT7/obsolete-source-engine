#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "tier1/strtools.h"

LUA_FUNCTION_STATIC(Angle__gc)
{
	if (!LUA->IsType(1, GarrysMod::Lua::Type::Angle))
		return 0;

	// ToDo: Free angle object.

	return 0;
}

LUA_FUNCTION_STATIC(Angle__tostring)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);
	char szBuf[64] = {};
	V_snprintf(szBuf, sizeof(szBuf),"%.3f %.3f %.3f", ang->x, ang->y, ang->z);
	LUA->PushString(szBuf);
	return 1;
}

LUA_FUNCTION_STATIC(Angle__index)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	const char* key = LUA->GetString(2);
	if (key != NULL) 
	{
		QAngle* ang = Get_Angle(1);
		if (strcmp(key, "p") == 0 || strcmp(key, "pitch") == 0 || strcmp(key, "x") == 0 || strcmp(key, "1") == 0) {
			LUA->PushNumber(ang->x);
			return 1;
		} else if (strcmp(key, "y") == 0 || strcmp(key, "yaw") == 0 || strcmp(key, "y") == 0 || strcmp(key, "2") == 0) {
			LUA->PushNumber(ang->y);
			return 1;
		} else if (strcmp(key, "r") == 0 || strcmp(key, "roll") == 0 || strcmp(key, "z") == 0 || strcmp(key, "3") == 0) {
			LUA->PushNumber(ang->z);
			return 1;
		}
	}

	LUA->GetMetaTable(1);
	LUA->Push(2);
	LUA->RawGet(-2);

	return 1;
}

LUA_FUNCTION_STATIC(Angle__newindex)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	const char* key = LUA->GetString(2);
	if (key == NULL)
		return 0;

	QAngle* ang = Get_Angle(1);
	if (strcmp(key, "p") == 0 || strcmp(key, "pitch") == 0 || strcmp(key, "x") == 0 || strcmp(key, "1") == 0) {
		ang->x = std::stoi(LUA->GetString(3));
	} else if (strcmp(key, "y") == 0 || strcmp(key, "yaw") == 0 || strcmp(key, "y") == 0 || strcmp(key, "2") == 0) {
		ang->y = std::stoi(LUA->GetString(3));
	} else if (strcmp(key, "r") == 0 || strcmp(key, "roll") == 0 || strcmp(key, "z") == 0 || strcmp(key, "3") == 0) {
		ang->z = std::stoi(LUA->GetString(3));
	}

	return 0;
}

LUA_FUNCTION_STATIC(Angle__eq)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

	LUA->PushBool(Get_Angle(1) == Get_Angle(2));
	return 1;
}

LUA_FUNCTION_STATIC(Angle__add)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

	QAngle* ang1 = Get_Angle(1);
	QAngle* ang2 = Get_Angle(2);

	Push_Angle(new QAngle(ang1->x + ang2->x, ang1->y + ang2->y, ang1->z + ang2->z));

	return 1;
}

LUA_FUNCTION_STATIC(Angle__sub)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

	QAngle* ang1 = Get_Angle(1);
	QAngle* ang2 = Get_Angle(2);

	Push_Angle(new QAngle(ang1->x - ang2->x, ang1->y - ang2->y, ang1->z - ang2->z));

	return 1;
}

LUA_FUNCTION_STATIC(Angle__unm)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);
	Push_Angle(new QAngle(-ang->x, -ang->y, -ang->z));

	return 1;
}

LUA_FUNCTION_STATIC(Angle__mul)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);
	int num = LUA->CheckNumber(2);
	Push_Angle(new QAngle(ang->x * num, ang->y * num, ang->z * num));

	return 1;
}

LUA_FUNCTION_STATIC(Angle__div)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);
	int num = LUA->CheckNumber(2);
	Push_Angle(new QAngle(ang->x / num, ang->y / num, ang->z / num));

	return 1;
}

LUA_FUNCTION_STATIC(Angle_Add)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

	QAngle* ang1 = Get_Angle(1);
	QAngle* ang2 = Get_Angle(2);

	ang1->x += ang2->x;
	ang1->y += ang2->y;
	ang1->z += ang2->z;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_Sub)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

	QAngle* ang1 = Get_Angle(1);
	QAngle* ang2 = Get_Angle(2);

	ang1->x -= ang2->x;
	ang1->y -= ang2->y;
	ang1->z -= ang2->z;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_Mul)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);
	int num = LUA->CheckNumber(2);
	ang->x *= num;
	ang->y *= num;
	ang->z *= num;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_Div)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);
	int num = LUA->CheckNumber(2);
	ang->x /= num;
	ang->y /= num;
	ang->z /= num;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_Forward)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	QAngle* ang = Get_Angle(1);

	Vector* forward = new Vector();
	AngleVectors(QAngle(ang->x, ang->y, ang->z), forward);

	Push_Vector(forward);

	return 1;
}

LUA_FUNCTION_STATIC(Angle_Right)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	QAngle* ang = Get_Angle(1);

	Vector* right = new Vector();
	AngleVectors(QAngle(ang->x, ang->y, ang->z), nullptr, right, nullptr);

	Push_Vector(right);

	return 1;
}

LUA_FUNCTION_STATIC(Angle_Up)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);

	Vector* up = new Vector();
	AngleVectors(QAngle(ang->x, ang->y, ang->z), nullptr, nullptr, up);

	Push_Vector(up);

	return 1;
}

LUA_FUNCTION_STATIC(Angle_RotateAroundAxis) // Verify this. Probably shit.
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

    QAngle* ang = Get_Angle(1);
    Vector* vec = Get_Vector(2);
    double number = LUA->CheckNumber(3);

    double radians = DEG2RAD(number);

    matrix3x4_t matrix;
    AngleMatrix(QAngle(ang->x, ang->y, ang->z), matrix);

    Vector rotated;
    VectorIRotate(&vec->x, matrix, &rotated.x);

    Quaternion rotation;
    AxisAngleQuaternion(rotated, radians, rotation);

    matrix3x4_t matrix2;
    QuaternionMatrix(rotation, matrix2);

    QAngle new_ang;
    MatrixAngles(matrix2, new_ang);

    ang->x = new_ang.x;
    ang->y = new_ang.y;
    ang->z = new_ang.z;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_Normalize)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	QAngle* ang = Get_Angle(1);

	ang->x = AngleNormalize(ang->x);
	ang->y = AngleNormalize(ang->y);
	ang->z = AngleNormalize(ang->z);

	return 0;
}

LUA_FUNCTION_STATIC(Angle_Set)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

	QAngle* ang1 = Get_Angle(1);
	QAngle* ang2 = Get_Angle(2);

	ang1->x = ang2->x;
	ang1->y = ang2->y;
	ang1->z = ang2->z;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_Zero)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	QAngle* ang = Get_Angle(1);

	ang->x = 0;
	ang->y = 0;
	ang->z = 0;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_IsZero)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	QAngle* ang = Get_Angle(1);

	LUA->PushBool(ang->x == 0 && ang->y == 0 && ang->z == 0);

	return 1;
}

LUA_FUNCTION_STATIC(Angle_Unpack)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	QAngle* ang = Get_Angle(1);

	LUA->PushNumber(ang->x);
	LUA->PushNumber(ang->y);
	LUA->PushNumber(ang->z);

	return 3;
}

LUA_FUNCTION_STATIC(Angle_Random)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	QAngle* ang = Get_Angle(1);

	int min = LUA->IsType(2, GarrysMod::Lua::Type::Number) ? LUA->GetNumber(2) : -360;
	int max = LUA->IsType(3, GarrysMod::Lua::Type::Number) ? LUA->GetNumber(3) : 360;

	ang->x = std::rand() % max + min;
	ang->y = std::rand() % max + min;
	ang->z = std::rand() % max + min;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_SetUnpacked)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);

	QAngle* ang = Get_Angle(1);
	double x = LUA->CheckNumber(2);
	double y = LUA->CheckNumber(3);
	double z = LUA->CheckNumber(4);

	ang->x = x;
	ang->y = y;
	ang->z = z;

	return 0;
}

LUA_FUNCTION_STATIC(Angle_IsEqualTol)
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);

	QAngle* ang1 = Get_Angle(1);
	QAngle* ang2 = Get_Angle(2);
	double tolerance = LUA->CheckNumber(3);

	LUA->PushBool(
		(abs(ang1->x - ang2->x) <= tolerance) &&
        (abs(ang1->y - ang2->y) <= tolerance) &&
        (abs(ang1->z - ang2->z) <= tolerance)
	);

	return 1;
}

LUA_FUNCTION_STATIC(Global_Angle)
{
	double x = LUA->CheckNumber(1);
	double y = LUA->CheckNumber(2);
	double z = LUA->CheckNumber(3);

	Push_Angle(new QAngle(x, y, z));

	return 1;
}

LUA_FUNCTION_STATIC(Global_LerpAngle)
{
	LUA->CheckType(2, GarrysMod::Lua::Type::Angle);
	LUA->CheckType(3, GarrysMod::Lua::Type::Angle);

	double delta = LUA->GetNumber(1);
	QAngle* start = Get_Angle(2);
	QAngle* end = Get_Angle(3);

	Push_Angle(new QAngle(Lerp(delta, start->x, end->x), Lerp(delta, start->y, end->y), Lerp(delta, start->z, end->z)));

	return 1;
}

extern CLuaClass angle_class;
QAngle* Get_Angle(int index)
{
	return (QAngle*)angle_class.Get(index);
}

void Push_Angle(const QAngle* ang)
{
	angle_class.Push((void*)ang);
}

void Angle_Class()
{
	g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->PushCFunction(Global_Angle);
		g_Lua->SetField(-2, "Angle");

		g_Lua->PushCFunction(Global_LerpAngle);
		g_Lua->SetField(-2, "LerpAngle");
	g_Lua->Pop(1);

	g_Lua->CreateMetaTableType("Angle", GarrysMod::Lua::Type::Angle);
		g_Lua->PushCFunction(Angle__gc); // Gmod uses this format -> Angle____gc, Angle__Forward and so on.
		g_Lua->SetField(-2, "__gc");

		g_Lua->PushCFunction(Angle__index);
		g_Lua->SetField(-2, "__index");

		g_Lua->PushCFunction(Angle__newindex);
		g_Lua->SetField(-2, "__newindex");

		g_Lua->PushCFunction(Angle__tostring);
		g_Lua->SetField(-2, "__tostring");

		g_Lua->PushCFunction(Angle__eq);
		g_Lua->SetField(-2, "_eq");
		
		g_Lua->PushCFunction(Angle__add);
		g_Lua->SetField(-2, "__add");

		g_Lua->PushCFunction(Angle__sub);
		g_Lua->SetField(-2, "__sub");

		g_Lua->PushCFunction(Angle__unm);
		g_Lua->SetField(-2, "__unm");

		g_Lua->PushCFunction(Angle__mul);
		g_Lua->SetField(-2, "__mul");

		g_Lua->PushCFunction(Angle__div);
		g_Lua->SetField(-2, "__div");


		g_Lua->PushCFunction(Angle_Add);
		g_Lua->SetField(-2, "Add");

		g_Lua->PushCFunction(Angle_Sub);
		g_Lua->SetField(-2, "Sub");

		g_Lua->PushCFunction(Angle_Mul);
		g_Lua->SetField(-2, "Mul");

		g_Lua->PushCFunction(Angle_Div);
		g_Lua->SetField(-2, "Div");

		g_Lua->PushCFunction(Angle_Forward);
		g_Lua->SetField(-2, "Forward");

		g_Lua->PushCFunction(Angle_Right);
		g_Lua->SetField(-2, "Right");

		g_Lua->PushCFunction(Angle_RotateAroundAxis);
		g_Lua->SetField(-2, "RotateAroundAxis");

		g_Lua->PushCFunction(Angle_Normalize);
		g_Lua->SetField(-2, "Normalize");

		g_Lua->PushCFunction(Angle_Set);
		g_Lua->SetField(-2, "Set");

		g_Lua->PushCFunction(Angle_Zero);
		g_Lua->SetField(-2, "Zero");

		g_Lua->PushCFunction(Angle_IsZero);
		g_Lua->SetField(-2, "IsZero");

		g_Lua->PushCFunction(Angle_Unpack);
		g_Lua->SetField(-2, "Unpack");

		g_Lua->PushCFunction(Angle_Random);
		g_Lua->SetField(-2, "Random");

		g_Lua->PushCFunction(Angle_SetUnpacked);
		g_Lua->SetField(-2, "SetUnpacked");

		g_Lua->PushCFunction(Angle_IsEqualTol);
		g_Lua->SetField(-2, "IsEqualTol");
}

CLuaClass angle_class("Angle", GarrysMod::Lua::Type::Angle, Angle_Class);