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

enum TIMEID
{
	ENERGY_SCHEDULE=1,
};

static struct tagSerialPortMap
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


EMSDevice::EMSDevice()
{
	m_vec_pv.clear();
	m_vec_ess.clear();
	m_vec_diesel.clear();
	m_vec_load.clear();
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
			return;
		case RS485:
		case MODBUS_RTU:
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
		case MODBUS_TCP_CLI:
		{
			pConnParam.fill(0,sizeof(ModbusTcpCliComm));
			ModbusTcpCliComm* pData = (ModbusTcpCliComm*)pConnParam.data();
			snprintf(pData->addr,sizeof(pData->addr),"%s",commparam["addr"].asString().c_str());
			pData->nPort = commparam["port"].asInt();
			pData->nSlaveId = commparam["slaveid"].asInt();
			return;
		}
		case MODBUS_TCP_SRV:
		{
			pConnParam.fill(0,sizeof(ModbusTcpSrvComm));
			ModbusTcpSrvComm* pData = (ModbusTcpSrvComm*)pConnParam.data();
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


float EMSDevice::GetPvTotalPower()				//获取当前光伏功率总和
{
	float totalP = 0.f;
	for(auto it = m_vec_pv.begin(); it != m_vec_pv.end(); ++it)
	{
		totalP += (*it)->GetData()->p;
	}
	return totalP;
}

float EMSDevice::GetEssTotalDischargePower()				//获取当前储能功率总和
{
	float totalP = 0.f;
	for(auto it = m_vec_ess.begin(); it != m_vec_ess.end(); ++it)
	{
		totalP += (*it)->GetData()->p;
	}
	return totalP;
}

float EMSDevice::GetDeTotalPower()				//获取柴发组功率总和
{
	float totalP = 0.f;
	for(auto it = m_vec_diesel.begin(); it != m_vec_diesel.end(); ++it)
	{
		totalP += (*it)->GetData()->p;
	}
	return totalP;
}

float EMSDevice::GetLoadNeedPower()				//获取负载需求功率和
{
	float totalP = 0.f;
	for(auto it = m_vec_load.begin(); it != m_vec_load.end(); ++it)
	{
		totalP += (*it)->GetData()->needp;
	}
	return totalP;
}

float EMSDevice::GetLoadTotalPower()				//获取负载当前功率和
{
	float totalP = 0.f;
	for(auto it = m_vec_load.begin(); it != m_vec_load.end(); ++it)
	{
		totalP += (*it)->GetData()->p;
	}
	return totalP;
}

void  EMSDevice::EnergySchedule()
{
	float loadTotalP = GetLoadNeedPower();
	float pvTotalP = GetPvTotalPower();

	if(loadTotalP <= pvTotalP)
	{
		//TODO 储能是否可以充电
		return;
	}

	float essMaxPower = GetEssTotalDischargePower();
	if(loadTotalP <= pvTotalP + essMaxPower)
	{
		//TODO 调整储能功率
		return;
	}

	float des = GetDeTotalPower();

	if(loadTotalP <= pvTotalP + essMaxPower + des)
	{
		//TODO 启动柴发并调整功率
		return;
	}

	// TODO 按优先级关闭负载
	// TurnOffLoad();
}

int EMSDevice::CreateDevices()
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
		const Json::Value& dev = v[i];
		const Json::Value& obj = dev["dev"];

		
		PVDevice *pPvDev = dynamic_cast<PVDevice *>(DeviceManager::GetInstance()->CreateDevice(obj["model"].asString()));
		if(!pPvDev)
			return -1;
		pPvDev->InitData();
		int connType = 0;
		ASyncCallDataInst data;
		GetDeviceConnParam(connType,data,obj);
		if(pPvDev->Init(value["uuid"].asString(), value["model"].asString(), value["uuid"].asString(), connType, data))
			return -1;
		m_vec_pv.append(pPvDev);
		
		if(!dev["Meter"].isObject())
			continue;

		MeterDevice* pMeter = CreateMeterDevices(dev["Meter"]);
		if(pMeter)
		{
			m_vec_meter.append(pMeter);
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
		delete pMeter
		return nullptr;
	}
	return pMeter;
}

