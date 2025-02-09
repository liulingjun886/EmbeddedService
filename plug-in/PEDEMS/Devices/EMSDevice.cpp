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
#include <unistd.h>


#define CRITICALINTERVAL 1000
#define IMPORTANTINTERVAL 10000
#define UNIMPORTANTINTERVAL 100000

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

void EMSDevice::StartDevice()
{
	m_time_energy_schedule.StartTimer(CRITICALINTERVAL*2);
}

const EMSData* EMSDevice::GetData()	
{
	return m_ems_data;
}

int EMSDevice::GetDeviceConnParam(int& connType,ASyncCallDataInst& pConnParam,const Json::Value& ConnParam)
{
	connType = ConnParam["commtype"].asInt();
	const Json::Value& commparam = ConnParam["commparam"];
	switch(connType)
	{
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
			return commparam["addr"].asInt();
		}
		case NET:
		{
			pConnParam.fill(0,sizeof(ModbusTcpCliComm));
			ModbusTcpCliComm* pData = (ModbusTcpCliComm*)pConnParam.data();
			snprintf(pData->addr,sizeof(pData->addr),"%s",commparam["addr"].asString().c_str());
			pData->nPort = commparam["port"].asInt();
			pData->nSlaveId = commparam["slaveid"].asInt();
			return 0;
		}
		case CAN:
		{
			pConnParam.fill(0,sizeof(CanComm));
			CanComm* pData = (CanComm*)pConnParam.data();
			pData->nCanId = commparam["nCanId"].asInt();
			pData->nBand = commparam["nBand"].asInt();
			return 0;
		}
		default:
			return 0;
	}
	return 0;
}

void  EMSDevice::EnergySchedule()
{	
	UINT32 load = m_pvManager.GetPower() + m_essManager.GetPower() + m_dieselManager.GetPower();

	//柴发启动停止阈值
	if(m_essManager.GetSoc() < m_ems_data->esc.threshold_dg_turnon)
		m_dieselManager.Start();
	else if(m_essManager.GetSoc() > m_ems_data->esc.threshold_dg_turnoff)
		m_dieselManager.Stop();

	log_debug("EMSDevice::EnergySchedule = %d, soc = %d", load,m_essManager.GetSoc());

	//重要负荷切合阈值
	if(m_essManager.GetSoc() < m_ems_data->esc.threshold_ld_imp_turnoff)
		m_loadManager.StopLoad(IMPORTANT);
	else if(m_essManager.GetSoc() > m_ems_data->esc.threshold_ld_imp_turnon)
		m_loadManager.StartLoad(IMPORTANT);

	//一般负荷切合阈值
	if(m_essManager.GetSoc() < m_ems_data->esc.threshold_ld_unimp_turnoff)
		m_loadManager.StopLoad(UNIMPORTANT);
	else if(m_essManager.GetSoc() > m_ems_data->esc.threshold_ld_unimp_turnon)
		m_loadManager.StartLoad(UNIMPORTANT);

	//光伏限功率阈值
	if(m_essManager.GetSoc() > m_ems_data->esc.threshold_pv_turn_off)
		m_pvManager.SetPower(0);
	else if(m_essManager.GetSoc() < m_ems_data->esc.threadhold_pv_unlimit)
		m_pvManager.SetPower(-1);
	else if(m_essManager.GetSoc() < m_ems_data->esc.threadhold_pv_limit)
		m_pvManager.SetPower(load);

	m_time_energy_schedule.StartTimer(CRITICALINTERVAL);
}

int EMSDevice::CreateDevices()
{
	HardWareCfg::GetInstance()->Init();			//初始化硬件接口
	DeviceManager::GetInstance()->Init();		//初始化设备管理

	Json::Value root;
	if(PublicTool::LoadJsonCfg(root, "Device.json"))
	{
		return -1;
	}

	if(root["Pv"].isArray() && CreatePvDevices(root["Pv"]))
		return -1;
	
	if(root["Ess"].isArray() && CreateEssDevices(root["Ess"]))
		return -1;

	if(root["Diesel"].isArray() && CreateDieselDevices(root["Diesel"]))
		return -1;

	if(root["Load"].isArray() && CreateLoadDevices(root["Load"]))
		return -1;

	if(root["Meter"].isArray() && CreateMeterDevices(root["Meter"]))
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
		PVDevice *pPvDev = dynamic_cast<PVDevice *>(DeviceManager::GetInstance()->CreateDevice(dev["model"].asString()));
		if(!pPvDev)
			return -1;
		
		pPvDev->InitData();
		int connType = 0;
		ASyncCallDataInst data;
		int nThreadIndex = GetDeviceConnParam(connType,data,dev);
		if(pPvDev->Init(dev["uuid"].asString(), dev["model"].asString(), dev["uuid"].asString(), connType, data))
			return -1;
		CAsyncCallSupport* pSync = dynamic_cast<CAsyncCallSupport*>(pPvDev);
		if(pSync)
		{
			SetDevAsyncCallThread(pSync,nThreadIndex);
		}
		pPvDev->SetCriticalInterval(CRITICALINTERVAL);
		pPvDev->SetImportantInterval(IMPORTANTINTERVAL);
		pPvDev->SetUnimportantInterval(UNIMPORTANTINTERVAL);
		DeviceManager::GetInstance()->RegisterDevice(pPvDev);
		m_pvManager.AddPvDev(pPvDev);
	}
}

