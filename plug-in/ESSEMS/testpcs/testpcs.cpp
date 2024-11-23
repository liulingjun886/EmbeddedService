#include "testpcs.h"
#include "DeviceCommType.h"
enum PCSCOMMAND
{
	AT_POWER_ON_OFF = 1,
	AT_SET_ACTIVE_POWER,
	AT_SET_REACTIVE_POWER,
	AT_QUERY_REALTIME_DATA,
	AT_QUERY_ORDINARY_DATA,
};

enum PCS_TIMER
{
	TM_OPENING=1,
	TM_CLOSING,
	TM_REALTIME_DATA,
	TM_ORDINARY_DATA,
};

struct URGENT_DATA
{
	UINT16 power;
	UINT16 work_state;
};

TestPcs::TestPcs():m_set_power(0.f),m_query_index(0)
{
	
}

TestPcs::~TestPcs()
{
	
}


int TestPcs::Initialize(int cmd, ASyncCallDataInst & initdata)
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

		if(m_modbus_proxy.InitCtx(pData->nSlaveId, 500))
			return -1;

		m_modbus_proxy.Connect();
	}
	
	m_query_data.fill(0, 32);
	m_query_urgent_data.fill(0, sizeof(URGENT_DATA));
	m_timer_open_pcs.InitTimerNode(this, TM_OPENING);
	m_timer_close_pcs.InitTimerNode(this, TM_CLOSING);
	m_timer_ordinary_data.InitTimerNode(this, TM_ORDINARY_DATA);
	m_timer_realtime_data.InitTimerNode(this, TM_REALTIME_DATA);
	return 0;
}

void TestPcs::StartDevice()      			//启用设备
{
	m_timer_realtime_data.StartTimer(m_quickly_interval);
	m_timer_ordinary_data.StartTimer(m_standardly_interval);
	log_debug("pcs startDevice");
}

void TestPcs::StopDevice()	        		//停用设备
{
	//m_timer_query_urgent.StopTimer();
	//m_timer_query.StopTimer();
}

int TestPcs::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case TM_OPENING:
		{
			if(RUNNING == m_pcs_data->m_u16_work_state)
			{
				m_timer_open_pcs.StopTimer();
				OnPcsPowerOn();
			}
			else
			{
				SendAsyncAction(AT_POWER_ON_OFF,1);
				m_timer_open_pcs.StartTimerSec(2);
			}
			break;
		}
		case TM_CLOSING:
		{
			if(PCSSTOP == m_pcs_data->m_u16_work_state)
			{
				m_timer_close_pcs.StopTimer();
				OnPcsPowerOff();
			}
			else
			{
				SendAsyncAction(AT_POWER_ON_OFF,0);
				m_timer_close_pcs.StartTimerSec(2);
			}
			break;
		}
		case TM_ORDINARY_DATA:
		{
			SendAsyncAction(AT_QUERY_ORDINARY_DATA);
			break;
		}
		case TM_REALTIME_DATA:
		{
			SendAsyncAction(AT_QUERY_REALTIME_DATA);
			break;
		}
	}
	return 0;
}


int TestPcs::PowerOnOrOff(PCSStATECTRL state)				//开机/关机
{
	if(state == PCSPOWERON)
	{
		if(RUNNING == m_pcs_data->m_u16_work_state || m_timer_open_pcs.IsActive())
			return 0;
		SendAsyncAction(AT_POWER_ON_OFF,1);
		m_timer_open_pcs.StartTimerSec(5);
	}
	else
	{
		if(PCSSTOP == m_pcs_data->m_u16_work_state || m_timer_close_pcs.IsActive())
			return 0;
		SendAsyncAction(AT_POWER_ON_OFF,0);
		m_timer_close_pcs.StartTimerSec(5);
	}
	return 0;
}

int TestPcs::SetP(float p)
{
	m_set_power = p;
	if(RUNNING != m_pcs_data->m_u16_work_state)
		return -1;
	INT32 power = (INT32)(m_set_power*10);
	SendAsyncAction(AT_SET_ACTIVE_POWER, power);
	return 0;
}

