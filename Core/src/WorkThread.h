#pragma once

#include "List.h"
#include <pthread.h>
#include "UserTypeDefines.h"
#include "ToolLock.h"
#include "ShareData.h"
#include <vector>

#define MAX_WORKTHREAD_REQ 256

class CAsyncCallSupport;

struct AsyncCallListNode : public CListNode<AsyncCallListNode>
{
	INT32 cmd;
	INT32 param;
	CAsyncCallSupport* pObj;
	ASyncCallDataInst pData;
};

class CWorkThread
{
public:
	CWorkThread();
	~CWorkThread();
public:
	int start();
	void stop();
	void run();
	int AddAsyncCallReq(CAsyncCallSupport* pObj,INT32 cmd, ASyncCallDataInst& pData, INT32 param);	

private:
	AsyncCallListNode *MallocAsyncCallListNode();
	void FreeAsyncCallListNode(AsyncCallListNode* pNode);
	
private:
	bool m_b_sleep;
	pthread_t m_pid;
	CMutexLock m_mutex;
	CWaitCond m_cond;
	CListFIFO<AsyncCallListNode> m_list_req;				//待处理的消息
	// CListFILO<AsyncCallListNode> m_list_rest_data_pool;		//当前闲置内存，如果为空了就要申请大内存了
	// AsyncCallListNode m_sz_block_pool[MAX_WORKTHREAD_REQ];
};