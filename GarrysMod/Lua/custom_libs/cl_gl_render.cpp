#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#ifdef CLIENT_DLL
#include "cdll_client_int.h"
#endif

LUA_FUNCTION_STATIC( render_RedownloadAllLightmaps )
{
#ifdef CLIENT_DLL
	engine->GMOD_R_RedownloadAllLightmaps( true ); // ToDo: What does it do with the given args?
#endif

	return 0;
}

void Render_Library()
{
	g_Lua->PushSpecial( SPECIAL_GLOB );
		g_Lua->CreateTable();
			g_Lua->PushCFunction( render_RedownloadAllLightmaps );
			g_Lua->SetField( -2, "RedownloadAllLightmaps");

		g_Lua->SetField( -2, "render" );

	g_Lua->Pop(1);
}

CLuaLibrary LL_Render( "render", Render_Library );