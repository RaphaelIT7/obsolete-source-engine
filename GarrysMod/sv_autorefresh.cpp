#include "cbase.h"
#include "sv_autorefresh.h"

void GarrysMod::AutoRefresh::EntityFilename( const std::string& fileName )
{
	// ToDo
}

std::string GarrysMod::AutoRefresh::ConvertLuaFilename( const std::string& fileName )
{
	// ToDo
	return "";
}

void GarrysMod::AutoRefresh::FindRootFile_( const std::string& fileName, CUtlVector<LuaFile*> vec )
{
	// ToDo
}

void GarrysMod::AutoRefresh::HandleLuaFileChange( const std::string& fileName )
{
	// ToDo
}

void GarrysMod::AutoRefresh::Init()
{
	// ToDo
}

void GarrysMod::AutoRefresh::HandleChange_Lua( const std::string& fileName, const std::string& unknown1, const std::string& unknown2 )
{
	// ToDo
}

void GarrysMod::AutoRefresh::HandleChange_VMT( const std::string& fileName, const std::string& unknown1, const std::string& unknown2 )
{
	// ToDo
	// Run mat_reloadmaterial
}

void GarrysMod::AutoRefresh::HandleChange_VTF( const std::string& fileName, const std::string& unknown1, const std::string& unknown2 )
{
	// ToDo
	// Run mat_reloadtexture
}

void GarrysMod::AutoRefresh::Cycle()
{
	// ToDo
}

void GarrysMod::AutoRefresh::HandleChange( std::string& fileName )
{
	// ToDo
}
