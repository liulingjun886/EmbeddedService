#include "Reactor.h"
#include "PollerObject.h"
#include "PollerUnit.h"
#include "TimerUnit.h"
#include "WorkThreadPool.h"
#include "ASyncCallSupport.h"
#include "CmdOpt.h"

static CTimerUnit m_timer_unit;
static CPollerUnit m_poller_unit;
//static unsigned int m_work_thread_num = 2;
//CWorkThread *m_pWorkThreads = nullptr;


CReactor* CReactor::GetInstance()
{
	static CReactor s_pReactor;
	return &s_pReactor;
}

int CReactor::Init(int num)
{
	//m_work_thread_num = num;
	if(CCmdOpt::GetInstance()->Init())
		return -1;
		
	if(CThreadPool::GetInstance()->Init(num))
		return -1;
	
	return 0;
}

int CReactor::AddThreads(int num)
{
	return CThreadPool::GetInstance()->AddThreads(num);
}


bool CReactor::IsRun()
{
	return m_b_running;
}


void CReactor::RunLoop()
{
	while(m_b_running)
	{
		m_timer_unit.ProcessTimeEvent();
		UINT64 msec = m_timer_unit.GetMinWaitTime();
		m_poller_unit.Wait(msec);
		m_timer_unit.ProcessTimeEvent();
		m_poller_unit.HandPollerEvents();
	}
	CThreadPool::GetInstance()->Stop();	
}

void CReactor::StopLoop()
{
	m_b_running = false;
}


void CReactor::RegisterPoller(CPollerObject* poller)
{
	m_poller_unit.AddPoller(poller);
}

void CReactor::UnRegisterPoller(CPollerObject* poller)
{
	m_poller_unit.RemovePoller(poller);
}

void CReactor::RegisterTimer(CTimeNode* const timerNode)
{
	m_timer_unit.AddTimer(timerNode);
}

void CReactor::UnRegisterTimer(CTimeNode* const timerNode)
{
	m_timer_unit.RemoveTimer(timerNode);
}

/*void CReactor::SendAsyncAction(CAsyncCallSupport* pObj, UINT32 cmd,ASyncCallDataInst& pReq)
{
	assert(pObj->GetThreadIndex() < m_work_thread_num);
	m_pWorkThreads[pObj->GetThreadIndex()].AddAsyncCallReq(pObj,cmd,pReq);
}

void CReactor::SendSyncAction(CAsyncCallSupport* pObj, UINT32 cmd, ASyncCallDataInst& data, INT32 ret)
{
	CCmdOpt::GetInstance()->SendSyncAction(pObj,cmd,data,ret);
}*/

