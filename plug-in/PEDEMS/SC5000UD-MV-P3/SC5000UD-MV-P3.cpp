#include "SC5000UD-MV-P3.h"
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
	QUERY_UNIMPORTANT_DATA
};

#pragma pack(1)
typedef struct stCriticalData
{
    UINT32 m_pcs_stat;
    UINT32 m_n_alarm1;
    UINT32 m_n_alarm2;

    UINT16 m_n_bat_stat;
    UINT16 m_n_soc;

    UINT32 m_n_p;
    UINT32 m_n_pq;
    UINT32 m_n_pa;
    UINT32 m_n_pf;
    UINT16 m_n_effi;
    UINT16 m_grid_freq;
}CriticalData;

typedef struct stImportantData
{
    UINT16 m_n_dc_volt;
    UINT16 m_n_dc_curr;
    UINT16 m_n_pcs_stat;
    UINT16 m_n_ab_volt;
    UINT16 m_n_bc_volt;
    UINT16 m_n_ca_volt;
    UINT16 m_n_a_curr;
    UINT16 m_n_b_curr;
    UINT16 m_n_c_curr;
    
    


    UINT16 m_can_charge_power;
    UINT16 m_can_discharge_power;
    UINT16 m_bat_total_volt;
    UINT16 m_bat_total_curr;
    UINT16 m_cell_max_soc;
    UINT16 m_cell_min_soc;
    UINT16 m_cell_max_temp;
    UINT16 m_cell_min_temp;
    UINT16 m_cabinet_temp;
}ImportantData;


typedef struct stUnImportantData
{
    UINT16 m_charge_day;
    UINT16 m_discharge_day;
    UINT16 m_charge_time_day;
    UINT16 m_discharge_time_day;
    UINT32 m_run_time_total;
    UINT32 m_charge_total;
    UINT32 m_discharge_total;
    UINT32 m_charge_time_total;
    UINT32 m_discharge_time_total;
}UnImportantData;

#pragma pack()

MainDevice::MainDevice()
{
    m_timer_critical_data.InitTimerNode(this,TIMER_CRITICAL);
    m_timer_important_data.InitTimerNode(this,TIMER_IMPORTANT);
    m_timer_unimportant_data.InitTimerNode(this,TIMER_UNIMPORTANT);
}
MainDevice::~MainDevice()
{

}

int  MainDevice::Initialize(int cmd, ASyncCallDataInst& initdata)                          //初始化设置
{
    if(ModbusOperator::InitModbusOperaterHander(&m_modbus_proxy, cmd, initdata.data(),500) < 0)
		return -1;
    //m_modbus_proxy.EnableDebug();
    return 0;
}
void MainDevice::StartDevice()                                                             //设备启动
{
    m_timer_critical_data.StartTimer(m_critical_interval);
    m_timer_important_data.StartTimer(m_important_interval);
    m_timer_unimportant_data.StartTimer(m_unimportant_interval);
}
int  MainDevice::ProcessTimeOutEvent(UINT32 nTimeId)									    //定时器处理函数
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
    return 0;
}
void MainDevice::DoAsyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pReq)			    //处理异步请求(工作线程执行)
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
        default:
            break;
    }
}

void MainDevice::DoSyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pResp)			    //处理同步请求(主线程执行)
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
        default:
            break;
    }
}

