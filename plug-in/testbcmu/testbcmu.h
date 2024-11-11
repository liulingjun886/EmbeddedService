#pragma once
#include "BCMUDevice.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"
#include "Log.h"
#include "ModbusOperater.h"


class TestBcmu : public BCMUDevice, public CAsyncCallSupport, public CTimeEventHander
{
public:
	virtual int  Initialize(int cmd, ASyncCallDataInst & initdata);
	virtual void PowerOnOrOff(int nState);
	virtual int  ProcessTimeOutEvent(UINT32 nTimeId);
	virtual void DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq);				//处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp);	
	virtual void StartDevice();
	virtual float GetMaxChargePower();				//获取当前最大充电功率
	virtual float GetMaxDisChargePower();				//获取当前最大放电功率
private:
	void QueryOrdinaryData();
	void OnQueryOrdinaryData();
private:
	CTimer m_timer_query_basic_info;
	CTimer m_timer_query_nomal_info;
	ModbusOperator m_modbus_proxy;
	ASyncCallDataInst m_modbus_basic_data;
	ASyncCallDataInst m_modbus_query_data;
	UINT32 m_n_query_step;
};