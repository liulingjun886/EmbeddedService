#pragma once
#include "LoadDevice.h"
#include "ModbusOperater.h"
#include "core/TimeNode.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"

#pragma pack(1)

typedef struct stOtherData
{
	UINT16 m_n_ctrl_type;
	UINT16 m_n_gun_num;
	UINT32 m_n_request_power;
	UINT16 m_n_power_crtl_switch;
	UINT16 m_n_power_limit;
	UINT16 m_n_power_limit_offline;
}OtherData;

#pragma pack()

class MainDevice : public LoadDevice, public CAsyncCallSupport, public CTimeEventHander
{
public:
    MainDevice();
    ~MainDevice();

public:
	virtual int  Initialize(int cmd, ASyncCallDataInst& initdata);
	virtual void DoAsyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pReq);			   //处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pResp);			   //处理同步请求(主线程执行)
	virtual int  ProcessTimeOutEvent(UINT32 nTimeId);									   //定时器处理函数
	virtual int TurnOnOrOff(RUNSTATE s);
	virtual int SetPower(float p);
	virtual void StartDevice();
	virtual int  GetDataLen();
protected:
	virtual int SetDataPtr(char * ptr);
private:
	void QueryCriticalData();
	void onQueryCriticalData(INT32 ret, ASyncCallDataInst& pResp);

	void QueryImPortantData();
	void onQueryImPortantData(INT32 ret, ASyncCallDataInst& pResp);

	void QueryUnImPortantData();
	void onQueryUnImPortantData(INT32 ret, ASyncCallDataInst& pResp);

private:
	CTimer m_timer_critical_data;
	ModbusOperator m_modbus_proxy;
	ASyncCallDataInst m_vec_critical_data;
	OtherData* m_pOtherData;
};