#include "YT-320.h"
#include "Log.h"
#include "PublicTool.h"

enum TIMER_ID
{
	TIMER_CRITICAL=0
};

enum COMMAND
{
	QUERY_CRItiCAL_DATA=0
};

MainDevice::MainDevice()
{
    m_timer_critical_data.InitTimerNode(this,TIMER_CRITICAL);
}
MainDevice::~MainDevice()
{

}
int  MainDevice::Initialize(int cmd, ASyncCallDataInst& initdata)
{
    if(ModbusOperator::InitModbusOperaterHander(&m_modbus_proxy,cmd,initdata.data(),500) < 0)
        return -1;
    //m_modbus_proxy.EnableDebug();
    m_vec_critical_data.fill(0,40);
    return 0;
}
void MainDevice::DoAsyncAction(INT32 cmd, INT32 ret, ASyncCallDataInst& pReq)			   //处理异步请求(工作线程执行)
{
    switch(cmd)
    {
        case QUERY_CRItiCAL_DATA:
        {
            UINT16* pData = (UINT16*)m_vec_critical_data.data();
            int ret = m_modbus_proxy.ReadRegisters(1000,10,&pData[0]);
            ret = m_modbus_proxy.ReadRegisters(3000,8,&pData[10]);
            SendSyncAction(QUERY_CRItiCAL_DATA, ret, m_vec_critical_data);
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
            log_debug("DoSyncAction ret = %d",ret);
            UINT16* pData = (UINT16*)pResp.data();
            m_ld_data->m_stPowerInfo.m_p = PublicTool::MakeUint32CDAB(pData[8],pData[9]);
            m_ld_data->m_stPowerInfo.m_pe = PublicTool::MakeUint32CDAB(pData[2],pData[3]);
            m_pOtherData->m_n_ctrl_type = pData[0];
            m_pOtherData->m_n_gun_num = pData[1];
            m_pOtherData->m_n_request_power = PublicTool::MakeUint32CDAB(pData[6],pData[7]);
            m_pOtherData->m_n_power_crtl_switch = pData[10];
            m_pOtherData->m_n_power_limit = pData[13];
            m_pOtherData->m_n_power_limit_offline = pData[17];
            m_timer_critical_data.StartTimer(m_critical_interval);
            break;
        }
        default:
            break;
    }
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
        default:
            break;
    }
}

int MainDevice::TurnOnOrOff(RUNSTATE s)
{
    return 0;
}

int MainDevice::SetPower(float p)
{
    return 0;
}

void MainDevice::StartDevice()
{
    m_timer_critical_data.StartTimer(m_critical_interval);
}

int  MainDevice::GetDataLen()
{
    return LoadDevice::GetDataLen() + sizeof(OtherData);
}

int MainDevice::SetDataPtr(char * ptr)
{
    int nBasicDataLen = LoadDevice::SetDataPtr(ptr);
	m_pOtherData = new (ptr + nBasicDataLen) OtherData;
	return sizeof(OtherData)+nBasicDataLen;
}

extern "C" Device* CreateDevice(const std :: string & param)
{
	return new MainDevice;
}