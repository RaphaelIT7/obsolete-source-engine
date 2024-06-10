#include "igamesystem.h"

struct QueuedMessage_t // ToDo: Yoink it from Gmod.
{

};

class CUserMessageManager : public CAutoGameSystem
{
public:
	virtual void LevelShutdownPreEntity();
	virtual void PreClientUpdate();

public:
	void SendMessage( QueuedMessage_t* );
	void AddToQueue( QueuedMessage_t* );
};

extern CUserMessageManager g_UserMessageManager;