int EMSDevice::CreateEssDevices(Json::Value& v)
{
	if(0 >= v.size())
		return 0;

	for(int i = 0; i < v.size(); ++i)
	{
		Json::Value& dev = v[i];
		ESSDevice *pEssDev = dynamic_cast<ESSDevice *>(DeviceManager::GetInstance()->CreateDevice(dev["model"].asString()));
		if(!pEssDev)
			return -1;
		
		pEssDev->InitData();
		int connType = 0;
		ASyncCallDataInst data;
		int nThreadIndex = GetDeviceConnParam(connType,data,dev);
		if(pEssDev->Init(dev["uuid"].asString(), dev["model"].asString(), dev["uuid"].asString(), connType, data))
			return -1;
		
		pEssDev->SetCriticalInterval(CRITICALINTERVAL);
		pEssDev->SetImportantInterval(IMPORTANTINTERVAL);
		pEssDev->SetUnimportantInterval(UNIMPORTANTINTERVAL);
		CAsyncCallSupport* pSync = dynamic_cast<CAsyncCallSupport*>(pEssDev);
		if(pSync)
		{
			SetDevAsyncCallThread(pSync,nThreadIndex);
		}
		DeviceManager::GetInstance()->RegisterDevice(pEssDev);
		m_essManager.AddEssDev(pEssDev);
	}
}

int EMSDevice::CreateDieselDevices(Json::Value& v)
{
	if(0 >= v.size())
		return 0;

	for(int i = 0; i < v.size(); ++i)
	{
		Json::Value& dev = v[i];
		DieselDevice *pPG = dynamic_cast<DieselDevice *>(DeviceManager::GetInstance()->CreateDevice(dev["model"].asString()));
		if(!pPG)
			return -1;
		
		pPG->InitData();
		int connType = 0;
		ASyncCallDataInst data;
		int nThreadIndex = GetDeviceConnParam(connType,data,dev);
		if(pPG->Init(dev["uuid"].asString(), dev["model"].asString(), dev["uuid"].asString(), connType, data))
			return -1;
		pPG->SetCriticalInterval(CRITICALINTERVAL);
		pPG->SetImportantInterval(IMPORTANTINTERVAL);
		pPG->SetUnimportantInterval(UNIMPORTANTINTERVAL);
		CAsyncCallSupport* pSync = dynamic_cast<CAsyncCallSupport*>(pPG);
		if(pSync)
		{
			SetDevAsyncCallThread(pSync,nThreadIndex);
		}
		DeviceManager::GetInstance()->RegisterDevice(pPG);
		m_dieselManager.AddDieselDev(pPG);
	}
}

int EMSDevice::CreateLoadDevices(Json::Value& v)
{
	if(0 >= v.size())
		return 0;

	for(int i = 0; i < v.size(); ++i)
	{
		Json::Value& dev = v[i];
		LoadDevice *pLd = dynamic_cast<LoadDevice *>(DeviceManager::GetInstance()->CreateDevice(dev["model"].asString()));
		if(!pLd)
			return -1;
		
		pLd->InitData();
		int connType = 0;
		ASyncCallDataInst data;
		int nThreadIndex = GetDeviceConnParam(connType,data,dev);
		if(pLd->Init(dev["uuid"].asString(), dev["model"].asString(), dev["uuid"].asString(), connType, data))
			return -1;

		CAsyncCallSupport* pSync = dynamic_cast<CAsyncCallSupport*>(pLd);
		if(pSync)
		{
			SetDevAsyncCallThread(pSync,nThreadIndex);
		}
		pLd->SetCriticalInterval(CRITICALINTERVAL);
		pLd->SetImportantInterval(IMPORTANTINTERVAL);
		pLd->SetUnimportantInterval(UNIMPORTANTINTERVAL);
		
		DeviceManager::GetInstance()->RegisterDevice(pLd);
		m_loadManager.AddLoadDev(pLd);
	}
	return 0;
}

int EMSDevice::CreateMeterDevices(Json::Value& v)
{
	if(0 >= v.size())
		return 0;
	for(int i = 0; i < v.size(); ++i)
	{
		Json::Value& dev = v[i];
		MeterDevice* pMeter = dynamic_cast<MeterDevice *>(DeviceManager::GetInstance()->CreateDevice(dev["model"].asString()));
		if(!pMeter)
			return -1;
		
		pMeter->InitData();

		int connType = 0;
		ASyncCallDataInst data;
		int nThreadIndex = GetDeviceConnParam(connType,data,dev);
		if(pMeter->Init(dev["uuid"].asString(), dev["model"].asString(), dev["uuid"].asString(),connType,data))
			return -1;

		CAsyncCallSupport* pSync = dynamic_cast<CAsyncCallSupport*>(pMeter);
		if(pSync)
		{
			SetDevAsyncCallThread(pSync,nThreadIndex);
		}
		DeviceManager::GetInstance()->RegisterDevice(pMeter);
	}
	return 0;
}

void EMSDevice::ExitPrograme()
{
	sleep(5);
	_exit(0);
}

void EMSDevice::SetDevAsyncCallThread(CAsyncCallSupport* p, int nIndex)
{
	if(0 == nIndex)
	{
		p->SetThreadIndex(rand() % GetThreadNum() + 1);
	}
	else
	{
		p->SetThreadIndex(nIndex % GetThreadNum() + 1);
	}
}
