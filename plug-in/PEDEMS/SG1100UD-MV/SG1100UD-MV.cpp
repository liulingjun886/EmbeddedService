#include "SG1100UD-MV.h"
#include "PublicTool.h"
#include "Log.h"

enum TIMER_ID
{
	TIMER_CRITICAL=0,
	TIMER_IMPORTANT,
	TIMER_UNIMPORTANT
};


enum COMMAND
{
	QUERY_CRItiCAL_DATA=0,
	QUERY_IMPORTANT_DATA,
	QUERY_UNIMPORTANT_DATA,

	POWER_ON_OFF,
	POWER_SET
};


#pragma pack(1)

struct PowerInfo
{
	UINT32 m_p;
	INT32  m_q;
	INT16  m_pf;
	UINT16 m_grid_fre;
};

struct ImportantData
{
	LineVolt 		m_lineVolt;				//线电压
	PhaseCurr 		m_phaseCurr;			//相电流
	PhasePower 		m_active_power;			//相有功功率
	PhasePower 		m_reactive_power;		//相无功功率
	PhasePower 		m_apparent_power;		//相视在功率
	UINT32			m_dc_power;				//直流功率
	UINT16			m_convert_effic;		//转换效率
	INT16			m_inner_temp;			//内部温度
};

struct UnImportantData
{
	UINT32			m_run_time;				//运行时间
	UINT16          m_gen_power_day;		//当日发电量
	UINT16 			m_gen_power_month;		//当月发电量
	UINT32			m_gen_power_year;		//年发电量
	UINT32			m_gen_power_total;		//累计发电量
};

#pragma pack()

MainDevice::MainDevice()
{
	m_timer_critical_data.InitTimerNode(this, TIMER_CRITICAL);
	m_timer_important_data.InitTimerNode(this, TIMER_IMPORTANT);
	m_timer_unimportant_data.InitTimerNode(this, TIMER_UNIMPORTANT);
}

MainDevice::~MainDevice()
{
}


int  MainDevice::Initialize(int cmd, ASyncCallDataInst& initdata)
{
	if(ModbusOperator::InitModbusOperaterHander(&m_modbus_proxy, cmd, initdata.data(),500) < 0)
		return -1;
	//m_modbus_proxy.EnableDebug();
	m_vec_critical_data.fill(0, sizeof(PowerInfo));
	return 0;
}

int  MainDevice::ProcessTimeOutEvent(UINT32 nTimeId)										//定时器处理函数
{
	switch(nTimeId)
	{
		case TIMER_CRITICAL:
		{
			SendAsyncAction(QUERY_CRItiCAL_DATA);
			break;
		}
		case TIMER_IMPORTANT:
		{
			SendAsyncAction(QUERY_IMPORTANT_DATA);
			break;
		}
		case TIMER_UNIMPORTANT:
		{
			SendAsyncAction(QUERY_UNIMPORTANT_DATA);
			break;
		}
		default:
			break;
	}
}


void MainDevice::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pReq)			   //处理异步请求(工作线程执行)
{
	switch(cmd)
	{
		case QUERY_CRItiCAL_DATA:
		{
			QueryCriticalData();
			break;
		}
		case QUERY_IMPORTANT_DATA:
		{
			QueryImPortantData();
			break;
		}
		case QUERY_UNIMPORTANT_DATA:
		{
			QueryUnImPortantData();
			break;
		}
		case POWER_SET:
		{
			UINT16 data = (UINT16)param;
			m_modbus_proxy.WriteRegister(5039,data);
			break;
		}
		case POWER_ON_OFF:
		{
			UINT16 data = (UINT16)param;
			m_modbus_proxy.WriteRegister(5006,data);
		}
		default:
			break;
	}
}

void MainDevice::DoSyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pResp)			   //处理同步请求(主线程执行)
{
	switch(cmd)
	{
		case QUERY_CRItiCAL_DATA:
		{
			onQueryCriticalData(ret,pResp);
			break;
		}
		case QUERY_IMPORTANT_DATA:
		{
			onQueryImPortantData(ret,pResp);
			break;
		}
		case QUERY_UNIMPORTANT_DATA:
		{
			onQueryUnImPortantData(ret,pResp);
			break;
		}
	}
}

int MainDevice::TurnOnOrOff(RUNSTATE s)
{
	INT32 data = 0;
	if(TURNON == s)
		data = 1;
	SendAsyncAction(POWER_ON_OFF,data);
	return 0;
}

int MainDevice::SetPower(UINT32 p)
{
	SendAsyncAction(POWER_SET,(INT32)p);
	return 0;
}

void MainDevice::StartDevice()
{
	m_timer_critical_data.StartTimer(m_critical_interval);
	m_timer_important_data.StartTimer(m_important_interval);
	m_timer_unimportant_data.StartTimer(m_unimportant_interval);
}

void MainDevice::QueryCriticalData()
{
	PowerInfo* pData = (PowerInfo*)m_vec_critical_data.data();
	int ret = m_modbus_proxy.ReadInputRegisters(5031, 6, (UINT16*)pData);
	if(ret > 0)
	{
		pData->m_p = PublicTool::MakeUint32CDAB(pData->m_p);
		pData->m_q = PublicTool::MakeUint32CDAB(pData->m_q);
	}
	else
	{
		log_error("QueryCriticalData ret = %d", ret);
	}
	SendSyncAction(QUERY_CRItiCAL_DATA, ret, m_vec_critical_data);
}

