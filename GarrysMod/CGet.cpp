#include "GarrysMod/IGet.h"
#include <GarrysMod/Lua/LuaShared.h>
#include <GarrysMod/Lua/LuaConVars.h>
#include <GarrysMod/IMenuSystem.h>
#include <GarrysMod/IntroScreen.h>
#include <materialsystem/imaterialsystemstub.h>
#include <GarrysMod/IResources.h>
#include <GarrysMod/IGMHTML.h>
#include <GarrysMod/IGmod_Audio.h>
#include <GarrysMod/IServerAddons.h>
#include "server.h"
#include <sys_dll.h>
#include "host.h"

class CGet : public IGet
{
public:
	virtual void OnLoadFailed( const char* reason );
	virtual const char* GameDir();
	virtual bool IsDedicatedServer();
	virtual int GetClientCount();
	virtual IFileSystem* FileSystem();
	virtual GarrysMod::Lua::ILuaShared* LuaShared();
	virtual GarrysMod::Lua::ILuaConVars* LuaConVars();
	virtual IMenuSystem* MenuSystem();
	virtual IResources* Resources();
	virtual IIntroScreen* IntroScreen();
	virtual IMaterialSystem* Materials();
	virtual IGMHTML* HTML();
	virtual IServerAddons* ServerAddons();
	virtual ISteamHTTP* SteamHTTP();
	virtual ISteamRemoteStorage* SteamRemoteStorage();
	virtual ISteamUtils* SteamUtils();
	virtual ISteamApps* SteamApps();
	virtual ISteamScreenshots* SteamScreenshots();
	virtual ISteamUser* SteamUser();
	virtual ISteamFriends* SteamFriends();
	virtual ISteamUGC* SteamUGC();
	virtual ISteamGameServer* SteamGameServer();
	virtual ISteamNetworking* SteamNetworking();
	virtual void Initialize( IFileSystem* );
	virtual void ShutDown();
	virtual void RunSteamCallbacks();
	virtual void ResetSteamAPIs();
	virtual void SetMotionSensor( IMotionSensor* );
	virtual IMotionSensor* MotionSensor();
	virtual int Version();
	virtual const char* VersionStr();
	virtual IGMod_Audio* Audio();
	virtual const char* VersionTimeStr();
	virtual IAnalytics* Analytics();
	virtual void UpdateRichPresense( const char* status );
	virtual void ResetRichPresense();
	virtual void FilterText(const char*, char*, int, ETextFilteringContext, CSteamID);
private:
	IFileSystem* m_pfilesystem;
	GarrysMod::Lua::ILuaShared* m_pluashared;
	GarrysMod::Lua::ILuaConVars* m_pluaconvars;
	IMenuSystem* m_pmenusystem;
	IResources* m_presources;
	IIntroScreen* m_pintroscreen;
	IMaterialSystem* m_pmaterials;
	IGMHTML* m_phtml;
	IServerAddons* m_pserveraddons;
	IMotionSensor* m_pmotionsensor;
	IGMod_Audio* m_paudio;
	IAnalytics* m_panalytics;
};

CGet cget;
IGet *get = &cget;

void CGet::OnLoadFailed( const char* reason )
{
	// ToDo
}

const char* CGet::GameDir()
{
	return ""; // ToDo
}

bool CGet::IsDedicatedServer()
{
	return ServerAddons() != nullptr;
}

int CGet::GetClientCount()
{
	return sv.GetNumClients();
}

IFileSystem* CGet::FileSystem()
{
	return m_pfilesystem;
}

GarrysMod::Lua::ILuaShared* CGet::LuaShared()
{
	return m_pluashared;
}

GarrysMod::Lua::ILuaConVars* CGet::LuaConVars()
{
	return m_pluaconvars;
}

IMenuSystem* CGet::MenuSystem()
{
	return m_pmenusystem;
}

IResources* CGet::Resources()
{
	return m_presources;
}

IIntroScreen* CGet::IntroScreen()
{
	return m_pintroscreen;
}

IMaterialSystem* CGet::Materials()
{
	return m_pmaterials;
}

IGMHTML* CGet::HTML()
{
	return m_phtml;
}

IServerAddons* CGet::ServerAddons()
{
	return m_pserveraddons;
}

ISteamHTTP* CGet::SteamHTTP()
{
	return nullptr; // ToDo
}

ISteamRemoteStorage* CGet::SteamRemoteStorage()
{
	return nullptr; // ToDo
}

ISteamScreenshots* CGet::SteamScreenshots()
{
	return nullptr; // ToDo
}

ISteamUtils* CGet::SteamUtils()
{
	return nullptr; // ToDo
}

ISteamApps* CGet::SteamApps()
{
	return nullptr; // ToDo
}

ISteamUser* CGet::SteamUser()
{
	return nullptr; // ToDo
}

ISteamFriends* CGet::SteamFriends()
{
	return nullptr; // ToDo
}

