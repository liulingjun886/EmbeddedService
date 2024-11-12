#pragma once
#include <iostream>
#include <sys/time.h>
#include "MinHeap.h"
#include "TimeNode.h"
#include "UserTypeDefines.h"

class CTimerUnit
{
public:
	CTimerUnit()
	{
		UpdateNow();
	}
	~CTimerUnit()
	{
		
	}
public:
	//添加新定时器
	void AddTimer(CTimeNode* node)
	{
		node->m_n_expired = m_time_now + node->m_n_trig_interval;
		m_heap_timer.Insert(node);
	}
	//移除定时器
	void RemoveTimer(CTimeNode* node)
	{
		m_heap_timer.Remove(node);
	}
	//获取当前最小过期时间
	UINT64 GetMinWaitTime()
	{
		if(0 == m_heap_timer.GetNelts())
			return -1;
		UpdateNow();
		CTimeNode* pNode = m_heap_timer.GetMinNode();
		return (pNode->GetExpireTime() > m_time_now) ? (pNode->GetExpireTime() - m_time_now) : 0;
	}
	//处理理定时器过期
	void ProcessTimeEvent()
	{
		UpdateNow();
		CTimeNode* pNode = nullptr;
		while(0 != m_heap_timer.GetNelts())
		{
			pNode = m_heap_timer.GetMinNode();
			if(m_time_now < pNode->GetExpireTime())
			{
				return;
			}
			m_heap_timer.Dequeue();
			pNode->TimerNotify();
		}
	}

	void UpdateNow()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		m_time_now = (UINT64)tv.tv_sec*1000 + tv.tv_usec/1000;
	}
private:
	CMinHeap<CTimeNode> m_heap_timer;
	UINT64 m_time_now;
};