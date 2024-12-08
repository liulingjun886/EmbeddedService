#include "EMSDevice.h"
#include "PVDevice.h"
#include "MeterDevice.h"
#include "ESSDevice.h"
#include "Diesel.h"
#include "LoadDevice.h"
#include "DeviceCommType.h"
#include "PublicTool.h"
#include "DeviceManager.h"
#include "Log.h"
#include "HardWareCfg.h"

enum TIMEID
{
	ENERGY_SCHEDULE=1,
};

EMSDevice::EMSDevice()
{
	m_vec_meter.clear();
	m_time_energy_schedule.InitTimerNode(this, ENERGY_SCHEDULE);
}

EMSDevice::~EMSDevice()
{
}

int EMSDevice::GetThreadNum()
{
	return 8;
}

int EMSDevice::GetDataLen()
{
	return sizeof(EMSData) + Device::GetDataLen();
}

int EMSDevice::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case ENERGY_SCHEDULE:
		{
			EnergySchedule();
			break;
		}
		default:
			break;
	}
	return 0;
}

int EMSDevice::SetDataPtr(char *ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_ems_data = new (ptr + nBasicDataLen) EMSData;
	return sizeof(EMSData)+nBasicDataLen ;
}

const EMSData* EMSDevice::GetData()	
{
	return m_ems_data;
}

void EMSDevice::GetDeviceConnParam(int& connType,ASyncCallDataInst& pConnParam,const Json::Value& ConnParam)
{
	connType = ConnParam["commtype"].asInt();
	const Json::Value& commparam = ConnParam["commparam"];
	switch(connType)
	{
		case NONE:
		case RS485:
		{
			pConnParam.fill(0,sizeof(ModbusRtuComm));
			ModbusRtuComm* pData = (ModbusRtuComm*)pConnParam.data();
			snprintf(pData->PortDev,sizeof(pData->PortDev),"%s",HardWareCfg::GetInstance()->GetRs485Name(commparam["addr"].asInt()).c_str());
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

void  EMSDevice::EnergySchedule()
{
	float loadTotalP = m_loadManager.GetNeedPower();
	float needPower = 0.f;
	needPower = m_pvManager.DisPatchPower(loadTotalP);

	if(0 == needPower)
		return;
	
	needPower = m_essManager.DisPatchPower(needPower);

	if(needPower < 0.f)
	{
		m_pvManager.SetPower(loadTotalP + needPower);
		return;
	}

	needPower = m_dieselManager.DisPatchPower(needPower);

	if(needPower > 0.f)
		m_loadManager.SetPower(loadTotalP - needPower);
}

int EMSDevice::CreateDevices()
{
	HardWareCfg::GetInstance()->Init();
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

	if(!root["Pv"].isArray() || CreatePvDevices(root["Pv"]))
		return -1;
	
	if(!root["Ess"].isArray() || CreatePvDevices(root["Ess"]))
		return -1;

	if(!root["Diesel"].isArray() || CreatePvDevices(root["Diesel"]))
		return -1;

	if(!root["Load"].isArray() || CreatePvDevices(root["Load"]))
		return -1;

	return 0;
}

int EMSDevice::CreatePvDevices(Json::Value& v)
{
	if(0 >= v.size())
		return 0;

	for(int i = 0; i < v.size(); ++i)
	{
		Json::Value& dev = v[i];
		Json::Value& obj = dev["dev"];

		PVDevice *pPvDev = dynamic_cast<PVDevice *>(DeviceManager::GetInstance()->CreateDevice(obj["model"].asString()));
		if(!pPvDev)
			return -1;
		
		pPvDev->InitData();
		int connType = 0;
		ASyncCallDataInst data;
		GetDeviceConnParam(connType,data,obj);
		if(pPvDev->Init(dev["uuid"].asString(), dev["model"].asString(), dev["uuid"].asString(), connType, data))
			return -1;
		m_pvManager.AddPvDev(pPvDev);
		
		if(!dev["Meter"].isObject())
			continue;

		MeterDevice* pMeter = CreateMeterDevices(dev["Meter"]);
		if(pMeter)
		{
			m_vec_meter.push_back(pMeter);
		}
	}
}

int EMSDevice::CreateEssDevices(Json::Value& v)
{
	
}

int EMSDevice::CreateDieselDevices(Json::Value& v)
{
	
}

int EMSDevice::CreateLoadDevices(Json::Value& v)
{
	
}

MeterDevice* EMSDevice::CreateMeterDevices(Json::Value& obj)
{
	MeterDevice* pMeter = dynamic_cast<MeterDevice *>(DeviceManager::GetInstance()->CreateDevice(obj["model"].asString()));
	if(!pMeter)
		return nullptr;
	
	pMeter->InitData();

	int connType = 0;
	ASyncCallDataInst data;
	GetDeviceConnParam(connType,data,obj);
	if(pMeter->Init(obj["uuid"].asString(), obj["model"].asString(), obj["uuid"].asString(),0))
	{
		delete pMeter;
		return nullptr;
	}
	return pMeter;
}

