#include "core/Reactor.h"
#include "IACESEMS.h"
#include "Log.h"
#include "DeviceManager.h"
#include "PublicTool.h"
#include "ESSDevice.h"
#include "PCSDevice.h"
#include "DeviceCommType.h"
#include "DIDevice.h"
#include "BCMUDevice.h"
#include "BMUDevice.h"
#include "MeterDevice.h"
#include <math.h>
#include "TimeOfUse.h"
#include "WaterColdEms.h"
#include "WindColdEms.h"



#define DATA_POOL_LEN 8192

enum EMSACTION
{
	AT_START_EMS=1,
};

struct tagSerialPortMap
{
	int addr;
	char devPath[32];
}SerialPortMap[] = {
	{0,"undefined"},
	{1,"/dev/ttySAC2"},
	{2,"/dev/ttySAC4"},
	{3,"/dev/ttySAC3"},
	{4,"/dev/ttySAC1"}
};


IACESEMS::IACESEMS():m_sz_data_pool(nullptr),m_n_malloc(0),m_p_ems_other_data(nullptr),
m_n_invt_meter_num(0),m_pInvtMeter(nullptr),m_n_bmu_num(0),m_pBmu(nullptr)
{
	m_timer_test.InitTimerNode(this, 1);
}

IACESEMS::~IACESEMS()
{
	if(m_sz_data_pool)
		delete[] m_sz_data_pool;
}

int IACESEMS::InitEMS()
{
	DeviceManager::GetInstance()->Init();
	std::string devCfg = PublicTool::ReadFile(PublicTool::GetProgramLocation()+"conf/Device.json");
	//log_debug("devCfg = %s", devCfg.c_str());
	Json::Value root;
	Json::Reader reader;
	
	log_info("reader.parse");
	if(!reader.parse(devCfg,root))
	{
		log_error("Device.json parse failer");
		return -1;
	}
	
	m_sz_data_pool = new char[DATA_POOL_LEN];
	if(!m_sz_data_pool)
		return -1;

	CreateDevice(root);
	
	if(InitEmergencyDi())
		;//return -1;
	
	log_info("m_n_malloc = %d", m_n_malloc);

	m_pTOU = new TOU(this);
	//m_pTOU->Start();
	//SendSyncAction(AT_START_EMS);
	return 0;
}

int IACESEMS::StartEMS()
{
	log_info("Start EMS...");
	m_pPcs->SetEnableState(ENABLE);
	m_pBcmu->SetEnableState(ENABLE);
	m_pInvtMeter->SetEnableState(ENABLE);
	m_pGridMeter->SetEnableState(ENABLE);
	this->SetEnableState(ENABLE);
	m_pTOU->Start();
	return 0;
}

int IACESEMS::CreateDevice(Json::Value& root)
{
	if(!root["ESS"].isObject() || InitESSDevice(root["ESS"]))
		return -1;
	
	log_info("Create ESS Devices");
	
	if(!root["EMS"].isObject() || InitEmsDevice(root["EMS"]))
		return -1;
	log_info("Create EMS Devices");

	if(!root["PCS"].isObject() || InitPcsDevice(root["PCS"]))
		return -1;
	log_info("Create PCS Devices");

	if(!root["BCMU"].isObject() || InitBmsDevice(root["BCMU"]))
		return -1;
	log_info("Create BCMU Devices");

	Json::Value& jsonBcmu = root["BCMU"];
	if(!jsonBcmu["BMU"].isArray() || InitBmuDevices(jsonBcmu["BMU"]))
		return -1;
	log_info("Create BMUS Devices");

	if(!root["GRIDMETER"].isObject() || InitGridMeter(root["GRIDMETER"]))
		return -1;
	log_info("Create GridMeter Devices");

	if(!root["INVTMETER"].isObject() || InitInvtMeter(root["INVTMETER"]))
		return -1;
	log_info("Create INVTMETER Devices");
	return 0;
}

void IACESEMS::HandOtherThings()
{
	return;
}

const PCSData* IACESEMS::GetPcsData()
{
	return m_pPcs->GetData();
}

const BCMUData* IACESEMS::GetBcmuData()
{
	return m_pBcmu->GetData();
}

const MeterData* IACESEMS::GetGridMeterData()
{
	return m_pGridMeter->GetData();
}

const MeterData* IACESEMS::GetInvtMeterData()
{
	return m_pInvtMeter->GetData();
}

Device* IACESEMS::GetTopDevice()
{
	return m_pEss;
}


