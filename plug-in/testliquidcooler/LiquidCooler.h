#pragma once

#include "LiquidCoolerDevice.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"
#include "Log.h"
#include "ModbusOperater.h"

class TestLiquidCooler : public LiquidCoolerDevice, public CAsyncCallSupport, public CTimeEventHander	
{
public:
	TestLiquidCooler();
	~TestLiquidCooler();
public:
	virtual int  Initialize(int cmd, ASyncCallDataInst & initdata);
	virtual void StartDevice();
	virtual void DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq);				//处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp);	
	virtual int  ProcessTimeOutEvent(UINT32 nTimeId);

private:
	void OnQueryDataReq();
	void OnQueryDataResp(INT32 ret);
private:
	ModbusOperator m_modbus_proxy;
	CTimer m_timer_query_nomal_info;
	UINT32 m_n_query_step;
	ASyncCallDataInst m_modbus_query_data;
};