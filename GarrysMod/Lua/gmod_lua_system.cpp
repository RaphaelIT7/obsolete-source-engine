#include "cbase.h"
#include "igamesystem.h"
#include "gmod_lua.h"

class CLuaGameSystem : public CAutoGameSystem
{
	virtual void LevelInitPreEntity()
	{
		Lua::Create();
		Lua::OnLoaded();
	}
};
CLuaGameSystem g_LuaGameSystem;