ISteamUGC* CGet::SteamUGC()
{
	return nullptr; // ToDo
}

ISteamGameServer* CGet::SteamGameServer()
{
	return nullptr; // ToDo
}

ISteamNetworking* CGet::SteamNetworking()
{
	return nullptr; // ToDo
}

CreateInterfaceFn GetFactory( const char* dll )
{
	CSysModule* mdl = g_pFullFileSystem->LoadModule( dll );
	if ( !mdl )
	{
		mdl = g_pFullFileSystem->LoadModule( dll, "GAMEBIN" );
		if ( !mdl )
		{
			Error( "Failed to find %s!\n", dll ); // if gmod_audio.dll fails here. Check if you added the bass.dll to the game/bin folder!
			return NULL;
		}
	}

	return Sys_GetFactory( mdl );
}

void CGet::Initialize( IFileSystem* fs )
{
	// ToDo
	// NOTE: Gmod uses CModuleLoader<T>(IFileSystem*, const char*, const char*, bool) for this.

	CreateInterfaceFn lua_sharedfn = GetFactory( "lua_shared" DLL_EXT_STRING );
	m_pluashared = (GarrysMod::Lua::ILuaShared*)lua_sharedfn( GMOD_LUASHARED_INTERFACE, NULL );
	m_pluaconvars = (GarrysMod::Lua::ILuaConVars*)lua_sharedfn( GMOD_LUACONVARS_INTERFACE, NULL );
	m_pluashared->Init( g_AppSystemFactory, false, nullptr, this );
	//m_pluaconvars->Init(); // ToDo: find out why it crashes :<

	CreateInterfaceFn menusystemfn = GetFactory( "menusystem" DLL_EXT_STRING );
	//m_pmenusystem = ( IMenuSystem* )menusystemfn( INTERFACEVERSION_MENUSYSTEM, NULL );

	CreateInterfaceFn clientfn = GetFactory( "client" DLL_EXT_STRING );
	m_pintroscreen = ( IIntroScreen* )clientfn( INTERFACEVERSION_INTROSCREEN, NULL );

	CreateInterfaceFn materialsystemfn = GetFactory( "materialsystem" DLL_EXT_STRING );
	m_pmaterials = ( IMaterialSystem* )materialsystemfn( MATERIAL_SYSTEM_STUB_INTERFACE_VERSION, NULL );

	//CreateInterfaceFn resourcesfn = GetFactory( "resources" DLL_EXT_STRING );
	//m_presources = ( IResources* )resourcesfn( INTERFACEVERSION_RESOURCES, NULL );

	// ToDo: Add gmhtml to the compile.
	//CreateInterfaceFn htmlfn = GetFactory( "gmhtml" DLL_EXT_STRING );
	//m_phtml = ( IGMHTML* )htmlfn( INTERFACEVERSION_GMHTML, NULL );

	CreateInterfaceFn audiofn = GetFactory( "gmod_audio" DLL_EXT_STRING );
	m_paudio = ( IGMod_Audio* )audiofn( INTERFACEVERSION_GMODAUDIO, NULL );
	m_paudio->Init( g_AppSystemFactory );

	// ToDo: Finish IServerAddons and expose them.
	//CreateInterfaceFn serveraddonsgn = GetFactory( "server" DLL_EXT_STRING );
	//m_pserveraddons = ( IServerAddons* )serveraddonsgn( INTERFACEVERSION_SERVERADDONS, NULL );

	g_pFullFileSystem->GMOD_SetupDefaultPaths( GetBaseDirectory(), "garrysmod" /*COM_GetModDirectory()*/ ); // This is probably called from inside GMOD_SetupDefaultPaths
}

void CGet::ShutDown( )
{
	// ToDo
}

void CGet::RunSteamCallbacks( )
{
	// ToDo
}

void CGet::ResetSteamAPIs( )
{
	// ToDo
}

void CGet::SetMotionSensor( IMotionSensor* sensor )
{
	m_pmotionsensor = sensor;
}

IMotionSensor* CGet::MotionSensor( )
{
	return m_pmotionsensor;
}

int CGet::Version( )
{
	// ToDo
	return 0;
}

const char* CGet::VersionStr( )
{
	// ToDo
	return "0";
}

IGMod_Audio* CGet::Audio( )
{
	return m_paudio;
}

const char* CGet::VersionTimeStr( )
{
	// ToDo
	return "1.01.0001";
}

IAnalytics* CGet::Analytics( )
{
	// ToDo
	return m_panalytics;
}

void CGet::UpdateRichPresense( const char* )
{
	// ToDo
	// #Status_Generic
	// steam_display
	// generic
	// status
}

void CGet::ResetRichPresense( )
{
	// ToDo
}

void CGet::FilterText( const char*, char*, int, ETextFilteringContext, CSteamID )
{
	// ToDo
}