int IACESEMS::ProcessTimeOutEvent(UINT32 nTimeId)
{
	SendAsyncAction(1);
	return 0;
}
	
int IACESEMS::Initialize(int cmd, ASyncCallDataInst& initdata)
{
	log_info("commtype = %d,m_str_uuid = %s", cmd,m_pDevBasicInfo->m_str_device_sn);
	//m_timer_test.StartTimer(100, -1);
	m_pEMSData->m_u16_power_demand = 100;
	m_pEMSData->m_sz_tou[0].m_u16_starthour = 0;
	m_pEMSData->m_sz_tou[0].m_u16_startmin = 0;
	m_pEMSData->m_sz_tou[0].m_u16_endhour = 0;
	m_pEMSData->m_sz_tou[0].m_u16_endmin = 0;
	m_pEMSData->m_sz_tou[0].m_i16_Power = 2;
	
	return 0;
}

int IACESEMS::SetDataPtr(char *ptr)
{
	int nBasicDataLen =EMSDevice::SetDataPtr(ptr);
	m_p_ems_other_data = (IACESEMSData *)(ptr + nBasicDataLen);
	return sizeof(IACESEMSData)+nBasicDataLen;
}

void IACESEMS::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst & pReq)
{
	log_debug("DoAsyncAction cmd = %d", cmd);
	SendSyncAction(1);
}

void IACESEMS::DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst & pResp)
{
	switch(cmd)
	{
		case AT_START_EMS:
		{
			log_info("Start EMS...");
			m_pPcs->SetEnableState(ENABLE);
			//m_pBcmu->SetEnableState(ENABLE);
			//m_pInvtMeter->SetEnableState(ENABLE);
			//m_pGridMeter->SetEnableState(ENABLE);
			//m_pLiquidCooler->SetEnableState(ENABLE);
			//m_pDehumidifier->SetEnableState(ENABLE);
			this->SetEnableState(ENABLE);
			m_pTOU->Start();
			break;
		}
	}
}

void IACESEMS::SetPower(float p)
{
	//todo:并机相关处理
	//再看是否有告警故障啥的
	log_warn("IACESEMS::SetPower = %.2f", p);

	if(fabs(p) < 0.1f || 0 != GetSysFault())
	{
		p = 0.f;
	}
	
	//m_p_ems_other_data->HandSetPower = p;
	if(0.f == p)
	{
		m_pPcs->SetP(0.f);
		m_pPcs->SetAppaP(0.f);
		m_pPcs->PowerOnOrOff(TURNOFF);
	}
	else
	{
		const BCMUData* pBcmuData = m_pBcmu->GetData();
		if(0 == pBcmuData->m_u16_state_maincontactor)			//主接触器没有闭合则下发主接触器闭合指令
		{
			log_warn("maincontactor opened");
			m_pBcmu->PowerOnOrOff(BMSPOWERON);
			m_pPcs->SetP(0.f);
			return;
		}

		if(1 == pBcmuData->m_u16_state_precontactor)			//预充接触器断开后才能下发功率指令
		{
			log_warn("premaincontactor opened");
			return;
		}

		const PCSData* pPcsData = m_pPcs->GetData();
		if(TURNON != pPcsData->m_u16_work_state)				//如果PCS待机状态则开启PCS
		{
			log_warn("pcs is not running %d",pPcsData->m_u16_work_state);
			m_pPcs->PowerOnOrOff(TURNON);
			return;
		}

		if(p < 0.f && (-p) > m_pBcmu->GetMaxChargePower())
		{
			p = -m_pBcmu->GetMaxChargePower();
		}
		else if(p > 0.f && p > m_pBcmu->GetMaxDisChargePower())
		{
			p = m_pBcmu->GetMaxDisChargePower();
		}
		m_pPcs->SetP(p);
	}
	HandOtherThings();
}

UINT16 IACESEMS::GetSoc()
{
	const BCMUData* pBcmuData = m_pBcmu->GetData();
	return pBcmuData->m_u16_soc;
}

float IACESEMS::GetMaxChargePower()
{
	const BCMUData* pBcmuData = m_pBcmu->GetData();
	return pBcmuData->m_u16_curr_max_charge * pBcmuData->m_u16_volt_total;
}

float IACESEMS::GetMaxDisChargePower()
{
	const BCMUData* pBcmuData = m_pBcmu->GetData();
	return pBcmuData->m_u16_curr_max_discharge * pBcmuData->m_u16_volt_total;
}

float IACESEMS::GetGridActPower() 			//获取市电电表有功功率
{
	return m_pGridMeter == nullptr ? 0.f : m_pGridMeter->GetData()->m_p_total;
}

