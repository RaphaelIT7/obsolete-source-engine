#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "tier1/strtools.h"
#include "materialsystem/itexture.h"

LUA_FUNCTION_STATIC(Texture__index) {
	LUA->CheckType(1, Type::Texture);
	if (!LUA->FindOnObjectsMetaTable(-2, -1)) LUA->PushNil();
	return 1;
}

LUA_FUNCTION(Texture_Download) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	tex->Download();

	return 0;
}
LUA_FUNCTION(Texture_GetColor) {
	LUA->CheckType(1, Type::Texture);
	LUA->CheckType(2, Type::Number);
	LUA->CheckType(3, Type::Number);

	// TODO: implement

	return 1;
}
LUA_FUNCTION(Texture_GetMappingWidth) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushNumber(tex->GetMappingWidth());

	return 1;
}
LUA_FUNCTION(Texture_GetMappingHeight) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushNumber(tex->GetMappingHeight());

	return 1;
}
LUA_FUNCTION(Texture_GetName) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushString(tex->GetName());

	return 1;
}
LUA_FUNCTION(Texture_GetNumAnimationFrames) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushNumber(tex->GetNumAnimationFrames());

	return 1;
}
LUA_FUNCTION(Texture_IsError) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushBool(tex->IsError());

	return 1;
}
LUA_FUNCTION(Texture_IsErrorTexture) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushBool(tex->GetName() == "debug/debugempty");

	return 1;
}
LUA_FUNCTION(Texture_Width) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushNumber(tex->GetActualWidth());

	return 1;
}
LUA_FUNCTION(Texture_Height) {
	LUA->CheckType(1, Type::Texture);

	ITexture* tex = Get_Texture(1);
	LUA->PushNumber(tex->GetActualHeight());

	return 1;
}

extern CLuaClass texture_class;

ITexture* Get_Texture(int index) { return (ITexture*)texture_class.Get(index); }

void Push_Texture(const ITexture* tex) { texture_class.Push((void*)tex); }

void Texture_Class() {
	g_Lua->CreateMetaTableType("ITexture", Type::Texture);

	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_Download, "Download");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_GetColor, "GetColor");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_GetMappingWidth,
								"GetMappingWidth");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_GetMappingHeight,
								"GetMappingHeight");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_GetName, "GetName");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_GetNumAnimationFrames,
								"GetNumAnimationFrames");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_IsError, "IsError");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_IsErrorTexture,
								"IsErrorTexture");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_Width, "Width");
	LUA_PUSH_METATABLE_FUNCTION(g_Lua, Texture_Height, "Height");

	g_Lua->Pop(1);
}

CLuaClass texture_class("ITexture", Type::Texture, Texture_Class);