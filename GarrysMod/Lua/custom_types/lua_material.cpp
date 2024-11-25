// ==========================================================================================================
// A Material object. It represents a game material, similarly to how a .vmt
// file does. It can be created with Material or CreateMaterial.
// ==========================================================================================================

#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "tier1/strtools.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialvar.h"
#include "materialsystem/itexture.h"
#include "mathlib/vector.h"
#include <Lua/LuaHelper.h>

LUA_FUNCTION_STATIC(Material__CreateMaterial) {
	LUA->CheckType(1, Type::String);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::Table);

	uint nameLen;
	const char* name = LUA->GetString(1, &nameLen);
	const char* shader = LUA->GetString(2);

	char* matname = new char[nameLen + 1];
	matname[0] = '!';
	strcpy(matname + 1, name);

	KeyValues* kvp = new KeyValues(shader); // ????
	CLuaObject obj;
	obj.Init(LUA);
	obj.SetFromStack(3);
	LuaHelper::TableToKeyValues(&obj, kvp);

	IMaterial* mat = g_pMaterialSystem->CreateMaterial(matname, kvp);
	//delete matname; // ? is this needed here?

	Push_Material(mat);

	return 1;
}

LUA_FUNCTION_STATIC(Material__index) {
	LUA->CheckType(1, Type::Material);
	if (!LUA->FindOnObjectsMetaTable(1, 2)) LUA->PushNil();
	return 1;
}

bool GetVarFromMaterialAtLuaStack(ILuaInterface* LUA, int whereIsMaterial,
								  const char* varname, IMaterialVar** outVar) {
	IMaterial* mat = Get_Material(whereIsMaterial);
	Assert(mat != NULL);
	if (mat == NULL) return false;

	bool found;
	IMaterialVar* var = mat->FindVar(varname, &found);
	Assert(var != NULL);
	if (found == false || var == NULL) return false;

	*outVar = var;
	return true;
}

bool GetVarFromMaterialAtLuaStack(ILuaInterface* LUA, int whereIsMaterial,
								  int whereIsString, IMaterialVar** outVar) {
	IMaterial* mat = Get_Material(whereIsMaterial);
	if (mat == NULL) return false;

	const char* varname = LUA->GetString(whereIsString);
	if (varname == NULL) return false;

	bool found;
	IMaterialVar* var = mat->FindVar(varname, &found);
	Assert(var != NULL);
	if (found == false || var == NULL) return false;

	*outVar = var;
	return true;
}

// ==========================================================================================================
// Returns the color of the specified pixel of the $basetexture, only works for
// materials created from PNG files. Basically identical to ITexture:GetColor
// used on IMaterial:GetTexture( "$basetexture" ).

// **ARGUMENTS:**
// 	 number x
// 	 number y

// **RETURNS:**
// 	 Color
// ==========================================================================================================
/*LUA_FUNCTION(Material_GetColor) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::Number);
	LUA->CheckType(3, Type::Number);

	IMaterial* self = Get_Material(1);
	double x = LUA->GetNumber(2);
	double y = LUA->GetNumber(3);


	LUA->PushColor(self->);

	return 1;
	}
*/

// ==========================================================================================================
// Returns the specified material value as a float, or nil if the value is not
// set.

// **ARGUMENTS:**
// 	 string materialFloat

// **RETURNS:**
// 	 number
// ==========================================================================================================
LUA_FUNCTION(Material_GetFloat) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		LUA->PushNil();
	} else {
		LUA->PushNumber(var->GetFloatValue());
	}

	return 1;
}

// ==========================================================================================================
// Returns the specified material value as a int, rounds the value if its a
// float, or nil if the value is not set. Please note that certain material
// flags such as $model are stored in the $flags variable and cannot be directly
// retrieved with this function. See the full list here: Material Flags

// **ARGUMENTS:**
// 	 string materialInt

// **RETURNS:**
// 	 number
// ==========================================================================================================
LUA_FUNCTION(Material_GetInt) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		LUA->PushNil();
	} else {
		LUA->PushNumber(var->GetIntValue());
	}

	return 1;
}

// ==========================================================================================================
// Gets all the key values defined for the material.

// **RETURNS:**
// 	 table
// ==========================================================================================================
LUA_FUNCTION(Material_GetKeyValues) {
	LUA->CheckType(1, Type::Material);

	IMaterial* self = Get_Material(1);

	/* User code here */

	LUA->CreateTable();
	/* Fill in the table!!! */

	return 1;
}

// ==========================================================================================================
// Returns the specified material matrix as a int, or nil if the value is not
// set or is not a matrix.

// **ARGUMENTS:**
// 	 string materialMatrix

// **RETURNS:**
// 	 VMatrix
// ==========================================================================================================
LUA_FUNCTION(Material_GetMatrix) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	// TODO: Implement

	return 1;
}

// ==========================================================================================================
// Returns the name of the material, in most cases the path.

// **RETURNS:**
// 	 string
// ==========================================================================================================
LUA_FUNCTION(Material_GetName) {
	LUA->CheckType(1, Type::Material);

	IMaterial* self = Get_Material(1);
	LUA->PushString(self->GetName());

	return 1;
}

