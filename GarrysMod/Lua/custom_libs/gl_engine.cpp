#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#ifdef GAME_DLL
#include "enginecallback.h"
#endif

#ifdef GAME_DLL
LUA_FUNCTION_STATIC( engine_LightStyle )
{
	int lightStyle = LUA->CheckNumber( 1 );
	const char* pattern = LUA->CheckString( 2 );
	engine->LightStyle( lightStyle, pattern );

	return 0;
}
#endif

void Engine_Library()
{
	g_Lua->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
		g_Lua->CreateTable();
#ifdef GAME_DLL // Serverside only.
			g_Lua->PushCFunction( engine_LightStyle );
			g_Lua->SetField( -2, "LightStyle" );
#endif

		g_Lua->SetField( -2, "engine" );

	g_Lua->Pop( 1 );
}

CLuaLibrary LL_Engine( "engine", Engine_Library );