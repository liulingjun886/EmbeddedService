#include "ASyncCallSupport.h"
#include "Reactor.h"
#include "WorkThreadPool.h"
#include "CmdOpt.h"


CAsyncCallSupport::CAsyncCallSupport():m_n_thread_index(-1)//,m_b_destroy(false)
{
	
}

CAsyncCallSupport::~CAsyncCallSupport()
{
	
}


int CAsyncCallSupport::GetThreadIndex()
{
	return m_n_thread_index;
}

void CAsyncCallSupport::SendAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& callData)
{
	assert(m_n_thread_index >= 0);
	CThreadPool::GetInstance()->AddAsyncCallReq(this, cmd, callData, param);
}

void CAsyncCallSupport::SendAsyncAction(INT32 cmd, ASyncCallDataInst& callData)
{
	SendAsyncAction(cmd, 0, callData);
}

void CAsyncCallSupport::SetDestroylater()
{
	
}

void CAsyncCallSupport::SetThreadIndex(int nIndex)
{
	assert(nIndex >= 0 && nIndex < CThreadPool::GetInstance()->GetWorkThreadNum());
	m_n_thread_index = nIndex;
}

void CAsyncCallSupport::SendSyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& callData)
{
	CCmdOpt::GetInstance()->SendSyncAction(this, cmd, callData, ret);
}

void CAsyncCallSupport::SendSyncAction(INT32 cmd, ASyncCallDataInst& callData)
{
	SendSyncAction(cmd,0,callData);
}

void CAsyncCallSupport::DoAsyncAction(INT32 cmd, INT32 param,ASyncCallDataInst& pReq)
{
	return;
}

void CAsyncCallSupport::DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp)
{
	return;
}

