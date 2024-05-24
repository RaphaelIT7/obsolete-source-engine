#include "GarrysMod/IGet.h"
#include "server.h"

class CGet : public IGet
{
public:
	virtual void OnLoadFailed( const char* reason );
	virtual const char* GameDir();
	virtual bool IsDedicatedServer();
	virtual int GetClientCount();
	virtual IFileSystem* FileSystem();
	virtual ILuaShared* LuaShared();
	virtual ILuaConVars* LuaConVars();
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
	ILuaShared* m_pluashared;
	ILuaConVars* m_pluaconvars;
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

ILuaShared* CGet::LuaShared()
{
	return m_pluashared;
}

ILuaConVars* CGet::LuaConVars()
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

void CGet::Initialize( IFileSystem* fs )
{
	// ToDo
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