float IACESEMS::GetLocalActPower()
{
	return m_pInvtMeter == nullptr ? 0.f : m_pInvtMeter->GetData()->m_p_total;
}

int IACESEMS::InitESSDevice(Json::Value& value)
{
	m_pEss = new ESSDevice;
	if(!m_pEss)
	{
		log_error("ESSDevice create failer");
		return -1;
	}
	m_n_malloc += m_pEss->InitData(m_sz_data_pool+m_n_malloc);

	if(m_pEss->Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), 0))
	{
		log_error("ESSDevice init failer");
		return -1;
	}
	DeviceManager::GetInstance()->RegisterDevice(m_pEss);
	return 0;
}

int IACESEMS::InitEmsDevice(Json::Value& value)
{
	m_n_malloc += InitData(m_sz_data_pool+m_n_malloc);
	if(Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), 0))
		return -1;
	SetParent(m_pEss);
	SetThreadIndex(EMS_THREAD);
	DeviceManager::GetInstance()->RegisterDevice(this);
	return 0;
}

int IACESEMS::InitPcsDevice(Json::Value& value)
{
	m_pPcs = dynamic_cast<PCSDevice *>(DeviceManager::GetInstance()->CreateDevice(value["model"].asString()));
	if(!m_pPcs)
	{
		log_error("create pcs failer");
		return -1;
	}
	CAsyncCallSupport* pAsync = dynamic_cast<CAsyncCallSupport*>(m_pPcs);
	if(pAsync)
	{
		pAsync->SetThreadIndex(PCS_THREAD);
	}
	m_n_malloc += m_pPcs->InitData(m_sz_data_pool+m_n_malloc);

	int connType = 0;
	ASyncCallDataInst data;
	GetDeviceConnParam(connType,data,value);
	
	if(m_pPcs->Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), connType, data))
	{
		log_error("PCS INIT Failer");
		return -1;
	}
	m_pPcs->SetParent(m_pEss);
	m_pPcs->SetQuicklyInterval(100);
	m_pPcs->SetStandardlyInterval(500);
	return 0;
}

int IACESEMS::InitBmsDevice(Json::Value& value)
{
	m_pBcmu = dynamic_cast<BCMUDevice *>(DeviceManager::GetInstance()->CreateDevice(value["model"].asString()));
	if(!m_pBcmu)
	{
		log_error("create bcmu failer");
		return -1;
	}
	CAsyncCallSupport* pAsync = dynamic_cast<CAsyncCallSupport*>(m_pBcmu);
	if(pAsync)
	{
		pAsync->SetThreadIndex(BCMU_THREAD);
	}
	m_n_malloc += m_pBcmu->InitData(m_sz_data_pool+m_n_malloc);

	int connType = 0;
	ASyncCallDataInst data;
	GetDeviceConnParam(connType,data,value);

	if(m_pBcmu->Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), connType, data))
	{
		log_error("Bcmu init failer");
		return -1;
	}
	m_pBcmu->SetParent(m_pEss);
	m_pBcmu->SetQuicklyInterval(100);
	m_pBcmu->SetStandardlyInterval(500);
	return 0;
}

int IACESEMS::InitBmuDevices(Json::Value& value)
{
	m_n_bmu_num = value.size();
	if(m_n_bmu_num <= 0)
		return 0;
	
	m_pBmu = new BMUDevice*[m_n_bmu_num];
	for(int i = 0; i < m_n_bmu_num; ++i)
	{
		const Json::Value& obj = value[i];
		m_pBmu[i] = dynamic_cast<BMUDevice *>(DeviceManager::GetInstance()->CreateDevice(obj["model"].asString()));
		m_n_malloc += m_pBmu[i]->InitData(m_sz_data_pool+m_n_malloc);
		m_pBmu[i]->Init(obj["uuid"].asString(), obj["model"].asString(), obj["uuid"].asString(),0);
		m_pBmu[i]->SetParent(m_pBcmu);
	}
	return 0;
}

