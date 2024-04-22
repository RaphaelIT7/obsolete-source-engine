#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <list>

// #define GMOD_DEDICATED

class CSteamAPIContext;
class IAddonDownloadNotification;
struct SteamUGCDetails_t;

namespace IAddonSystem
{
	struct Information
	{
		std::string title;
		std::string file;
		std::string tags;
		std::string placeholder1;
		uint64_t time_updated;
		uint64_t wsid;
		uint64_t creator;
		uint64_t hcontent_file;
		uint64_t size;
		uint64_t hcontent_preview;
		uint32_t timeadded;
	};

	struct UGCInfo
	{
		std::string title;
		std::string file;
		std::string placeholder1;
		uint64_t wsid;
		uint64_t creator;
		uint32_t pubdate;
	};
}

namespace Steamworks::FileDetails
{
	class Request;
}

namespace Addon
{
	class FileSystem;

	namespace Job
	{
		class Base
		{
			public:
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};
	}

	class FileSystem
	{
		public:
			virtual void Clear( ) = 0;
			virtual void Refresh( ) = 0;
			virtual int MountFile( const std::string &, std::vector<std::string> * ) = 0;
			virtual bool ShouldMount( const std::string & ) = 0;
			virtual bool ShouldMount( uint64_t ) = 0;
			virtual void SetShouldMount( const std::string &, bool ) = 0;
			virtual void Save( ) = 0;
			virtual const std::list<IAddonSystem::Information> &GetList( ) const = 0;
			virtual const std::list<IAddonSystem::UGCInfo> &GetUGCList( ) const = 0;
			virtual void ScanForSubscriptions( CSteamAPIContext *, const char * ) = 0;
			virtual void Think( ) = 0;
			virtual void SetDownloadNotify( IAddonDownloadNotification * ) = 0;
			virtual int Notify( ) = 0;
			virtual bool IsSubscribed( uint64_t ) = 0;
			virtual const IAddonSystem::Information *FindFileOwner( const std::string & ) = 0;
			virtual void AddFile( const IAddonSystem::Information & ) = 0;
			virtual void ClearAllGMAs( ) = 0;
			virtual void GetSteamUGCFile( uint64_t, bool ) = 0;
			virtual void UnmountAddon( uint64_t ) = 0;
			virtual void UnmountServerAddons( ) = 0;
			virtual void MountFloatingAddons( ) = 0;
			virtual void Shutdown( ) = 0;
			virtual void AddFile( const SteamUGCDetails_t & ) = 0;
			virtual void AddSubscription( const SteamUGCDetails_t & ) = 0;
			virtual void AddJob( Job::Base * ) = 0;
			virtual bool HasChanges( ) = 0;
			virtual void MarkChanged( ) = 0;
			virtual void AddonDownloaded( IAddonSystem::Information & ) = 0;
			virtual void NotifyAddonFailedToDownload( IAddonSystem::Information & ) = 0;
			virtual const std::list<SteamUGCDetails_t> &GetSubList( ) const = 0;
			virtual void IsAddonValidPreInstall( SteamUGCDetails_t ) = 0;
			virtual void Load( ) = 0;
	};

	namespace Task
	{
		class DownloadAddons : Addon::Job::Base
		{
			public:
				virtual ~DownloadAddons( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class DownloadFile : Addon::Job::Base
		{
			public:
				virtual ~DownloadFile( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class AddFloatingAddons : Addon::Job::Base
		{
			public:
				virtual ~AddFloatingAddons( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class GetSubscriptions : Addon::Job::Base
		{
			public:
				virtual ~GetSubscriptions( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class GetSubscriptions_Offline : Addon::Job::Base
		{
			public:
				virtual ~GetSubscriptions_Offline( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class MountAvailable : Addon::Job::Base
		{
			public:
				virtual ~MountAvailable( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class NotifyStart : Addon::Job::Base
		{
			public:
				virtual ~NotifyStart( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class NotifyEnd : Addon::Job::Base
		{
			public:
				virtual ~NotifyEnd( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};

		class OnSubscribed : Addon::Job::Base
		{
			public:
				virtual ~OnSubscribed( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
				virtual void OnReceiveFileInfo( Steamworks::FileDetails::Request * ) = 0;
		};

		class UpdateTotals : Addon::Job::Base
		{
			public:
				virtual ~UpdateTotals( ) = 0;
				virtual void Start( ) = 0;
				virtual void Cycle( ) = 0;
				virtual void Finished( ) = 0;
				virtual void Init( Addon::FileSystem *, CSteamAPIContext * ) = 0;
		};
	}
}