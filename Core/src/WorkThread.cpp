#include "WorkThread.h"
#include "Reactor.h"
#include "ASyncCallSupport.h"
#include "ShareData.h"

void *Run(void *pArgs)
{
	CWorkThread* pThread = (CWorkThread*)pArgs;
	pThread->run();
	return NULL;
}

CWorkThread::CWorkThread():m_b_sleep(false)
{
	
}

CWorkThread::~CWorkThread()
{
	AsyncCallListNode *p = nullptr;
	while(p = m_list_req.Pop())
	{
		delete p;
	}
}

int CWorkThread::start()
{
	// for(int i = 0; i < MAX_WORKTHREAD_REQ; ++i)
	// {
	// 	m_list_rest_data_pool.Push(m_sz_block_pool[i]);
	// }

	if(pthread_create(&m_pid,NULL,Run,this))
		return -1;
	return 0;
}

void CWorkThread::stop()
{
	m_cond.WakeUp();
	pthread_join(m_pid,NULL);
}


void CWorkThread::run()
{
	AsyncCallListNode *p = nullptr;
	int ret = -1;
	while(CReactor::GetInstance()->IsRun())
	{
		m_mutex.Lock();
		p = m_list_req.Pop();
		if(!p)
		{
			m_b_sleep = true;
			m_cond.Wait(m_mutex);
			m_b_sleep = false;
			m_mutex.UnLock();
			continue;
		}
		m_mutex.UnLock();
		p->pObj->DoAsyncAction(p->cmd,p->param,p->pData);
		delete p;
		//FreeAsyncCallListNode(p);
	}
}

AsyncCallListNode *CWorkThread::MallocAsyncCallListNode()
{
	// CToolLock lock(&m_mutex);
	// AsyncCallListNode *p = m_list_rest_data_pool.Pop();
	// if(p)
	// 	return p;
	return nullptr;
}

void CWorkThread::FreeAsyncCallListNode(AsyncCallListNode* pNode)
{
	//CToolLock lock(&m_mutex);
	//m_list_rest_data_pool.Push(pNode);
}


int CWorkThread::AddAsyncCallReq(CAsyncCallSupport* pObj,INT32 cmd, ASyncCallDataInst& pData, INT32 param)
{
	AsyncCallListNode *p = new AsyncCallListNode;
	//AsyncCallListNode *p = MallocAsyncCallListNode();
	if(!p)
		return -1;
	
	p->pObj = pObj;
	p->cmd = cmd;
	p->param = param;
	p->pData = pData;

	{
		CToolLock lock(&m_mutex);
		m_list_req.Push(p);
		if(m_b_sleep)
			m_cond.WakeUp();
	}

	return 0;
}
