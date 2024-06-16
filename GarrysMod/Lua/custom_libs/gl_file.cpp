#include "cbase.h"
#include "Lua/CLuaClass.h"
#include "GarrysMod/Lua/Interface.h"
#include "Externals.h"
#include "Bootil/Bootil.h"
#include "filesystem.h"

void ReplaceAllCharacters(std::string& unknown1, std::string unknown2, std::string unknown3)
{
	// ToDo
}

bool IsFileAccessAllowed(const std::string& file, const std::string& mode, bool unknown1, bool unknown2) // Manages all file access. Whitelist and blacklist
{
	// ToDo

	// 

	return true;
}

namespace Lua
{
	extern CLuaClass LC_File;
	struct Lua_File // Idk how gmod handles this class :/
	{
		FileHandle_t handle;
		const char* filename;
	};

	LUA_FUNCTION_STATIC(File__tostring)
	{
		Lua_File* file = (Lua_File*)LC_File.Get(1);
		char szBuf[270] = {};
		V_snprintf(szBuf, sizeof(szBuf),"File [%s]", file->handle ? file->filename : "NULL");
		LUA->PushString(szBuf);
		return 1;
	}

	void File_Class()
	{
	
	}
	CLuaClass LC_File("File", GarrysMod::Lua::Type::File, File_Class);
}

namespace GarrysMod::Lua::Libraries::File
{
	void AsyncCycle()
	{
		// ToDo
	}
}

void File_Library()
{
	g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->CreateTable();
			//g_Lua->PushCFunction(ents_Create);
			//g_Lua->SetField(-2, "Create");

		g_Lua->SetField(-2, "file");
	g_Lua->Pop(1);
}

CLuaLibrary file_library("file", File_Library);