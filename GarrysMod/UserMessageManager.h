#include "igamesystem.h"
#include "recipientfilter.h"

struct QueuedMessage_t // ToDo: Yoink it from Gmod.
{
	bf_write* bf;
	CRecipientFilter* filter;
};

class CUserMessageManager : public CAutoGameSystemPerFrame
{
public:
	virtual void LevelShutdownPreEntity() OVERRIDE;
	virtual void PreClientUpdate() OVERRIDE;

public:
	void SendMessage( QueuedMessage_t* );
	void AddToQueue( QueuedMessage_t* );

private:
	CUtlVector<QueuedMessage_t*> m_pQueue; // ToDo: Yoink structure from Gmod.
};

extern CUserMessageManager g_UserMessageManager;