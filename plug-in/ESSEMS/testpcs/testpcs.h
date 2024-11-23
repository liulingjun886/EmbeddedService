#pragma once
#include "PCSDevice.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"
#include "Log.h"
#include "ModbusOperater.h"

class TestPcs : public PCSDevice, public CAsyncCallSupport, public CTimeEventHander
{
public:
	TestPcs();
	~TestPcs();
	
public:
	virtual int  Initialize(int cmd, ASyncCallDataInst & initdata);
	virtual int  ProcessTimeOutEvent(UINT32 nTimeId);											//定时器消息处理
	virtual void StartDevice();      															//启用设备
	virtual void StopDevice();	        														//停用设备
	virtual void DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq);				//处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp);				//处理同步请求
	virtual int  PowerOnOrOff(PCSStATECTRL state);												//开机/关机
	virtual int  SetP(float p);																	//设置有功功率 正放 负充 单位kw
	virtual int  SetP(float pA,float pB, float pC)												//设置有功功率 正放 负充 单位kw
	{
		return 0.f;
	}
	virtual int  Reset()																		//复位
	{
		return 0;
	}
	
	
	
	virtual int SetAppaP(float p)					//设置无功功率
	{
		return 0;
	}
	
	virtual int SetPowerFactor(float factor)		//设置功率因数
	{
		return 0;
	}
	
	virtual int GetRatePower()						//获取PCS额定功率
	{
		return 0;
	}

	
private:
	void OnPcsPowerOn();
	void OnPcsPowerOff();
	int  OnQueryData();
	void OnQueryDataCallBack();
private:
	CTimer m_timer_ordinary_data;				//查询普通数据
	CTimer m_timer_realtime_data;				//查询紧要数据
	ASyncCallDataInst m_query_data;				//当前查询的数据
	ASyncCallDataInst m_query_urgent_data;		//当前查询的紧要数据
	
	ModbusOperator m_modbus_proxy;				//modbus代理
	
	CTimer m_timer_open_pcs;					//PCS 开机监视
	CTimer m_timer_close_pcs;					//PCS 关机监视
	
	float  m_set_power;							//当前设置的功率
	int    m_query_failer_cnt;					//查询失败次数
	int	   m_query_index;						//当前查询索引号
};