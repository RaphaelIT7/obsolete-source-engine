// Copyright Valve Corporation, All rights reserved.
//
// Hammer launcher. Hammer is sitting in its own DLL.

#ifdef _WIN32
#include "winlite.h"
#include <comdef.h>  // _com_error
#include <WinSock2.h>

#include "windows/scoped_com.h"
#include "windows/scoped_timer_resolution.h"
#include "windows/scoped_winsock.h"
#endif

#include "appframework/AppFramework.h"
#include "inputsystem/iinputsystem.h"
#include "datacache/idatacache.h"
#include "datacache/imdlcache.h"
#include "materialsystem/imaterialsystem.h"
#include "tier0/icommandline.h"
#include "tier0/dbg.h"
#include "tier0/platform.h"
#include "vgui/ivgui.h"
#include "vgui/ISurface.h"
#include "inputsystem/iinputsystem.h"
#include "tier0/icommandline.h"
//#include "p4lib/ip4.h"

// Global systems

namespace {

// The application object
class HammerAppSystemGroup final : public CAppSystemGroup {
 public:
  HammerAppSystemGroup()
      : file_system_{nullptr},
        data_cache_{nullptr},
        input_system_{nullptr},
        material_system_{nullptr},
        hammer_{nullptr} {}

  // Methods of IApplication
  bool Create() override;
  bool PreInit() override;
  int Main() override;
  void PostShutdown() override;
  void Destroy() override;

 private:
  int MainLoop();

