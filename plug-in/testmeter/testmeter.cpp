#include "testmeter.h"
#include "DeviceCommType.h"


enum TIMER_ID
{
	TM_QUERY_REALTIME_DATA,
	TM_QUERY_ORDINARY_DATA,
};

enum METERACTION
{
	AT_QUERY_REALTIME_DATA,
	AT_QUERY_ORDINARY_DATA
};

int TestMeter::Initialize(int cmd, ASyncCallDataInst & initdata)
{
	if(MODBUS_RTU == cmd)
	{
		ModbusRtuComm* pRtu = (ModbusRtuComm*)initdata.data();
		if(m_modbus_proxy.CreateCtx(pRtu->PortDev, pRtu->nBaud, pRtu->nDataBit, pRtu->nParity, pRtu->nStopBit))
		{
			log_error("m_modbus_proxy create failer %s", m_pDevBasicInfo->m_str_device_sn);
			//return -1;
		}
		if(m_modbus_proxy.InitCtx(pRtu->nSlaveId, 500))
		{
			log_error("m_modbus_proxy InitCtx failer %s", m_pDevBasicInfo->m_str_device_sn);
			//return -1;
		}
		m_modbus_proxy.Connect();
	}
	
	m_timer_ordinary_data.InitTimerNode(this, TM_QUERY_ORDINARY_DATA);
	m_timer_realtime_data.InitTimerNode(this, TM_QUERY_REALTIME_DATA);
	return 0;
}

int TestMeter::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case TM_QUERY_REALTIME_DATA:
		{
			SendAsyncAction(AT_QUERY_REALTIME_DATA);
			break;
		}
		case TM_QUERY_ORDINARY_DATA:
		{
			SendAsyncAction(AT_QUERY_ORDINARY_DATA);
			break;
		}
	}
	return 0;
}

void TestMeter::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq)				//处理异步请求(工作线程执行)
{
	log_debug("");
	switch(cmd)
	{
		case AT_QUERY_REALTIME_DATA:
		{
			INT32 data = -1;
			int ret = m_modbus_proxy.ReadRegisters(0x016A, 2, (UINT16 *)&data);
			SendSyncAction(TM_QUERY_REALTIME_DATA, data);
			//SendSyncAction(TM_QUERY_REALTIME_DATA, data);
			break;
		}
		case AT_QUERY_ORDINARY_DATA:
		{
			SendSyncAction(AT_QUERY_ORDINARY_DATA);
			break;
		}
	}
}

void TestMeter::DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp)
{
	log_debug("cmd = %d", cmd);
	switch(cmd)
	{
		case AT_QUERY_REALTIME_DATA:
		{
			m_pMeterData->m_p_total = param;
			m_timer_realtime_data.StartTimer(m_quickly_interval);
			break;
		}
		case AT_QUERY_ORDINARY_DATA:
		{
			m_timer_ordinary_data.StartTimer(m_slowly_interval);
			break;
		}
	}
}

void TestMeter::StartDevice()      			//启用设备
{
	m_timer_ordinary_data.StartTimer(m_quickly_interval);
	m_timer_realtime_data.StartTimer(m_standardly_interval);
}

void TestMeter::StopDevice()	        		//停用设备
{
	m_timer_ordinary_data.StopTimer();
	m_timer_realtime_data.StopTimer();
}


extern "C" Device* CreateDevice(const std::string& param)
{
	return new TestMeter;
}

