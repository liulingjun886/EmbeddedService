#include "WorkThreadPool.h"
#include "WorkThread.h"
#include "ASyncCallSupport.h"

CThreadPool* CThreadPool::GetInstance()
{
	static CThreadPool m_SingleObj;
	return &m_SingleObj;
}


CThreadPool::CThreadPool():m_work_thread_num(0),m_pWorkThreads(nullptr)
{
	m_vec_WorkThreads.clear();
}

CThreadPool::~CThreadPool()
{
	/*if(m_pWorkThreads)
	{
		delete[] m_pWorkThreads;
		m_pWorkThreads = nullptr;
	}*/

	for(int i = 0; i < m_vec_WorkThreads.size(); ++i)
	{
		delete m_vec_WorkThreads[i];
	}
	m_vec_WorkThreads.clear();
}

int CThreadPool::Init(int num)
{
	/*if(num <= 0)
		return 0;
	
	m_work_thread_num = num;
	m_pWorkThreads = new CWorkThread[m_work_thread_num];
	if(!m_pWorkThreads)
		return -1;
		
	for(int i = 0; i < m_work_thread_num; ++i)
	{
		if(m_pWorkThreads[i].start())
			return -1;
	}
	return 0;*/
	return AddThreads(num);
}

int CThreadPool::AddThreads(int num)
{
	if(num <= 0)
		return 0;
	for(int i = 0; i < num; ++i)
	{
		CWorkThread* pThread = new CWorkThread;
		if(!pThread)
			return -1;
		pThread->start();
		m_vec_WorkThreads.push_back(pThread);
	}
}


int CThreadPool::GetWorkThreadNum() const
{
	//return m_work_thread_num;
	return m_vec_WorkThreads.size();
}


int CThreadPool::AddAsyncCallReq(CAsyncCallSupport* pObj,INT32 cmd, ASyncCallDataInst& pData,INT32 param)
{
	assert(m_vec_WorkThreads.size() > pObj->GetThreadIndex());
	return m_vec_WorkThreads[pObj->GetThreadIndex()]->AddAsyncCallReq(pObj, cmd, pData, param);
}


void CThreadPool::Stop()
{
	/*for(int i = 0; i < m_work_thread_num; ++i)
	{
		m_pWorkThreads[i].stop();
	}*/	

	for(int i = 0; i < m_vec_WorkThreads.size(); ++i)
	{
		m_vec_WorkThreads[i]->stop();
	}
}

