#include "SysCfgModule.h"
#include "TcpV4Server.h"
#include "SysCfgSockItem.h"
#include "PublicTool.h"
#include "Log.h"

enum CFGTYEPE
{
	SYS_CONF=1,
	DEV_CONF,
};

SysCfgModule* SysCfgModule::GetInstance()
{
	static SysCfgModule cfg;
	return &cfg;
}

SysCfgModule::SysCfgModule():m_pSrv(nullptr)
{
	
}

SysCfgModule::~SysCfgModule()
{
	
}

int SysCfgModule::Init()
{
	m_pSrv = new CTcpV4Server;
	if(!m_pSrv)
	{
		log_error("SysCfgModule::Init");
		return -1;
	}

	m_pSrv->setAddrInfo("0.0.0.0", 8899);
	m_pSrv->SetCreateFun(&CreateSocket<SysCfgSockItem>);
	return m_pSrv->Init();
}

int SysCfgModule::HandSysCfgMsg(SysCfgSockItem* const pSock, int type, std::string& obj)
{
	std::string cfgfile = PublicTool::GetProgramLocation()+"/conf/";
	switch(type)
	{
		case SYS_CONF:
		{
			cfgfile.append("syscfg.json");
			break;
		}
		case DEV_CONF:
		{
			cfgfile.append("Device.json");
			break;
		}
		default:
		{
			log_error("SysCfgModule::HandSysCfgMsg unknow type = %d",type);
			return -1;
		}
	}
	PublicTool::WriteFile(cfgfile, obj);
	//TODO:向客户端回数据
	return 0;
}