int IACESEMS::InitGridMeter(Json::Value& value)
{
	m_pGridMeter = dynamic_cast<MeterDevice *>(DeviceManager::GetInstance()->CreateDevice(value["model"].asString()));
	if(!m_pGridMeter)
	{
		log_error("create GridMeter failer");
		return -1;
	}
	CAsyncCallSupport* pAsync = dynamic_cast<CAsyncCallSupport*>(m_pGridMeter);
	if(pAsync)
	{
		pAsync->SetThreadIndex(METER_THREAD);
	}
	m_n_malloc += m_pGridMeter->InitData(m_sz_data_pool+m_n_malloc);
	int connType = 0;
	ASyncCallDataInst data;
	GetDeviceConnParam(connType,data,value);
	if(m_pGridMeter->Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), connType, data))
	{
		log_error("gridmeter init failer");
		return -1;
	}
	m_pGridMeter->SetParent(m_pEss);
	m_pGridMeter->SetQuicklyInterval(100);
	m_pGridMeter->SetStandardlyInterval(500);
	return 0;
}

int IACESEMS::InitInvtMeter(Json::Value& value)
{
	m_pInvtMeter = dynamic_cast<MeterDevice *>(DeviceManager::GetInstance()->CreateDevice(value["model"].asString()));
	if(!m_pInvtMeter)
	{
		log_error("create InvtMeter failer");
		return -1;
	}
	CAsyncCallSupport* pAsync = dynamic_cast<CAsyncCallSupport*>(m_pInvtMeter);
	if(pAsync)
	{
		pAsync->SetThreadIndex(METER_THREAD);
	}
	m_n_malloc += m_pInvtMeter->InitData(m_sz_data_pool+m_n_malloc);
	int connType = 0;
	ASyncCallDataInst data;
	GetDeviceConnParam(connType,data,value);
	if(m_pInvtMeter->Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), connType, data))
	{
		log_error("InvtMeter init failer");
		return -1;
	}
	m_pInvtMeter->SetParent(m_pEss);
	m_pInvtMeter->SetQuicklyInterval(100);
	m_pInvtMeter->SetStandardlyInterval(500);
	return 0;
}

int IACESEMS::InitEmergencyDi()
{
	m_pEmergencyStop = new DiDevice;
	if(m_pEmergencyStop->InitDi(138))
		return -1;
	m_pEmergencyStop->SetThreadIndex(DI_THREAD);
	m_pEmergencyStop->Start();
	return 0;
}

//获取A，B，C三相负载
bool IACESEMS::GetThreePhasePower(float& pA, float& pB, float& pC)
{
	if(!m_pGridMeter)
		return false;
	if(!m_pInvtMeter)
		return false;

	const MeterData* pGridMeterData = m_pGridMeter->GetData();
	const MeterData* pInvtMeterData = m_pInvtMeter->GetData();

	pA = pGridMeterData->m_p_a - pGridMeterData->m_p_a;
	pB = pGridMeterData->m_p_b - pGridMeterData->m_p_b;
	pC = pGridMeterData->m_p_c - pGridMeterData->m_p_c;
	return true;
}

//通过配置获取设备的连接方式及连接参数
void IACESEMS::GetDeviceConnParam(int& connType,ASyncCallDataInst& pConnParam,const Json::Value& ConnParam)
{
	connType = ConnParam["commtype"].asInt();
	const Json::Value& commparam = ConnParam["commparam"];
	switch(connType)
	{
		case NONE:
			return;
		case RS485:
		{
			pConnParam.fill(0,sizeof(ModbusRtuComm));
			ModbusRtuComm* pData = (ModbusRtuComm*)pConnParam.data();
			snprintf(pData->PortDev,sizeof(pData->PortDev),"%s",SerialPortMap[commparam["addr"].asInt()].devPath);
			pData->nBaud = commparam["Baud"].asInt();
			pData->nDataBit = commparam["DataBit"].asInt();
			pData->nParity = commparam["Parity"].asInt();
			pData->nStopBit = commparam["StopBit"].asInt();
			pData->nSlaveId = commparam["slaveid"].asInt();
			return;
		}
		case NET:
		{
			pConnParam.fill(0,sizeof(ModbusTcpCliComm));
			ModbusTcpCliComm* pData = (ModbusTcpCliComm*)pConnParam.data();
			snprintf(pData->addr,sizeof(pData->addr),"%s",commparam["addr"].asString().c_str());
			pData->nPort = commparam["port"].asInt();
			pData->nSlaveId = commparam["slaveid"].asInt();
			return;
		}
		case CAN:
		{
			pConnParam.fill(0,sizeof(CanComm));
			CanComm* pData = (CanComm*)pConnParam.data();
			pData->nCanId = commparam["nCanId"].asInt();
			pData->nBand = commparam["nBand"].asInt();
			return;
		}
		default:
			return;
	}
	return;
}

extern "C" Device* CreateDevice(const std::string& param)
{
	if("WaterCold" == param)
		return new WaterColdEMS;
	else
		return new WindColdEMS;
}