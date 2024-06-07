#include "networkstringtabledefs.h"

class NetworkString
{
public:
#ifdef CLIENT_DLL
	static void Install();
#else
	static void Create();
#endif

	static void Add( const char* strVar );
	static int Get( const char* strVar );
	static const char* Convert( int iIdx );

private:
	static INetworkStringTable* pStringTable;
};

class NetworkVarNames
{
public:
#ifdef CLIENT_DLL
	static void Install();
#else
	static void Create();
#endif

	static void Add( const char* strVar );
	static int Get( const char* strVar );
	static const char* Convert( int iIdx );

private:
	static INetworkStringTable* pStringTable;
};