// ==========================================================================================================
// Returns the name of the materials shader.
// This function does not work serverside on Linux SRCDS. Issue Tracker: 3256

// **RETURNS:**
// 	 string
// ==========================================================================================================
LUA_FUNCTION(Material_GetShader) {
	LUA->CheckType(1, Type::Material);

	IMaterial* self = Get_Material(1);
	LUA->PushString(self->GetShaderName());

	return 1;
}

// ==========================================================================================================
// Returns the specified material string, or nil if the value is not set or if
// the value can not be converted to a string.

// **ARGUMENTS:**
// 	 string materialString

// **RETURNS:**
// 	 string
// ==========================================================================================================
LUA_FUNCTION(Material_GetString) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		LUA->PushNil();
	} else {
		LUA->PushString(var->GetStringValue());
	}

	return 1;
}

// ==========================================================================================================
// Returns an ITexture based on the passed shader parameter.

// **ARGUMENTS:**
// 	 string param

// **RETURNS:**
// 	 ITexture
// ==========================================================================================================
LUA_FUNCTION(Material_GetTexture) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		LUA->PushNil();
	} else {
		Push_Texture(var->GetTextureValue());
	}

	return 1;
}

// ==========================================================================================================
// Returns the specified material vector, or nil if the value is not set.
// See also IMaterial:GetVectorLinear

// **ARGUMENTS:**
// 	 string materialVector

// **RETURNS:**
// 	 Vector
// ==========================================================================================================
LUA_FUNCTION(Material_GetVector) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		LUA->PushNil();
	} else {
		const float* vec = var->GetVecValue();
		Push_Vector(new Vector(vec[0], vec[1], vec[2]));
	}

	return 1;
}

// ==========================================================================================================
// Returns the specified material vector as a 4 component vector.

// **ARGUMENTS:**
// 	 string name

// **RETURNS:**
// 	 number
// 	 number
// 	 number
// 	 number
// ==========================================================================================================
LUA_FUNCTION(Material_GetVector4D) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		LUA->PushNil();
		LUA->PushNil();
		LUA->PushNil();
		LUA->PushNil();
	} else {
		const float* vec = var->GetVecValue();
		LUA->PushNumber(vec[0]);
		LUA->PushNumber(vec[1]);
		LUA->PushNumber(vec[2]);
		LUA->PushNumber(vec[3]);
	}

	return 4;
}

// ==========================================================================================================
// Returns the specified material linear color vector, or nil if the value is
// not set. See https://en. wikipedia. org/wiki/Gamma_correction See also
// IMaterial:GetVector

// **ARGUMENTS:**
// 	 string materialVector

// **RETURNS:**
// 	 Vector
// ==========================================================================================================
LUA_FUNCTION(Material_GetVectorLinear) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	// TODO: implement!
	LUA->PushNil();
	return 1;
}

// ==========================================================================================================
// Returns the height of the member texture set for $basetexture.

// **RETURNS:**
// 	 number
// ==========================================================================================================
LUA_FUNCTION(Material_Height) {
	LUA->CheckType(1, Type::Material);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, "$basetexture", &var)) {
		LUA->PushNil();
	} else {
		LUA->PushNumber(var->GetTextureValue()->GetActualHeight());
	}

	return 1;
}

// ==========================================================================================================
// Returns whenever the material is valid, i. e. whether it was not loaded
// successfully from disk or not.

// **RETURNS:**
// 	 boolean
// ==========================================================================================================
LUA_FUNCTION(Material_IsError) {
	LUA->CheckType(1, Type::Material);

	IMaterial* self = Get_Material(1);
	LUA->PushBool(self->IsErrorMaterial());

	return 1;
}

// ==========================================================================================================
// Recomputes the material's snapshot. This needs to be called if you have
// changed variables on your material and it isn't changing. Be careful though -
// this function is slow - so try to call it only when needed!
// ==========================================================================================================
LUA_FUNCTION(Material_Recompute) {
	LUA->CheckType(1, Type::Material);

	IMaterial* self = Get_Material(1);
	// TODO: implement
	return 0;
}

// ==========================================================================================================
// Sets the specified material float to the specified float, does nothing on a
// type mismatch.

// **ARGUMENTS:**
// 	 string materialFloat
// 	 number float
// ==========================================================================================================
LUA_FUNCTION(Material_SetFloat) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::Number);

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		var->SetFloatValue(LUA->GetNumber(3));
	}

	return 0;
}

// ==========================================================================================================
// Sets the specified material value to the specified int, does nothing on a
// type mismatch. Please note that certain material flags such as $model are
// stored in the $flags variable and cannot be directly set with this function.
// See the full list here: Material Flags

// **ARGUMENTS:**
// 	 string materialInt
// 	 number int
// ==========================================================================================================
LUA_FUNCTION(Material_SetInt) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::Number);

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		var->SetIntValue((int)LUA->GetNumber(3));
	}

	return 0;
}