void MainDevice::onQueryCriticalData(INT32 ret, ASyncCallDataInst& pResp)
{
	log_debug("onQueryCriticalData ret = %d",ret);
	PowerInfo* pData = (PowerInfo*)pResp.data();
	if(ret <= 0)
		goto END;
	
	m_pv_data->m_ac_info.m_power_info.m_p = pData->m_p;
	m_pv_data->m_ac_info.m_power_info.m_pq = pData->m_q;
	m_pv_data->m_ac_info.m_power_info.m_pf = pData->m_pf;
	m_pv_data->m_ac_info.m_grid_frequency = pData->m_grid_fre;

	//log_debug("m_pv_data->m_ac_info.m_power_info.m_p = %d",m_pv_data->m_ac_info.m_power_info.m_p);

END:	m_timer_critical_data.StartTimer(m_critical_interval);
}

void MainDevice::QueryImPortantData()
{
	ASyncCallDataInst data;
	data.fill(0,sizeof(ImportantData));
	ImportantData* pData = (ImportantData*)data.data();
	UINT16 data1[14] = {0};
	int ret = m_modbus_proxy.ReadInputRegisters(5017, 14, (UINT16*)data1);
	if(ret > 0)
	{
		pData->m_dc_power = (data1[1] << 16) | data1[0];
		pData->m_lineVolt.m_u_ab = data1[2];
		pData->m_lineVolt.m_u_bc = data1[3];
		pData->m_lineVolt.m_u_ac = data1[4];
		pData->m_phaseCurr.m_i_a = data1[5];
		pData->m_phaseCurr.m_i_b = data1[6];
		pData->m_phaseCurr.m_i_c = data1[7];
		pData->m_active_power.m_p_a = PublicTool::MakeUint32CDAB(data1[8], data1[9]);
		pData->m_active_power.m_p_b = PublicTool::MakeUint32CDAB(data1[10], data1[11]);
		pData->m_active_power.m_p_c = PublicTool::MakeUint32CDAB(data1[12], data1[13]);
	}
	else
	{
		log_error("QueryImPortantData = %d",ret);
	}

	ret = m_modbus_proxy.ReadInputRegisters(5008, 1, (UINT16*)&pData->m_inner_temp);
	ret = m_modbus_proxy.ReadInputRegisters(5037, 1, (UINT16*)&pData->m_convert_effic);

	SendSyncAction(QUERY_IMPORTANT_DATA, ret, m_vec_critical_data);
}

void MainDevice::onQueryImPortantData(INT32 ret, ASyncCallDataInst& pResp)
{
	log_debug("onQueryImPortantData ret = %d", ret);
	ImportantData* pData = (ImportantData*)pResp.data();
	if(ret < 0)
		goto END;
	
	m_pv_data->m_dc_power = pData->m_dc_power;
	m_pv_data->m_ac_info.m_lineVolt.m_u_ab = pData->m_lineVolt.m_u_ab;
	m_pv_data->m_ac_info.m_lineVolt.m_u_bc = pData->m_lineVolt.m_u_bc;
	m_pv_data->m_ac_info.m_lineVolt.m_u_ac = pData->m_lineVolt.m_u_ac;
	m_pv_data->m_ac_info.m_phaseCurr.m_i_a = pData->m_phaseCurr.m_i_a;
	m_pv_data->m_ac_info.m_phaseCurr.m_i_b = pData->m_phaseCurr.m_i_b;
	m_pv_data->m_ac_info.m_phaseCurr.m_i_b = pData->m_phaseCurr.m_i_c;
	m_pv_data->m_ac_info.m_active_power.m_p_a = pData->m_active_power.m_p_a;
	m_pv_data->m_ac_info.m_active_power.m_p_b = pData->m_active_power.m_p_b;
	m_pv_data->m_ac_info.m_active_power.m_p_c = pData->m_active_power.m_p_c;
	m_pv_data->m_convert_effic = pData->m_convert_effic;
	m_pv_data->m_inner_temp = pData->m_inner_temp;

END:	m_timer_important_data.StartTimer(m_important_interval);
}

void MainDevice::QueryUnImPortantData()
{
	ASyncCallDataInst data;
	data.fill(0,sizeof(UnImportantData));
	UnImportantData* pData = (UnImportantData*)data.data();
	UINT16 data1[9] = {0};
	int ret = m_modbus_proxy.ReadInputRegisters(5003, 5, (UINT16*)data1);
	pData->m_gen_power_day = data1[0];
	pData->m_gen_power_total = PublicTool::MakeUint32CDAB(data1[1], data1[2]);
	pData->m_run_time = PublicTool::MakeUint32CDAB(data1[3], data1[4]);

	ret = m_modbus_proxy.ReadInputRegisters(5126, 4, (UINT16*)&data1[5]);
	pData->m_gen_power_year = PublicTool::MakeUint32CDAB(data1[5], data1[6]);
	pData->m_gen_power_month = PublicTool::MakeUint32CDAB(data1[7], data1[8]);
	SendSyncAction(QUERY_UNIMPORTANT_DATA, ret, data);
}

void MainDevice::onQueryUnImPortantData(INT32 ret, ASyncCallDataInst& pResp)
{
	log_debug("onQueryUnImPortantData ret = %d",ret);
	UnImportantData* pData = (UnImportantData*)pResp.data();
	if(ret < 0)
		goto END;

	m_pv_data->m_run_time = pData->m_run_time;
	m_pv_data->m_gen_power_day = pData->m_gen_power_day;
	m_pv_data->m_gen_power_month = pData->m_gen_power_month;
	m_pv_data->m_gen_power_year = pData->m_gen_power_year;
	m_pv_data->m_gen_power_total = pData->m_gen_power_total;
	
END:m_timer_unimportant_data.StartTimer(m_unimportant_interval);
}


extern "C" Device* CreateDevice(const std :: string & param)
{
	return new MainDevice;
}

