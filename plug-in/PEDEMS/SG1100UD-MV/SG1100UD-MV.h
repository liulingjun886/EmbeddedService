#pragma once
#include "PVDevice.h"
#include "ModbusOperater.h"
#include "core/TimeNode.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"

#pragma pack(1)

#pragma pack()


class MainDevice : public PVDevice, public CAsyncCallSupport, public CTimeEventHander
{
public:
	MainDevice();
	~MainDevice();
public:
	virtual int  Initialize(int cmd, ASyncCallDataInst& initdata);
	virtual void DoAsyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pReq);			   //处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pResp);			   //处理同步请求(主线程执行)
	virtual int  ProcessTimeOutEvent(UINT32 nTimeId);										//定时器处理函数

	virtual int TurnOnOrOff(RUNSTATE s);
	virtual int SetPower(UINT32 p);

	virtual void StartDevice();

private:
	void QueryCriticalData();
	void onQueryCriticalData(INT32 ret, ASyncCallDataInst& pResp);

	void QueryImPortantData();
	void onQueryImPortantData(INT32 ret, ASyncCallDataInst& pResp);

	void QueryUnImPortantData();
	void onQueryUnImPortantData(INT32 ret, ASyncCallDataInst& pResp);
private:
	CTimer m_timer_critical_data;
	CTimer m_timer_important_data;
	CTimer m_timer_unimportant_data;
	ModbusOperator m_modbus_proxy;
	ASyncCallDataInst m_vec_critical_data;
};