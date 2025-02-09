#include "TimeNode.h"
#include "Reactor.h"
#include "TimeEventHander.h"

CTimeNode::CTimeNode():m_n_expired(0),m_n_repeat(0),
m_n_trig_interval(0),m_timer_node(nullptr),m_n_time_id(0)
{
	
}

CTimeNode::~CTimeNode()
{
	
}

void CTimeNode::InitTimerNode(CTimeEventHander* pHand,UINT32 nTimeId)
{
	m_timer_node = pHand;
	m_n_time_id = nTimeId;
}

void CTimeNode::StartTimer(UINT32 nExpired,UINT64 nCnt)
{
	if(0 != m_n_repeat)
		StopTimer();
	m_n_repeat = nCnt;
	m_n_trig_interval = nExpired;
	CReactor::GetInstance()->RegisterTimer(this);
}

void CTimeNode::StartTimerSec(UINT32 nExpired,UINT64 nCnt)
{
	StartTimer(nExpired*1000,nCnt);
}

void CTimeNode::StopTimer()
{
	if(0 == m_n_repeat)
		return;

	CReactor::GetInstance()->UnRegisterTimer(this);
	m_n_repeat = 0;
}

bool CTimeNode::IsActive()
{
	return 0 != m_n_repeat;
}

void CTimeNode::TimerNotify()
{
	if(0 != (--m_n_repeat))
	{
		CReactor::GetInstance()->RegisterTimer(this);
	}
	
	if(m_timer_node)
		m_timer_node->ProcessTimeOutEvent(m_n_time_id);
}

bool CTimeNode::operator<(const CTimeNode& node)
{
	return m_n_expired < node.m_n_expired;
}

UINT64 CTimeNode::GetExpireTime() const 
{
	return m_n_expired;
}

