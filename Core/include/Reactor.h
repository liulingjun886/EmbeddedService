#pragma once
#include "UserTypeDefines.h"

class CPollerObject;
class CTimeNode;
class CAsyncCallSupport;
class ASyncCallDataInst;

class CReactor
{
public:
	static CReactor* GetInstance();
private:
	CReactor():m_b_running(true)
	{
		
	}
public:
	~CReactor()
	{
	
	}
	int Init(int num);
	int AddThreads(int num);
	bool IsRun();
	void RunLoop();
	void StopLoop();
	void RegisterPoller(CPollerObject* poller);
	void UnRegisterPoller(CPollerObject* poller);
	void RegisterTimer(CTimeNode* const timerNode);
	void UnRegisterTimer(CTimeNode* const timerNode);
private:
	bool m_b_running;
};

