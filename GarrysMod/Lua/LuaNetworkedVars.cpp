#include "cbase.h"
#include "LuaNetworkedVars.h"
#include "usermessages.h"

CLuaNetworkedVars::CLuaNetworkedVars()
{
	// ToDo
}

void CLuaNetworkedVars::RegisterUserMessages()
{
	usermessages->Register( "NetworkedVar", -1 );

#ifdef CLIENT_DLL

#endif
}