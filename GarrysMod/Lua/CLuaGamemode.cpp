#include "cbase.h"
#include "Externals.h"
#include "GarrysMod/Lua/LuaInterface.h"
#include "CLuaClass.h"
#include "vprof.h"

CLuaGamemode::CLuaGamemode()
{
	// ToDo
}

CLuaGamemode::~CLuaGamemode()
{
	// ToDo
}

void CLuaGamemode::LoadGamemode( const std::string& gamemode, bool reload )
{
	VPROF( "CLuaGamemode::LoadGamemode", VPROF_BUDGETGROUP_GMOD );

	// ToDo
}

void CLuaGamemode::ReloadGamemode()
{
	// ToDo
	// Call LoadGamemode with reload set to true.
}

bool CLuaGamemode::IsValidGamemode( const std::string& gamemode )
{
	// ToDo
	return true;
}

void CLuaGamemode::SetGamemode( const std::string& gamemode, bool unknown )
{
	VPROF( "CLuaGamemode::SetGamemode", VPROF_BUDGETGROUP_GMOD );

	// ToDo
}

bool CLuaGamemode::Call( const char* hook ) // Were gonna mainly use "::Call( int hook )"
{
	VPROF( "CLuaGamemode::Call", VPROF_BUDGETGROUP_GMOD );

	// ToDo

	return false;
}

void CLuaGamemode::DeriveGamemode( const std::string& gamemode )
{
	// ToDo
}

bool CLuaGamemode::Call( int hook ) // Unfinished! Gmod uses CLuaObjects for this.
{
	VPROF( "CLuaGamemode::Call", VPROF_BUDGETGROUP_GMOD );

	int reference = -1;
	g_Lua->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
		g_Lua->GetField( -1, "gamemode" );
		if (g_Lua->IsType( -1, GarrysMod::Lua::Type::Table ))
		{
			g_Lua->GetField( -1, "Call" );
			if ( g_Lua->IsType( -1, GarrysMod::Lua::Type::Function ))
			{
				reference = g_Lua->ReferenceCreate();
			} else {
				g_Lua->Pop( 2 );
			}
		}
	g_Lua->Pop( 2 );

	if ( reference != -1 )
	{
		g_Lua->ReferencePush( reference );
		g_Lua->ReferenceFree( reference );
		g_Lua->PushPooledString( hook );
		return true;
	}
	
	return false;
}

bool CLuaGamemode::CallWithArgs( const char* hook )  // Unfinished! Gmod uses CLuaObjects for this.
{
	VPROF( "CLuaGamemode::CallWithArgs", VPROF_BUDGETGROUP_GMOD );

	// ToDo

	return false;
}

bool CLuaGamemode::CallWithArgs( int hook ) // Unfinished! Gmod uses CLuaObjects for this.
{
	VPROF( "CLuaGamemode::CallWithArgs", VPROF_BUDGETGROUP_GMOD );

	if (!ThreadInMainThread())
		Error("[GM:CallWithArgs - !ThreadInMainThread] %s", g_Lua->GetPooledString(hook));

	int reference = -1;
	g_Lua->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
		g_Lua->GetField( -1, "gamemode" );
		if (g_Lua->IsType( -1, GarrysMod::Lua::Type::Table ))
		{
			g_Lua->GetField( -1, "Call" );
			if ( g_Lua->IsType( -1, GarrysMod::Lua::Type::Function ))
			{
				reference = g_Lua->ReferenceCreate();
			} else {
				g_Lua->Pop( 2 );
			}
		}
	g_Lua->Pop( 2 );

	if ( reference != -1 )
	{
		g_Lua->ReferencePush( reference );
		g_Lua->ReferenceFree( reference );
		g_Lua->PushPooledString( hook );
		return true;
	}
	
	return false;
}

void CLuaGamemode::CallFinish( int args ) // CallWithArgs pushes the function. This calls the function.
{
	VPROF( "CLuaGamemode::CallFinish", VPROF_BUDGETGROUP_GMOD );

	g_Lua->CallInternalNoReturns( args );
}

void CLuaGamemode::CallReturns( int args, int returns )
{
	g_Lua->CallInternal( args, returns );
}

void CLuaGamemode::LoadCurrentlyActiveGamemode()
{
	// ToDo
}