// ==========================================================================================================
// Sets the specified material value to the specified matrix, does nothing on a
// type mismatch.

// **ARGUMENTS:**
// 	 string materialMatrix
// 	 VMatrix matrix
// ==========================================================================================================
LUA_FUNCTION(Material_SetMatrix) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::Matrix);

	// TODO: Implement

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		// var->SetMatrixValue(Get_Matrix(3));
	}

	return 0;
}

// ==========================================================================================================
// We advise against using this. It may be changed or removed in a future
// update.
//
// This function does nothing
// The functionality of this function was removed due to the amount of crashes
// it caused.

// **ARGUMENTS:**
// 	 string shaderName
// ==========================================================================================================
LUA_FUNCTION(Material_SetShader) { return 0; }

// ==========================================================================================================
// Sets the specified material value to the specified string, does nothing on a
// type mismatch.

// **ARGUMENTS:**
// 	 string materialString
// 	 string string
// ==========================================================================================================
LUA_FUNCTION(Material_SetString) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::String);

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		var->SetStringValue(LUA->GetString(3));
	}

	return 0;
}

// ==========================================================================================================
// Sets the specified material texture to the specified texture, does nothing on
// a type mismatch.

// **ARGUMENTS:**
// 	 string materialTexture
// 	 ITexture texture
// ==========================================================================================================
LUA_FUNCTION(Material_SetTexture) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::Texture);

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		var->SetTextureValue(Get_Texture(3));
	}

	return 0;
}

// ==========================================================================================================
// Unsets the value for the specified material value.

// **ARGUMENTS:**
// 	 string materialValueName
// ==========================================================================================================
LUA_FUNCTION(Material_SetUndefined) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		var->SetUndefined();
	}

	return 0;
}

// ==========================================================================================================
// Sets the specified material vector to the specified vector, does nothing on a
// type mismatch.

// **ARGUMENTS:**
// 	 string MaterialVector
// 	 Vector vec
// ==========================================================================================================
LUA_FUNCTION(Material_SetVector) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::Vector);

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		const float* vec = Get_Vector(3)->Base();
		var->SetVecValue(vec, 3);
	}

	return 0;
}

// ==========================================================================================================
// Sets the specified material vector to the specified 4 component vector, does
// nothing on a type mismatch.

// **ARGUMENTS:**
// 	 string name
// 	 number x
// 	 number y
// 	 number z
// 	 number w
// ==========================================================================================================
LUA_FUNCTION(Material_SetVector4D) {
	LUA->CheckType(1, Type::Material);
	LUA->CheckType(2, Type::String);
	LUA->CheckType(3, Type::Number);
	LUA->CheckType(4, Type::Number);
	LUA->CheckType(5, Type::Number);
	LUA->CheckType(6, Type::Number);

	IMaterialVar* var;
	if (GetVarFromMaterialAtLuaStack(LUA, 1, 2, &var)) {
		double x = LUA->GetNumber(3);
		double y = LUA->GetNumber(4);
		double z = LUA->GetNumber(5);
		double w = LUA->GetNumber(6);
		var->SetVecValue(x, y, z, w);
	}

	return 0;
}

// ==========================================================================================================
// Returns the width of the member texture set for $basetexture.

// **RETURNS:**
// 	 number
// ==========================================================================================================
LUA_FUNCTION(Material_Width) {
	LUA->CheckType(1, Type::Material);

	IMaterialVar* var;
	if (!GetVarFromMaterialAtLuaStack(LUA, 1, "$basetexture", &var)) {
		LUA->PushNil();
	} else {
		LUA->PushNumber(var->GetTextureValue()->GetActualWidth());
	}

	return 1;
}

extern CLuaClass material_class;

IMaterial* Get_Material(int index) {
	return (IMaterial*)material_class.Get(index);
}
void Push_Material(const IMaterial* self) { material_class.Push((void*)self); }

void Material_Class() {
	g_Lua->PushSpecial(SPECIAL_GLOB);
	g_Lua->PushCFunction(Material__CreateMaterial);
	g_Lua->SetField(-2, "CreateMaterial");
	g_Lua->Pop();

	g_Lua->CreateMetaTableType("IMaterial", Type::Material);

	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material__index, "__index");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetFloat, "GetFloat");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetInt, "GetInt");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetKeyValues, "GetKeyValues");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetMatrix, "GetMatrix");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetName, "GetName");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetShader, "GetShader");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetString, "GetString");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetTexture, "GetTexture");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetVector, "GetVector");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetVector4D, "GetVector4D");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_GetVectorLinear,
								"GetVectorLinear");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_Height, "Height");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_IsError, "IsError");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_Recompute, "Recompute");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetFloat, "SetFloat");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetInt, "SetInt");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetMatrix, "SetMatrix");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetShader, "SetShader");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetString, "SetString");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetTexture, "SetTexture");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetUndefined, "SetUndefined");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetVector, "SetVector");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_SetVector4D, "SetVector4D");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Material_Width, "Width");
}

CLuaClass material_class("IMaterial", Type::Material, Material_Class);