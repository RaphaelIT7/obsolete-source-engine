#include "networkstringtabledefs.h"

namespace GarrysMod
{
	class StringTable
	{
	public:
		static void Reset();
	#ifdef CLIENT_DLL
		static void Install( const char* tableName );
	#else
		static void Create();
	#endif

	private:
		static INetworkStringTable* g_StringTable_GameInfo;
	};
}