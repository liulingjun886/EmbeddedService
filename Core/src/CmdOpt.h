#pragma once
#include <pthread.h>
#include "PollerObject.h"
#include "ToolLock.h"
#include "List.h"
#include "ShareData.h"

class CAsyncCallSupport;

struct CmdOptPtr : public CListNode<CmdOptPtr>
{
	INT32 cmd;
	INT32 param;
	CAsyncCallSupport* pObj;
	ASyncCallDataInst pData; 
};

class CCmdOpt : public CPollerObject
{
public:
	static CCmdOpt* GetInstance();
	~CCmdOpt();
private:
	CCmdOpt();
public:
	int Init();
	int InPutNotify();
	int OutPutNotify(){return 0;}
	int HunpUpNotiry() {return 0;}
	void SendSyncAction(CAsyncCallSupport* pObj, UINT32 cmd, ASyncCallDataInst& pData, INT32 ret = 0);
private:
	CmdOptPtr* MallocCmdOptPtr();
	void FreeCmdOptPtr(CmdOptPtr* pNode);
private:
	int m_w_fd;
	CMutexLock m_mutex;
	CListFIFO<CmdOptPtr> m_list_aync_callback;
	CListFILO<CmdOptPtr> m_list_rest_data_pool;		//当前闲置内存，如果为空了就要申请大内存了
	CListFILO<CmdOptPtr> m_list_block_pool;			//总内存池，主管申请大块内存
};
