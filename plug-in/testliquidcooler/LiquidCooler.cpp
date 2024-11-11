#include "LiquidCooler.h"
#include "DeviceCommType.h"


enum TIMER_LiquidCooler
{
	TM_ORDINARY_DATA,
};

enum AT_LiquidCooler
{
	AT_QUERY_DATA,
};

TestLiquidCooler::TestLiquidCooler():m_n_query_step(0)
{
	
}

TestLiquidCooler::~TestLiquidCooler()
{
	
}

int  TestLiquidCooler::Initialize(int cmd, ASyncCallDataInst & initdata)
{
	if(MODBUS_RTU == cmd)
	{
		ModbusRtuComm* pRtu = (ModbusRtuComm*)initdata.data();
		if(m_modbus_proxy.CreateCtx(pRtu->PortDev, pRtu->nBaud, pRtu->nDataBit, pRtu->nParity, pRtu->nStopBit))
			return -1;

		if(m_modbus_proxy.InitCtx(pRtu->nSlaveId, 500))
			return -1;

		if(m_modbus_proxy.Connect())
			return -1;
		
		if(m_modbus_proxy.EnableRs485())
			return -1;
	}
	else if(MODBUS_TCP_CLI == cmd)
	{
		ModbusTcpCliComm* pData = (ModbusTcpCliComm*)initdata.data();
		log_info("cmd = %d, addr = %s, port = %d, slaveid = %d", cmd,pData->addr,pData->nPort,pData->nSlaveId);
		if(m_modbus_proxy.CreateCtx(pData->addr, pData->nPort))
			return -1;

		if(m_modbus_proxy.InitCtx(pData->nSlaveId, 200))
			return -1;

		m_modbus_proxy.Connect();
	}

	m_timer_query_nomal_info.InitTimerNode(this, TM_ORDINARY_DATA);
	m_modbus_query_data.fill(0, sizeof(UINT16)*10);
	return 0;
}

void TestLiquidCooler::StartDevice()
{
	m_timer_query_nomal_info.StartTimer(m_standardly_interval);
}

int  TestLiquidCooler::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case TM_ORDINARY_DATA:
		{
			SendAsyncAction(AT_QUERY_DATA);
			break;
		}
		default:
			break;
	}
}


void TestLiquidCooler::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq)
{
	switch(cmd)
	{
		case AT_QUERY_DATA:
		{
			OnQueryDataReq();
			break;
		}
		default:
			break;
	}
	
}

void TestLiquidCooler::DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp)
{
	switch(cmd)
	{
		case AT_QUERY_DATA:
		{
			OnQueryDataResp(param);
			break;
		}
		default:
			break;
	}
}

void TestLiquidCooler::OnQueryDataReq()
{
	int nStep = m_n_query_step % 5;
	switch(m_n_query_step)
	{
		case 0:
		{
			int ret = m_modbus_proxy.ReadInputRegisters(0xE700, 10, (UINT16*)m_modbus_query_data.data());
			SendSyncAction(AT_QUERY_DATA, ret);
			break;
		}
		case 1:
		{
			int ret = m_modbus_proxy.ReadInputRegisters(0xE70B, 10, (UINT16*)m_modbus_query_data.data());
			SendSyncAction(AT_QUERY_DATA, ret);
			break;
		}
		case 2:
		{
			int ret = m_modbus_proxy.ReadInputRegisters(0xE715, 9, (UINT16*)m_modbus_query_data.data());
			SendSyncAction(AT_QUERY_DATA, ret);
			break;
		}
		default:
			break;
	}
}


/*UINT16 m_work_mode;			//工作模式
UINT16 m_work_state;			//工作状态
UINT16 m_warn_level;			//当前告警最高等级
UINT16 m_vfd_fault;				//变频器故障
UINT16 m_temp_cool;				//水温制冷点
UINT16 m_temp_heat;				//水温加热点
UINT16 m_temp_water_out;		//出水温度
UINT16 m_temp_water_back;		//回水温度
UINT16 m_pressure_back;			//进水压力
UINT16 m_pressure_out;			//出水压力
UINT16 m_tmep_enev;				//环境温度
UINT16 m_temp_exhaust;			//排气温度
UINT16 m_water_pump_state;		//水泵状态
UINT16 m_water_pump_speed;		//水泵转速
UINT16 m_compressor_state;		//压缩机状态*/


void TestLiquidCooler::OnQueryDataResp(INT32 ret)
{
	int nStep = m_n_query_step % 5;
	switch(nStep)
	{
		case 0://0xE700 10
		{
			UINT16* pData = (UINT16*)m_modbus_query_data.data();
			m_LiquidCoolerData->m_temp_water_out = pData[0xE700 - 0xE700];
			m_LiquidCoolerData->m_temp_water_back = pData[0xE701 - 0xE700];
			m_LiquidCoolerData->m_temp_exhaust = pData[0xE702 - 0xE700];
			m_LiquidCoolerData->m_tmep_enev = pData[0xE703 - 0xE700];
			m_LiquidCoolerData->m_pressure_back = pData[0xE704 - 0xE700];
			m_LiquidCoolerData->m_pressure_out = pData[0xE705 - 0xE700];
			break;
		}
		case 1:
		{
			//int ret = m_modbus_proxy.ReadInputRegisters(0xE70B, 10, (UINT16*)m_modbus_query_data.data());
			//SendSyncAction(AT_QUERY_DATA, ret);
			break;
		}
		case 2:
		{
			UINT16* pData = (UINT16*)m_modbus_query_data.data();
			m_LiquidCoolerData->m_warn_level = pData[0xE715 - 0xE715];
			m_LiquidCoolerData->m_water_pump_speed = pData[0xE71A - 0xE715];
			m_LiquidCoolerData->m_water_pump_state = pData[0xE71B - 0xE715];
			m_LiquidCoolerData->m_compressor_state = pData[0xE71D - 0xE715];
			SendSyncAction(AT_QUERY_DATA, ret);
			break;
		}
		default:
			break;
	}
	++m_n_query_step;
	m_timer_query_nomal_info.StartTimer(m_standardly_interval);
}


extern "C" Device* CreateDevice(const std::string& param)
{
	return new TestLiquidCooler;
}

