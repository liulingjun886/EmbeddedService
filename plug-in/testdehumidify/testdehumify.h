#include "DehumidifierDevice.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"
#include "Log.h"
#include "ModbusOperater.h"

class TestDehumidify : public DehumidifierDevice, public CAsyncCallSupport, public CTimeEventHander 
{
public:
	TestDehumidify();
	~TestDehumidify();
public:
	virtual int  Initialize(int cmd, ASyncCallDataInst & initdata);
	virtual int  ProcessTimeOutEvent(UINT32 nTimeId);											//定时器消息处理
	virtual void StartDevice();      															//启用设备
	virtual void StopDevice();	        														//停用设备
	virtual void DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq);				//处理异步请求(工作线程执行)
	virtual void DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp);				//处理同步请求
private:
	ModbusOperator m_modbus_proxy;				//modbus代理
	ASyncCallDataInst m_query_data;				//当前查询的数据
	CTimer m_timer_ordinary_data;				//查询普通数据
};