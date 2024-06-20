#include "cbase.h"
#include "UserMessageManager.h"
#include "usermessages.h"

ConVar sv_usermessage_maxsize( "sv_usermessage_maxsize", "1024", 0, "The max number of bytes to output per tick" );

CUserMessageManager g_UserMessageManager;
void CUserMessageManager::LevelShutdownPreEntity()
{
	// ToDo
	FOR_EACH_VEC( m_pQueue, i )
	{
		QueuedMessage_t* msg = m_pQueue[i];
		delete msg->filter; // ToDo
		delete msg->bf;
		delete msg; // Nuke it
	}

	m_pQueue.RemoveAll();
}

void CUserMessageManager::PreClientUpdate() // ToDo: Redo this later.
{
	int sent = 0;
	CUtlVector<QueuedMessage_t*> pSentMessages;

	FOR_EACH_VEC( m_pQueue, i )
	{
		QueuedMessage_t* msg = m_pQueue[i];
		// Some magic checks Warning( "Warning: Usermessage was larger than 256 bytes. This will cause problems." );

		sent += msg->bf->GetNumBytesWritten();

		SendMessage( msg );
		pSentMessages.AddToTail( msg );

		if ( sent > sv_usermessage_maxsize.GetInt() )
			break;
	}

	FOR_EACH_VEC( pSentMessages, i )
	{
		QueuedMessage_t* msg = m_pQueue[i];
		m_pQueue.Remove( i );

		delete msg->filter; // ToDo
		delete msg->bf;
		delete msg; // Nuke it
	}
	pSentMessages.RemoveAll();
}

void CUserMessageManager::SendMessage( QueuedMessage_t* msg ) // This is still unfinished
{
	int msgType = usermessages->LookupUserMessage( "LuaUserMessage" );
	if ( msgType != -1 )
	{
		bf_write* g_pMsgBuffer = engine->UserMessageBegin( msg->filter, msgType );
		if ( !g_pMsgBuffer->WriteBits( msg->bf->GetBasePointer(), msg->bf->GetNumBitsWritten() ) ) // Gmod does it differently
			Msg( "Error sending usermessage - too large (%s)", "" ); // ToDo: What was the string again?

		engine->MessageEnd();
	}
}

void CUserMessageManager::AddToQueue( QueuedMessage_t* msg )
{
	if ( m_pQueue.Size() > 5000 )
		Warning( "Warning: %i global queued usermessages!", m_pQueue.Size() );

	// if magicTarget.Size() > 5000
	// Warning( "Warning: Player %i has %i queued usermessages!" );

	// if !IsValid(magicTarget)
	// Warning( "Tried to send message to invalid or non existent player! (%i)" );

	m_pQueue.AddToTail( msg );
}