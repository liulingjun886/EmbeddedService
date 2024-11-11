#pragma once
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"
#include "core/TimeNode.h"

class DiDevice : public CAsyncCallSupport, public CTimeEventHander
{
public:
	DiDevice();
	~DiDevice();
	
public:
	int InitDi(int nPinValue, int msec=200);
	int GetValue() const;
	void Start();
	virtual void DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq);
	virtual void DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp);
	virtual int ProcessTimeOutEvent(UINT32 nTimeId);	
	
private:
	int m_n_value;
	int m_n_last_value;
	int m_n_pin;
	int m_n_update_time;
	CTimer m_timer_update;
};