void MainDevice::QueryCriticalData()
{
    ASyncCallDataInst pResp;
    pResp.fill(0,sizeof(CriticalData));
    UINT16 data[16] = {0};
    int ret = m_modbus_proxy.ReadInputRegisters(10737, 8, data);
    CriticalData* pData = (CriticalData*)pResp.data();
    if(ret > 0)
    {
        //CriticalData* pData = pResp.data();
        pData->m_pcs_stat = PublicTool::MakeUint32CDAB(data[0],data[1]);
        pData->m_n_alarm1 = PublicTool::MakeUint32CDAB(data[4],data[5]);
        pData->m_n_alarm2 = PublicTool::MakeUint32CDAB(data[6],data[7]);
    }
    else
    {
        log_error("QueryCriticalData ret = %d", ret);
        goto END;
    }

    ret = m_modbus_proxy.ReadInputRegisters(10805, 4, data);
    if(ret > 0)
    {
        pData->m_n_bat_stat = data[0];
        pData->m_n_soc = data[3];
    }
    else
    {
        log_error("QueryCriticalData ret = %d", ret);
        goto END;
    }

    ret = m_modbus_proxy.ReadInputRegisters(10070,9,data);
    if(ret > 0)
    {
        pData->m_n_p = PublicTool::MakeUint32CDAB(data[2],data[3]);
        pData->m_n_pq = PublicTool::MakeUint32CDAB(data[4],data[5]);
        pData->m_n_pa = PublicTool::MakeUint32CDAB(data[6],data[7]);
        pData->m_n_pf = data[8];
        pData->m_grid_freq = data[0];
        pData->m_n_effi = data[1];
    }
    else
    {
        log_error("QueryCriticalData ret = %d", ret);
    }
END:SendSyncAction(QUERY_CRItiCAL_DATA,ret,pResp);
}

void MainDevice::onQueryCriticalData(INT32 ret, ASyncCallDataInst& pResp)
{
    CriticalData* pData = (CriticalData*)pResp.data();
    if(ret > 0)
    {
        m_ess_data->m_soc = pData->m_n_soc;
        m_ess_data->m_ac_info.m_power_info.m_p = pData->m_n_p;
        m_ess_data->m_ac_info.m_power_info.m_pq = pData->m_n_pq;
        m_ess_data->m_ac_info.m_power_info.m_pa = pData->m_n_pa;
        m_ess_data->m_ac_info.m_power_info.m_pf = pData->m_n_pf;
        m_ess_data->m_ac_info.m_grid_frequency = pData->m_grid_freq;
        m_ess_data->m_alarm[0] = (pData->m_n_alarm1 >> 16);
        m_ess_data->m_alarm[1] = (pData->m_n_alarm1 & 0x0F);
        m_ess_data->m_alarm[2] = (pData->m_n_alarm2 >> 16);
        m_ess_data->m_alarm[3] = (pData->m_n_alarm2 & 0x0F);

        log_debug("m_ess_data->m_ac_info.m_power_info.m_p = %d",m_ess_data->m_ac_info.m_power_info.m_p);
    }
    m_timer_critical_data.StartTimer(m_critical_interval);
}

void MainDevice::QueryImPortantData()
{
    ASyncCallDataInst pResp;
    pResp.fill(0,sizeof(ImportantData));
    UINT16 data[20] = {0};
    ImportantData* pData = (ImportantData*)pResp.data();
    int ret = m_modbus_proxy.ReadInputRegisters(10809, 9, data);
    if(ret > 0)
    {
        pData->m_can_charge_power = data[0];
        pData->m_can_discharge_power = data[1];
        pData->m_bat_total_curr = data[2];
        pData->m_bat_total_volt = data[3];
        pData->m_cell_max_soc = data[4];
        pData->m_cell_min_soc = data[5];
        pData->m_cell_max_temp = data[6];
        pData->m_cell_min_temp = data[7];
        pData->m_cabinet_temp = data[8];
    }
    else
    {
        log_error("query bat data failer",ret);
        goto END;
    }

    ret = m_modbus_proxy.ReadInputRegisters(10057,15,data);
    if(ret > 0)
    {
        pData->m_n_dc_volt = data[0];
        pData->m_n_dc_curr = data[1];
        pData->m_n_pcs_stat = data[2];
        pData->m_n_ab_volt = data[6];
        pData->m_n_bc_volt = data[7];
        pData->m_n_ca_volt = data[8];
        pData->m_n_a_curr = data[9];
        pData->m_n_b_curr = data[10];
        pData->m_n_c_curr = data[11];
    }
    else
    {
        log_error("query volt data failer",ret);
    }
END:SendSyncAction(QUERY_IMPORTANT_DATA,pResp);
}

