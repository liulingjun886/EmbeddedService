#pragma once
#include "MeterDevice.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"
#include "Log.h"
#include "ModbusOperater.h"

class TestMeter : public MeterDevice, public CAsyncCallSupport, public CTimeEventHander
{
public:
	virtual int Initialize(int cmd, ASyncCallDataInst & initdata);
	virtual int ProcessTimeOutEvent(UINT32 nTimeId);
	virtual void DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq);				//处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp);
	virtual void StartDevice();      			//启用设备
	virtual void StopDevice();	        		//停用设备

private:
	CTimer m_timer_ordinary_data;
	CTimer m_timer_realtime_data;
	ModbusOperator m_modbus_proxy;
};