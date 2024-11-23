#include "testbcmu.h"
#include "DeviceCommType.h"


enum TIMER_ID
{
	TM_QUERY_BASIC_INFO,
	TM_QUERY_NOMAL_INFO,
};

enum BCMUACTION
{
	AT_POWER_ON_OFF,
	AT_QUERY_BASIC_INFO,
	AT_QUERY_NOMAL_INFO,
};

int TestBcmu::Initialize(int cmd, ASyncCallDataInst & initdata)
{
	if(MODBUS_TCP_CLI == cmd)
	{
		ModbusTcpCliComm* pData = (ModbusTcpCliComm*)initdata.data();
		log_info("cmd = %d, addr = %s, port = %d, slaveid = %d", cmd,pData->addr,pData->nPort,pData->nSlaveId);
		if(m_modbus_proxy.CreateCtx(pData->addr, pData->nPort))
			return -1;

		if(m_modbus_proxy.InitCtx(pData->nSlaveId, 200))
			return -1;

		m_modbus_proxy.Connect();
	}
	m_timer_query_basic_info.InitTimerNode(this, TM_QUERY_BASIC_INFO);
	m_timer_query_nomal_info.InitTimerNode(this, TM_QUERY_NOMAL_INFO);

	m_modbus_basic_data.fill(0,7*2);
	m_modbus_query_data.fill(0,240*2);
	return 0;
}

void TestBcmu::PowerOnOrOff(int nState)
{
	SendAsyncAction(AT_POWER_ON_OFF, nState);
	return;
}

int TestBcmu::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case TM_QUERY_BASIC_INFO:
		{
			SendAsyncAction(AT_QUERY_BASIC_INFO);
			break;
		}
		case TM_QUERY_NOMAL_INFO:
		{
			SendAsyncAction(AT_QUERY_NOMAL_INFO);
			break;
		}
	}
	return 0;
}

void TestBcmu::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq)				//处理异步请求(工作线程执行)
{
	switch(cmd)
	{
		case AT_POWER_ON_OFF:
		{
			UINT16 command = (param == BMSPOWERON ? 1 : 0);
			int ret = m_modbus_proxy.WriteRegister(0x0001,command);
			break;
		}
		case AT_QUERY_BASIC_INFO:
		{
			UINT16* pData = (UINT16*)m_modbus_basic_data.data();
			int ret = m_modbus_proxy.ReadInputRegisters(0x2001, 7, pData);
			SendSyncAction(AT_QUERY_BASIC_INFO,ret);
			break;
		}
		case AT_QUERY_NOMAL_INFO:
		{
			QueryOrdinaryData();
			break;
		}
	}
}

void TestBcmu::QueryOrdinaryData()
{
	switch(m_n_query_step)
	{
		case 0:
		{
			UINT16* pData = (UINT16*)m_modbus_query_data.data();
			int ret = m_modbus_proxy.ReadInputRegisters(0x200A, 11, pData);
			SendSyncAction(AT_QUERY_NOMAL_INFO,ret);
			break;
		}
		case 1:
		{
			UINT16* pData = (UINT16*)m_modbus_query_data.data();
			int ret = m_modbus_proxy.ReadInputRegisters(0x2015, 10, pData);
			SendSyncAction(AT_QUERY_NOMAL_INFO,ret);
			break;
		}
	}
}


void TestBcmu::DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp)
{
	log_debug("bcmu cmd = %d,ret = %d", cmd,param);
	switch(cmd)
	{
		case AT_QUERY_BASIC_INFO:
		{
			UINT16* pData = (UINT16*)m_modbus_basic_data.data();
			m_pBcmu->m_u16_state_maincontactor = (pData[2] & 0x1) > 0 ? 1 : 0;
			m_pBcmu->m_u16_state_precontactor = (pData[2] & 0x2) > 0 ? 1 : 0;
			m_pBcmu->m_u16_state_dc_circuit_breaker = (pData[2] & 0x10) > 0 ? 1 : 0;
			m_pBcmu->m_u16_state_ac_circuit_breaker = (pData[2] & 0x20) > 0 ? 1 : 0;

			m_pBcmu->m_u16_alarm[1] = pData[4];
			m_pBcmu->m_u16_alarm[2] = pData[5];
			m_pBcmu->m_u16_alarm[3] = pData[6];
			m_timer_query_basic_info.StartTimer(m_quickly_interval);
			break;
		}
	}
}

void TestBcmu::StartDevice()
{
	m_timer_query_basic_info.StartTimer(m_quickly_interval);
	m_timer_query_nomal_info.StartTimer(m_standardly_interval);
}

float TestBcmu::GetMaxChargePower()				//获取当前最大充电功率
{
	return m_pBcmu->m_u16_volt_total * m_pBcmu->m_u16_curr_max_charge;
}

float TestBcmu::GetMaxDisChargePower()				//获取当前最大放电功率
{
	return m_pBcmu->m_u16_volt_total * m_pBcmu->m_u16_curr_max_discharge;
}


extern "C" Device* CreateDevice(const std::string& param)
{
	return new TestBcmu;
}