void MainDevice::onQueryImPortantData(INT32 ret, ASyncCallDataInst& pResp)
{
    ImportantData* pData = (ImportantData*)pResp.data();
    if(ret < 0)
        goto END;
    
    m_ess_data->m_ac_info.m_lineVolt.m_u_ab = pData->m_n_ab_volt;
    m_ess_data->m_ac_info.m_lineVolt.m_u_bc = pData->m_n_bc_volt;
    m_ess_data->m_ac_info.m_lineVolt.m_u_ac = pData->m_n_ca_volt;
    m_ess_data->m_ac_info.m_phaseCurr.m_i_a = pData->m_n_a_curr;
    m_ess_data->m_ac_info.m_phaseCurr.m_i_b = pData->m_n_b_curr;
    m_ess_data->m_ac_info.m_phaseCurr.m_i_c = pData->m_n_c_curr;
  
    m_ess_data->m_batt_curr = pData->m_bat_total_curr;
    m_ess_data->m_batt_volt = pData->m_bat_total_volt;
    m_ess_data->m_cell_max_volt = pData->m_cell_max_soc;
    m_ess_data->m_cell_min_volt = pData->m_cell_min_soc;
    m_ess_data->m_cell_max_temp = pData->m_cell_max_temp;
    m_ess_data->m_cell_min_temp = pData->m_cell_min_temp;

    //log_debug("m_important_interval = %d",m_important_interval);

END:m_timer_important_data.StartTimer(m_important_interval);
}

void MainDevice::QueryUnImPortantData()
{
    ASyncCallDataInst pResp;
    pResp.fill(0,sizeof(UnImportantData));
    UINT16 data[20] = {0};
    UnImportantData* pData = (UnImportantData*)pResp.data();
    int ret = m_modbus_proxy.ReadInputRegisters(10053, 4, data);
    if(ret > 0)
    {
        pData->m_charge_day = data[0];
        pData->m_discharge_day = data[1];
        pData->m_charge_time_day = data[2];
        pData->m_discharge_time_day = data[3];
    }
    else
    {
        log_error("QueryUnImPortantData 10053 failer");
        goto END;
    }

    ret = m_modbus_proxy.ReadInputRegisters(10301, 10, data);
    if(ret > 0)
    {
        pData->m_run_time_total = PublicTool::MakeUint32CDAB(data[0],data[1]);
        pData->m_charge_total = PublicTool::MakeUint32CDAB(data[2],data[3]);
        pData->m_discharge_total = PublicTool::MakeUint32CDAB(data[4],data[5]);
        pData->m_charge_time_total = PublicTool::MakeUint32CDAB(data[6],data[7]);
        pData->m_discharge_time_total = PublicTool::MakeUint32CDAB(data[8],data[9]);
    }
    else
    {
        log_error("QueryUnImPortantData 10301 failer");
        goto END;
    }

END:SendSyncAction(QUERY_UNIMPORTANT_DATA,ret, pResp);
}

void MainDevice::onQueryUnImPortantData(INT32 ret, ASyncCallDataInst& pResp)
{
    UnImportantData* pData = (UnImportantData*)pResp.data();
    if(ret < 0)
        goto END;

    m_ess_data->m_charge_p_day = pData->m_charge_day;			//当日充电量
	m_ess_data->m_discharge_p_day = pData->m_discharge_day;		//当日放电量
	m_ess_data->m_charge_p_total = pData->m_charge_total;		//累计充电量
	m_ess_data->m_discharge_p_total = pData->m_discharge_total;	//累计放电量

END:m_timer_unimportant_data.StartTimer(m_unimportant_interval);
}

int  MainDevice::TurnOnOrOff(RUNSTATE s)
{
    return 0;
}
int  MainDevice::SetWorkMode(WORKMODE m)
{
    return 0;
}

extern "C" Device* CreateDevice(const std :: string & param)
{
	return new MainDevice;
}