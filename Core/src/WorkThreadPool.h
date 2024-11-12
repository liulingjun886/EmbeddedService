#pragma once
#include "ShareData.h"
#include <vector>
class CWorkThread;
class CAsyncCallSupport;

class CThreadPool
{
public:
	static CThreadPool* GetInstance();
public:
	CThreadPool();
	~CThreadPool();
public:
	int Init(int num);
	int AddThreads(int num);
	int GetWorkThreadNum() const;
	int AddAsyncCallReq(CAsyncCallSupport* pObj,INT32 cmd, ASyncCallDataInst& pData, INT32 param);
	void Stop();
private:
	int m_work_thread_num;
	CWorkThread *m_pWorkThreads;
	std::vector<CWorkThread *> m_vec_WorkThreads;
};