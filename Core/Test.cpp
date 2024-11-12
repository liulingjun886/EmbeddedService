#include "Reactor.h"
#include "TimeNode.h"
#include "TimeEventHander.h"
#include <iostream>
#include <unistd.h>
#include "ASyncCallSupport.h"
#include "ShareData.h"
#include "stdlib.h"

static int i = 0;

class TestTimer : public CTimeEventHander, public CAsyncCallSupport
{
public:
	TestTimer():m_i(++i)
	{
		m_timer.InitTimerNode(this,1);
		m_timer.StartTimer((0 == m_i%100)?100:500,-1);
		SetThreadIndex(m_i % 4);
	}
	
	int ProcessTimeOutEvent(UINT32 nTimeId)
	{
		SendAsyncAction(1);
		printf("ProcessTimeOutEvent\n");
		return 0;
	}

	virtual void DoAsyncAction(UINT32 cmd, ASyncCallDataInst& Req)
	{
		printf("DoAsyncAction = %d, len = %d\n",cmd,Req.length());
		SendSyncAction(cmd);
		return;
	}

	void DoSyncAction(UINT32 cmd, INT32 ret, ASyncCallDataInst& Resp)
	{
		printf("DoSyncAction len = %d,m_i = %d\n", Resp.length(),m_i);
		//_exit(0);
	}
private:
	CTimer m_timer;
	int m_i;
};

int main()
{
	CReactor::GetInstance()->Init(4);
	TestTimer t[8000];
	CReactor::GetInstance()->RunLoop();
	return 0;
}
