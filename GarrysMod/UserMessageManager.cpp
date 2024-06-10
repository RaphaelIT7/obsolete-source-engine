#include "cbase.h"
#include "UserMessageManager.h"

ConVar sv_usermessage_maxsize( "sv_usermessage_maxsize", "1024", 0, "The max number of bytes to output per tick" );

CUserMessageManager g_UserMessageManager;
void CUserMessageManager::LevelShutdownPreEntity()
{
	// ToDo
}

void CUserMessageManager::PreClientUpdate()
{
	// ToDo

	// Loop throu all queried messages

	// Send them
}

void CUserMessageManager::SendMessage( QueuedMessage_t* msg )
{
	// ToDo
}

void CUserMessageManager::AddToQueue( QueuedMessage_t* msg )
{
	// ToDo
}