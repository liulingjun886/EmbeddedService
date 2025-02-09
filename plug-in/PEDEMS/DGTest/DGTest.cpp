#include "DGTest.h"
#include "Log.h"
#include "PublicTool.h"

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

    TURN_ON_OFF,
    POWER_SET
};

#pragma pack(1)

typedef struct stCriticalData
{
    UINT32 m_p;
    UINT32 m_pq;
    UINT32 m_pa;
    UINT16 m_pf;
    UINT16 m_grid_freq;
    UINT16 m_alarm[4];
}CriticalData;

typedef struct stImportantData
{
    UINT16 m_ab_volt;
    UINT16 m_bc_volt;
    UINT16 m_ac_volt;
    UINT16 m_a_curr;
    UINT16 m_b_curr;
    UINT16 m_c_curr;
    UINT32 m_a_p;
    UINT32 m_b_p;
    UINT32 m_c_p;

    UINT32 m_a_pq;
    UINT32 m_b_pq;
    UINT32 m_c_pq;
    UINT32 m_a_pa;
    UINT32 m_b_pa;
    UINT32 m_c_pa;
}ImportantData;

typedef struct stUnImportantData
{
    UINT16          m_generator_rot_speed;  // 发动机转速
    INT16 			m_oil_level;	        // 油位
	INT16 			m_oil_temp;		        // 油温
	UINT16			m_mpg;			        // 油耗
    UINT16          m_generator_temp;       // 发电机温度
	UINT16 			m_gen_day;		        // 当日发电量
	UINT32   		m_gen_total;	        // 累计发电量
	UINT32	  		m_run_time;		        // 累计运行时间
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
int  MainDevice::Initialize(int cmd, ASyncCallDataInst& initdata)
{
    if(ModbusOperator::InitModbusOperaterHander(&m_modbus_proxy,cmd,initdata.data(),500) < 0)
        return -1;
    //m_modbus_proxy.EnableDebug();
    return 0;
}

int  MainDevice::GetDataLen()
{
    return DieselDevice::GetDataLen() + sizeof(OtherData);
}

int MainDevice::SetDataPtr(char * ptr)
{
    int nBasicDataLen = DieselDevice::SetDataPtr(ptr);
	m_pOtherData = new (ptr + nBasicDataLen) OtherData;
	return sizeof(OtherData)+nBasicDataLen;
}

void MainDevice::StartDevice()
{
    m_timer_critical_data.StartTimer(m_critical_interval);
    m_timer_important_data.StartTimer(m_important_interval);
    m_timer_unimportant_data.StartTimer(m_unimportant_interval);
    log_debug("DGTest StartDevice");
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
            SendAsyncAction(TIMER_IMPORTANT);
            break;
        }
        case TIMER_UNIMPORTANT:
        {
            SendAsyncAction(TIMER_UNIMPORTANT);
            break;
        }
        default:
            break;
    }
}

void MainDevice::DoAsyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pReq)			   //处理异步请求(工作线程执行)
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
        case TURN_ON_OFF:
        {
            UINT16 data = (UINT16)(ret);
            m_modbus_proxy.WriteRegister(1001,data);
            break;
        }
        case POWER_SET:
        {
            m_modbus_proxy.WriteRegisters(1002,2,(UINT16*)&ret);
            break;
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
        default:
            break;
    }
}

void MainDevice::QueryCriticalData()
{
    ASyncCallDataInst data;
    data.fill(0,sizeof(CriticalData));
    CriticalData* pData = (CriticalData*)data.data();
    int ret = m_modbus_proxy.ReadInputRegisters(1028,8,(UINT16*)pData);
    if(ret < 0)
        goto END;
    
    pData->m_p = PublicTool::MakeUint32CDAB(pData->m_p);
    pData->m_pq = PublicTool::MakeUint32CDAB(pData->m_pq);
    pData->m_pa = PublicTool::MakeUint32CDAB(pData->m_pa);

    ret = m_modbus_proxy.ReadInputRegisters(1046,4,(UINT16*)&pData->m_alarm[0]);

END:SendSyncAction(QUERY_CRItiCAL_DATA,ret,data);
}

void MainDevice::onQueryCriticalData(INT32 ret, ASyncCallDataInst& pResp)
{
    CriticalData* pData = (CriticalData*)pResp.data();
    //log_error("onQueryCriticalData ret = %d",ret);
    if(ret < 0)
        goto END;
    
    m_dg_data->m_ac_info.m_power_info.m_p = pData->m_p;
    m_dg_data->m_ac_info.m_power_info.m_pq = pData->m_pq;
    m_dg_data->m_ac_info.m_power_info.m_pa = pData->m_pa;
    m_dg_data->m_ac_info.m_power_info.m_pf = pData->m_pf;
    m_dg_data->m_ac_info.m_grid_frequency = pData->m_grid_freq;

END:m_timer_critical_data.StartTimer(m_critical_interval);
}

