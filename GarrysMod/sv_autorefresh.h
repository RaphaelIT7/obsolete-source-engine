#include "GarrysMod/Lua/LuaShared.h"
#include "Bootil/Bootil.h"
#include "Externals.h"

namespace GarrysMod::AutoRefresh
{
	static Bootil::File::ChangeMonitor g_ChangeMonitor;
	extern void EntityFilename( const std::string& fileName );
	extern std::string ConvertLuaFilename( const std::string& fileName );
	extern void FindRootFile_( const std::string& fileName, CUtlVector<GarrysMod::Lua::LuaFile*> vec ); // ::File -> LuaFile
	extern void HandleLuaFileChange( const std::string& fileName );

	extern void Init();
	extern void HandleChange_Lua( const std::string& fileName, const std::string& unknown1, const std::string& unknown2 );
	extern void HandleChange_VMT( const std::string& fileName, const std::string& unknown1, const std::string& unknown2 );
	extern void HandleChange_VTF( const std::string& fileName, const std::string& unknown1, const std::string& unknown2 );
	extern void Cycle();
	extern void HandleChange( std::string& fileName );
}