  IFileSystem *file_system_;
  IDataCache *data_cache_;
  IInputSystem *input_system_;
  IMaterialSystem *material_system_;
  IHammer *hammer_;
};

template <size_t out_size>
const char *PrefixMessageGroup(
    _Out_z_bytecapcount_(out_size) char (&out)[out_size], const char *group,
    const char *message) {
  const char *out_group{GetSpewOutputGroup()};

  out_group = out_group && out_group[0] ? out_group : group;

  const size_t length{strlen(message)};
  if (length > 1 && message[length - 1] == '\n') {
    Q_snprintf(out, std::size(out), "[%s] %s", out_group, message);
  } else {
    Q_snprintf(out, std::size(out), "%s", message);
  }

  return out;
}

SpewRetval_t HammerSpewFunc(SpewType_t type, char const *raw) {
  char message[4096];
  Plat_DebugString(PrefixMessageGroup(message, "hammer", raw));

  if (type == SPEW_ASSERT) return SPEW_DEBUGGER;

  if (type == SPEW_WARNING) {
    MessageBoxA(nullptr, message, "Hammer - Warning", MB_OK | MB_ICONWARNING);
    return SPEW_CONTINUE;
  }

  if (type == SPEW_ERROR) {
    MessageBoxA(nullptr, message, "Hammer - Error", MB_OK | MB_ICONSTOP);
    return SPEW_ABORT;
  }

  return SPEW_CONTINUE;
}

// Create all singleton systems
bool HammerAppSystemGroup::Create() {
  // Save some memory so engine/hammer isn't so painful
  CommandLine()->AppendParm("-disallowhwmorph", nullptr);

  SpewOutputFunc(HammerSpewFunc);

  // Game and hammer require theses.
  const CPUInformation *cpu_info{GetCPUInformation()};
  if (!cpu_info->m_bSSE || !cpu_info->m_bSSE2 || !cpu_info->m_bSSE3 ||
      !cpu_info->m_bSSE41 || !cpu_info->m_bSSE42) {
    Error(
        "Sorry, your CPU missed SSE / SSE2 / SSE3 / SSE4.1 / SSE4.2 "
        "instructions required for the game.\n\nPlease upgrade your CPU.");
  }

#ifdef WIN32
  // COM is required.
  const se::common::windows::ScopedCom scoped_com{
      static_cast<COINIT>(COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE |
                          COINIT_SPEED_OVER_MEMORY)};
  if (FAILED(scoped_com.errc())) {
    const _com_error com_error{scoped_com.errc()};
    Error("Unable to initialize COM (0x%x): %s.\n\n", scoped_com.errc(),
          com_error.ErrorMessage());
  }

  using namespace std::chrono_literals;
  constexpr std::chrono::milliseconds kSystemTimerResolution{2ms};

  // System timer precision affects Sleep & friends performance.
  const se::common::windows::ScopedTimerResolution scoped_timer_resolution{
      kSystemTimerResolution};
  if (!scoped_timer_resolution) {
    Warning(
        "Unable to set Windows timer resolution to %lld ms. Will use default "
        "one.",
        static_cast<long long>(kSystemTimerResolution.count()));
  }

  const se::common::windows::ScopedWinsock scoped_winsock{WINSOCK_VERSION};
  if (scoped_winsock.errc()) {
    Warning("Windows sockets 2.2 unavailable (%d): %s.\n",
            scoped_winsock.errc(), scoped_winsock.errc().message().c_str());
  }
#endif

	// Add Perforce separately since it's possible it isn't there. (SDK)
	/*if ( !CommandLine()->CheckParm( "-nop4" ) )
	{
		AppModule_t p4Module = LoadModule( "p4lib.dll" );
		AddSystem( p4Module, P4_INTERFACE_VERSION );
	}*/
	// Connect to interfaces loaded in AddSystems that we need locally
	g_pMaterialSystem = (IMaterialSystem*)FindSystem( MATERIAL_SYSTEM_INTERFACE_VERSION );
	g_pHammer = (IHammer*)FindSystem( INTERFACEVERSION_HAMMER );
	g_pDataCache = (IDataCache*)FindSystem( DATACACHE_INTERFACE_VERSION );
	g_pInputSystem = (IInputSystem*)FindSystem( INPUTSYSTEM_INTERFACE_VERSION );

  bool is_steam{false};
  char file_system_path[MAX_PATH];
  file_system_path[0] = '\0';

  if (FileSystem_GetFileSystemDLLName(file_system_path, MAX_PATH, is_steam) !=
      FS_OK) {
    return false;
  }

  const AppModule_t file_system_module{LoadModule(file_system_path)};
  if (file_system_module == APP_MODULE_INVALID) return false;

  file_system_ =
      AddSystem<IFileSystem>(file_system_module, FILESYSTEM_INTERFACE_VERSION);
  if (!file_system_) return false;

  FileSystem_SetBasePaths(file_system_);

  AppSystemInfo_t app_systems[] = {
      {"materialsystem" DLL_EXT_STRING, MATERIAL_SYSTEM_INTERFACE_VERSION},
      {"inputsystem" DLL_EXT_STRING, INPUTSYSTEM_INTERFACE_VERSION},
      {"studiorender" DLL_EXT_STRING, STUDIO_RENDER_INTERFACE_VERSION},
      {"vphysics" DLL_EXT_STRING, VPHYSICS_INTERFACE_VERSION},
      {"datacache" DLL_EXT_STRING, DATACACHE_INTERFACE_VERSION},
      {"datacache" DLL_EXT_STRING, MDLCACHE_INTERFACE_VERSION},
      {"datacache" DLL_EXT_STRING, STUDIO_DATA_CACHE_INTERFACE_VERSION},
      {"engine" DLL_EXT_STRING, VENGINE_LAUNCHER_API_VERSION},
      {"vguimatsurface" DLL_EXT_STRING, VGUI_SURFACE_INTERFACE_VERSION},
      {"vgui2" DLL_EXT_STRING, VGUI_IVGUI_INTERFACE_VERSION},
      {"hammer_dll" DLL_EXT_STRING, INTERFACEVERSION_HAMMER},
      {"", ""}  // Required to terminate the list
  };

  if (!AddSystems(app_systems)) {
    Error(
        "Please check Hammer installed correctly.\n\n"
        "Unable to add launcher systems from *" DLL_EXT_STRING
        "s. Looks like required components are missed or broken.");
    return false;
  }

  // dimhotepus: Drop Perforce support
  // Add Perforce separately since it's possible it isn't there. (SDK)
  // if ( !CommandLine()->CheckParm( "-nop4" ) )
  // {
  // 	AppModule_t p4Module = LoadModule( "p4lib" DLL_EXT_STRING );
  // 	AddSystem( p4Module, P4_INTERFACE_VERSION );
  // }
  // Connect to interfaces loaded in AddSystems that we need locally
  data_cache_ = FindSystem<IDataCache>(DATACACHE_INTERFACE_VERSION);
  input_system_ = FindSystem<IInputSystem>(INPUTSYSTEM_INTERFACE_VERSION);
  material_system_ =
      FindSystem<IMaterialSystem>(MATERIAL_SYSTEM_INTERFACE_VERSION);
  hammer_ = FindSystem<IHammer>(INTERFACEVERSION_HAMMER);

  // This has to be done before connection.
  material_system_->SetShaderAPI("shaderapidx9" DLL_EXT_STRING);

  return true;
}

void HammerAppSystemGroup::Destroy() {
  hammer_ = nullptr;
  material_system_ = nullptr;
  input_system_ = nullptr;
  data_cache_ = nullptr;
  file_system_ = nullptr;

  // dimhotepus: Restore default spew.
  SpewOutputFunc(DefaultSpewFunc);
}

// Init, shutdown
bool HammerAppSystemGroup::PreInit() {
  if (!hammer_->InitSessionGameConfig(GetVProjectCmdLineValue())) return false;

  // Init the game and mod dirs in the file system.  This needs to happen before
  // calling Init on the material system.
  CFSSearchPathsInit search_paths_init;
  search_paths_init.m_pFileSystem = file_system_;
  search_paths_init.m_pDirectoryName = hammer_->GetDefaultModFullPath();

  if (FileSystem_LoadSearchPaths(search_paths_init) != FS_OK) {
    Error("Unable to load search paths!\n");
  }

  // Required to run through the editor
  material_system_->EnableEditorMaterials();

  // needed for VGUI model rendering
  material_system_->SetAdapter(0, MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE);

  return true;
}

void HammerAppSystemGroup::PostShutdown() {}

// main application
int HammerAppSystemGroup::Main() { return hammer_->MainLoop(); }

}  // namespace

// Define the application object.
HammerAppSystemGroup g_hammer_app;
DEFINE_WINDOWED_APPLICATION_OBJECT_GLOBALVAR(g_hammer_app);