void TestPcs::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq)
{	
	switch(cmd)
	{
		case AT_POWER_ON_OFF:
		{
			int ret = m_modbus_proxy.WriteRegister(0x0291, param);
			break;
		}
		case AT_SET_ACTIVE_POWER:
		{
			m_modbus_proxy.WriteRegister(0x0D57, param);
			break;
		}
		case AT_SET_REACTIVE_POWER:
		{
			break;
		}
		case AT_QUERY_REALTIME_DATA:
		{
			URGENT_DATA* pData = (URGENT_DATA*)m_query_urgent_data.data();
			//m_modbus_proxy.SetResponseTimeOut(m_realtime_interval);
			if(m_modbus_proxy.ReadRegisters(0x6057, 1, &(pData->work_state)) < 0)
			{
				SendSyncAction(AT_QUERY_REALTIME_DATA, -1);
				return;
			}
			if(m_modbus_proxy.ReadRegisters(0x6039, 1, &(pData->power)) < 0)
			{
				SendSyncAction(AT_QUERY_REALTIME_DATA, -2);
				return;
			}
			SendSyncAction(AT_QUERY_REALTIME_DATA, 0);
			break;
		}
		case AT_QUERY_ORDINARY_DATA:
		{
			OnQueryData();
			break;
		}
		default:
			break;
	}
}

void TestPcs::DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp)
{
	log_info("cmd = %d, param = %d",cmd,param);
	switch(cmd)
	{
		case AT_QUERY_REALTIME_DATA:
		{
			if(0 == param)
			{
				URGENT_DATA* pData = (URGENT_DATA*)m_query_urgent_data.data();
				m_pcs_data->m_p_ac_total = pData->power*0.1f;
				if(m_pcs_data->m_u16_work_state != pData->work_state)
				{
					if(RUNNING == pData->work_state)
						OnPcsPowerOn();
					else if(PCSSTOP == pData->work_state)
						OnPcsPowerOff();
					m_pcs_data->m_u16_work_state = pData->work_state;
				}
			}
			m_timer_realtime_data.StartTimer(m_quickly_interval);
			break;
		}
		case AT_QUERY_ORDINARY_DATA:
		{
			if(param > 0)
			{
				OnQueryDataCallBack();
			}
			m_timer_ordinary_data.StartTimer(m_standardly_interval);
			break;
		}
		
	}
}

void TestPcs::OnPcsPowerOn()
{
	SetP(m_set_power);
}

void TestPcs::OnPcsPowerOff()
{
	SetP(0);
}

int  TestPcs::OnQueryData()
{
	int ret = -1;
	switch(m_query_index%4)
	{
		case 0:
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			ret = m_modbus_proxy.ReadRegisters(0x6020, 12, pData);
			break;
		}
		case 1:
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			ret = m_modbus_proxy.ReadRegisters(0x6030, 9, pData);
			break;
		}
		case 2:
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			ret = m_modbus_proxy.ReadRegisters(0x6050, 12, pData);
			break;
		}
		case 3:
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			ret = m_modbus_proxy.ReadRegisters(0x1700, 8, pData);
			break;
		}
		default:
			break;
	}
	SendSyncAction(AT_QUERY_ORDINARY_DATA, ret);
	return 0;
}