void MainDevice::QueryImPortantData()
{
    ASyncCallDataInst data;
    data.fill(0,sizeof(ImportantData));
    ImportantData* pData = (ImportantData*)data.data();
    int ret = m_modbus_proxy.ReadInputRegisters(1002,12,(UINT16*)pData);
    if(ret < 0)
        goto END;

    pData->m_a_p = PublicTool::MakeUint32CDAB(pData->m_a_p);
    pData->m_b_p = PublicTool::MakeUint32CDAB(pData->m_b_p);
    pData->m_c_p = PublicTool::MakeUint32CDAB(pData->m_c_p);

    ret = m_modbus_proxy.ReadInputRegisters(1014,12,(UINT16*)&pData->m_a_pq);
    pData->m_a_pq = PublicTool::MakeUint32CDAB(pData->m_a_pq);
    pData->m_b_pq = PublicTool::MakeUint32CDAB(pData->m_b_pq);
    pData->m_c_pq = PublicTool::MakeUint32CDAB(pData->m_c_pq);

    pData->m_a_pa = PublicTool::MakeUint32CDAB(pData->m_a_pa);
    pData->m_b_pa = PublicTool::MakeUint32CDAB(pData->m_b_pa);
    pData->m_c_pa = PublicTool::MakeUint32CDAB(pData->m_c_pa);
    
END:SendSyncAction(QUERY_IMPORTANT_DATA,ret,data);
}

void MainDevice::onQueryImPortantData(INT32 ret, ASyncCallDataInst& pResp)
{
    ImportantData* pData = (ImportantData*)pResp.data();
    if(ret < 0)
        goto END;
    
    m_dg_data->m_ac_info.m_active_power.m_p_a = pData->m_a_p;
    m_dg_data->m_ac_info.m_active_power.m_p_b = pData->m_b_p;
    m_dg_data->m_ac_info.m_active_power.m_p_c = pData->m_c_p;

    m_dg_data->m_ac_info.m_reactive_power.m_p_a = pData->m_a_pq;
    m_dg_data->m_ac_info.m_reactive_power.m_p_b = pData->m_b_pq;
    m_dg_data->m_ac_info.m_reactive_power.m_p_c = pData->m_c_pq;

    m_dg_data->m_ac_info.m_apparent_power.m_p_a = pData->m_a_pa;
    m_dg_data->m_ac_info.m_apparent_power.m_p_b = pData->m_b_pa;
    m_dg_data->m_ac_info.m_apparent_power.m_p_c = pData->m_c_pa;

    m_dg_data->m_ac_info.m_lineVolt.m_u_ab = pData->m_ab_volt;
    m_dg_data->m_ac_info.m_lineVolt.m_u_bc = pData->m_bc_volt;
    m_dg_data->m_ac_info.m_lineVolt.m_u_ac = pData->m_ac_volt;

    m_dg_data->m_ac_info.m_phaseCurr.m_i_a = pData->m_a_curr;
    m_dg_data->m_ac_info.m_phaseCurr.m_i_b = pData->m_b_curr;
    m_dg_data->m_ac_info.m_phaseCurr.m_i_c = pData->m_c_curr;

END:m_timer_important_data.StartTimer(m_important_interval);
}

void MainDevice::QueryUnImPortantData()
{
    ASyncCallDataInst data;
    data.fill(0,sizeof(UnImportantData));
    UnImportantData* pData = (UnImportantData*)data.data();
    int ret = m_modbus_proxy.ReadInputRegisters(1036,10,(UINT16*)pData);
    if(ret < 0)
        goto END;

    pData->m_gen_total = PublicTool::MakeUint32CDAB(pData->m_gen_total);
    pData->m_run_time = PublicTool::MakeUint32CDAB(pData->m_run_time);

END:SendSyncAction(QUERY_UNIMPORTANT_DATA,ret,data);
}

void MainDevice::onQueryUnImPortantData(INT32 ret, ASyncCallDataInst& pResp)
{
    UnImportantData* pData = (UnImportantData*)pResp.data();
    if(ret < 0)
        goto END;
    
    m_dg_data->m_generator_rot_speed = pData->m_generator_rot_speed;
    m_dg_data->m_oil_level = pData->m_oil_level;
    m_dg_data->m_oil_temp = pData->m_oil_temp;
    m_dg_data->m_generator_temp = pData->m_generator_temp;
    m_dg_data->m_mpg = pData->m_mpg;
    m_dg_data->m_gen_day = pData->m_gen_day;
    m_dg_data->m_gen_total = pData->m_gen_total;
    m_dg_data->m_run_time = pData->m_run_time;
    
END:m_timer_unimportant_data.StartTimer(m_unimportant_interval);
}

int MainDevice::TurnOnOrOff(RUNSTATE s)
{
    UINT16 data = 0;
    if(TURNON == s)
    {
        data = 1;
    }
    SendAsyncAction(TURN_ON_OFF,data);
    return 0;
}

int MainDevice::SetPower(UINT32 p)
{
    SendAsyncAction(POWER_SET,(INT32)p);
    return 0;
}

int  MainDevice::SetWorkMode(WORKMODE wm)
{
    UINT16 data = wm;
    m_modbus_proxy.WriteRegister(1000,data);
    return 0;
}

extern "C" Device* CreateDevice(const std :: string & param)
{
	return new MainDevice;
}