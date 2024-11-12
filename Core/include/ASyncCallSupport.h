#pragma once

#include "UserTypeDefines.h"
#include "ShareData.h"

class CAsyncCallSupport
{
	friend class CWorkThread;
	friend class CCmdOpt;
public:
	CAsyncCallSupport();
	virtual ~CAsyncCallSupport();
	
protected:
	virtual void DoAsyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pReq);			   //处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pResp);			   //处理同步请求(主线程执行)
	
public:
	int  GetThreadIndex();					//获取工作线程索引
	void SetThreadIndex(int nIndex);		//设置工作线程
	void SetDestroylater();					//因有异步操作，这个是延迟销毁

	void SendSyncAction(INT32 cmd, INT32 param = 0, ASyncCallDataInst& callData = ASyncCallDataInst::GetInstance()); //发送同步请求函数
	void SendSyncAction(INT32 cmd, ASyncCallDataInst& callData);														//重载发送同步请求函数

protected:
	void SendAsyncAction(INT32 cmd, INT32 param = 0, ASyncCallDataInst& callData = ASyncCallDataInst::GetInstance()); 				//发送异步请求
	void SendAsyncAction(INT32 cmd,    ASyncCallDataInst& callData); 				//发送异步请求
	
private:
	int m_n_thread_index;
	bool m_b_destroy;
	//CTimer m_timer_destroy;
};