void TestPcs::OnQueryDataCallBack()
{
	switch(m_query_index%4)
	{
		case 0: //0x6020 ~ 0x602B
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			m_pcs_data->m_volt_ab = pData[0x6020-0x6020];
			m_pcs_data->m_volt_bc = pData[0x6021-0x6020];
			m_pcs_data->m_volt_ca = pData[0x6022-0x6020];
			m_pcs_data->m_volt_out_a = pData[0x6023-0x6020];
			m_pcs_data->m_volt_out_b = pData[0x6024-0x6020];
			m_pcs_data->m_volt_out_c = pData[0x6025-0x6020];
			m_pcs_data->m_curr_out_a = pData[0x6026-0x6020];
			m_pcs_data->m_curr_out_b = pData[0x6027-0x6020];
			m_pcs_data->m_curr_out_c = pData[0x6028-0x6020];
			m_pcs_data->m_curr_indu_a = pData[0x6029-0x6020];
			m_pcs_data->m_curr_indu_b = pData[0x602A-0x6020];
			m_pcs_data->m_curr_indu_c = pData[0x602B-0x6020];
			break;
		}
		case 1: //0x6030 ~ 0x603C
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			m_pcs_data->m_p_ac_a = pData[0x6030-0x6030]; 		// 交流 A 相有功功率
			m_pcs_data->m_p_ac_b = pData[0x6031-0x6020]; 		// 交流 B 相有功功率
			m_pcs_data->m_p_ac_c = pData[0x6032-0x6020]; 		// 交流 C 相有功功率
			m_pcs_data->m_appap_ac_a = pData[0x6033-0x6020]; 	// 交流 A 相视在功率
			m_pcs_data->m_appap_ac_b = pData[0x6034-0x6020]; 	// 交流 B 相视在功率
			m_pcs_data->m_appap_ac_c = pData[0x6035-0x6020]; 	// 交流 C 相视在功率
			m_pcs_data->m_reacp_ac_a = pData[0x6036-0x6020]; 	// 交流 A 相无功功率
			m_pcs_data->m_reacp_ac_b = pData[0x6037-0x6020]; 	// 交流 B 相无功功率
			m_pcs_data->m_reacp_ac_c = pData[0x6038-0x6020]; 	// 交流 C 相无功功率
			m_pcs_data->m_p_ac_total = pData[0x6039-0x6020]; 	// 交流输出总有功功率
			m_pcs_data->m_appap_ac_total = pData[0x603A-0x6030]; // 交流输出总无功功率
			m_pcs_data->m_reacp_ac_total = pData[0x603B-0x6030]; // 交流输出总视在功率
			m_pcs_data->m_power_factor_ac = pData[0x603C-0x6030];// 交流功率因数
			break;
		}
		case 2:
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			m_pcs_data->m_volt_bus_total = pData[0x6050-0x6050]; 	 // 总母线电压
			m_pcs_data->m_volt_bus_positive = pData[0x6051-0x6050];  // 正母线电压
			m_pcs_data->m_volt_bus_negative = pData[0x6052-0x6050];  // 负母线电压
			m_pcs_data->m_volt_battery = pData[0x6053-0x6050]; 		 // 电池电压
			m_pcs_data->m_curr_battery = pData[0x6054-0x6050]; 		 // 电池电流
			m_pcs_data->m_p_dc = pData[0x6055-0x6050]; 				 // 直流功率
			m_pcs_data->m_p_dc_total = pData[0x6056-0x6050]; 		 // 直流总电流
			UINT16 nWorkState = pData[0x6057-0x6050];
			m_pcs_data->m_u16_work_state = (nWorkState & 0x01) > 0 ? RUNNING:PCSSTOP;	 // 工作状态可不用
			//log_info("m_pcs_data->m_u16_work_state = %d",m_pcs_data->m_u16_work_state);
			m_pcs_data->m_temp_igbt = pData[0x6058-0x6050]; 		 // IGBT 温度
			m_pcs_data->m_temp_env = pData[0x6059-0x6050]; 			 // 环境温度
			m_pcs_data->m_temp_indu = pData[0x605A-0x6050]; 		 // 电感温度
			m_pcs_data->m_u16_work_mode = pData[0x605B-0x6050];		 // 当前实际充放电工作模式
			break;
		}
		case 3:
		{
			UINT16* pData = (UINT16*)m_query_data.data();
			m_pcs_data->m_sz_alarm[0] = pData[0x1700-0x1700];
			m_pcs_data->m_sz_alarm[1] = pData[0x1701-0x1700];
			m_pcs_data->m_sz_alarm[2] = pData[0x1702-0x1700];
			m_pcs_data->m_sz_alarm[3] = pData[0x1703-0x1700];
			m_pcs_data->m_sz_alarm[4] = pData[0x1704-0x1700];
			m_pcs_data->m_sz_alarm[5] = pData[0x1705-0x1700];
			m_pcs_data->m_sz_alarm[6] = pData[0x1706-0x1700];
			m_pcs_data->m_sz_alarm[7] = pData[0x1707-0x1700];
			break;
		}
	}
	m_query_index += 1;
}

extern "C" Device* CreateDevice(const std::string& param)
{
	return new TestPcs;
}

