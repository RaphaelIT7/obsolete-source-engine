#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"

LUA_FUNCTION_STATIC( global_CurTime )
{
	LUA->PushNumber(gpGlobals->curtime);
	return 1;
}

LUA_FUNCTION_STATIC( global_SysTime )
{
	LUA->PushNumber(Plat_FloatTime());
	return 1;
}

LUA_FUNCTION_STATIC( global_FrameTime )
{
	LUA->PushNumber(gpGlobals->frametime);
	return 1;
}

void Global_Library()
{
	g_Lua->PushSpecial( SPECIAL_GLOB );

		g_Lua->PushCFunction( global_CurTime );
		g_Lua->SetField( -2, "CurTime" );

		g_Lua->PushCFunction( global_FrameTime );
		g_Lua->SetField( -2, "FrameTime" );

		g_Lua->PushCFunction( global_SysTime );
		g_Lua->SetField( -2, "SysTime" );

	g_Lua->Pop( 1 );
}

CLuaLibrary LL_Global( "global", Global_Library );