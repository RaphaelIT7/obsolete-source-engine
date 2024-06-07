#include "cbase.h"
#include "gl_message.h"
#include "usermessages.h"

void RecieveLuaUserMessage( bf_read& msg )
{
	// ToDo

	// call usermessage.IncomingMessage
}

void RegisterLuaUserMessages()
{
	usermessages->Register( "LuaUserMessage", -1 );

#ifdef CLIENT_DLL
	usermessages->HookMessage( "LuaUserMessage", RecieveLuaUserMessage );
#endif
}