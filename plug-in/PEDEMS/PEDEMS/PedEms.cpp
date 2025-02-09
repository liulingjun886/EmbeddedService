#include "PedEms.h"
#include "RunCfgSockItem.h"
#include "Log.h"
#include "TcpV4Server.h"
#include "json/json.h"
#include "DeviceManager.h"
#include "PublicTool.h"

PedEms* PedEms::GetInstance()
{
	static PedEms s_instance;
	return &s_instance;
}

PedEms::PedEms()
{
	
}

PedEms::~PedEms()
{
	
}

int PedEms::InitEMS()
{	
	Log::GetInstance()->SetLogLevel(LOG_DEBUG);
	InitEMSDevice();								//初始化EMS设备自身
	CreateDevices();								//创建子设备
	
	InitRunCfgSrv();								//初始化上位机接口
}

int PedEms::StartEMS()
{
	const std::map<std::string, Device*> devs = DeviceManager::GetInstance()->GetDevices();

	for(std::map<std::string, Device*>::const_iterator it = devs.begin(); it != devs.end(); ++it)
	{
		it->second->SetEnableState(ENABLE);
	}
	//m_time_energy_schedule.StartTimer(1000,-1);
	SetEnableState(ENABLE);
	return 0;
}

int PedEms::Initialize(int cmd, ASyncCallDataInst & initdata)
{
	return 0;
}

enum Local_Oper
{
	GET_ALL_DEV,
	GET_DEV_DATA,
	SET_DEV_DATA
};

int PedEms::HandRunCfgMsg(RunCfgSockItem* const pSock, int type, std::string& obj)
{
	//pSock->SendData(const char * pData, UINT32 nLen)
	if(GET_ALL_DEV == type)
	{
		ASyncCallDataInst data;
		const std::map<std::string, Device*>& vec_dev = DeviceManager::GetInstance()->GetDevices();
		data.fill(0,sizeof(DevBasicInfo) * vec_dev.size());
		DevBasicInfo* pInfo = (DevBasicInfo*)data.data();
		int i = 0;
		for(std::map<std::string, Device*>::const_iterator it = vec_dev.begin(); it != vec_dev.end(); ++it,++i)
		{
			memcpy(&pInfo[i],it->second->GetDevBasicInfo(),sizeof(DevBasicInfo));
		}
		log_debug("HandRunCfgMsg = %d,%d,%d",sizeof(DevBasicInfo),vec_dev.size(),data.length());
		pSock->SendData(type,data.data(),data.length());
	}
	if(GET_DEV_DATA == type)
	{
		Device* pDev = DeviceManager::GetInstance()->GetDeviceByUuid(obj);
		int nlen = 0;
		if(pDev)
		{
			char* pData = pDev->GetLocalData(nlen);
			pSock->SendData(type,pData,nlen);
		}
	}
	else if(SET_DEV_DATA == type)
	{
		//int ret = pDev->SetLocalData(obj);
		//pSock->SendData(type,&ret,sizeof(int));
	}
	return 0;
}

int PedEms::InitEMSDevice()
{
	InitData();
	Json::Value root;
	if(PublicTool::LoadJsonCfg(root, "syscfg.json"))
	{
		log_error("syscfg.json parse failer");
		return -1;
	}
	
	if(0 == Init(root["sn"].asString(), root["model"].asString(), root["name"].asString()))
	{
		DeviceManager::GetInstance()->RegisterDevice(this);
		return 0;
	}
	
	log_error("EMS Init failer");
	return -1;
}

//创建上位机服务接口
int	PedEms::InitRunCfgSrv()
{
	m_pSrv = new CTcpV4Server;
	if(!m_pSrv)
	{
		log_error("SysCfgModule::Init");
		return -1;
	}
	m_pSrv->setAddrInfo("0.0.0.0", 12000);
	m_pSrv->SetCreateFun(&CreateSocket<RunCfgSockItem>);
	if(m_pSrv->Init())
		return -1;
	return 0;
}

int PedEms::BindMeterDevice()
{
	
}

extern "C" Device* CreateDevice(const std :: string & param)
{
	return PedEms::GetInstance();
}