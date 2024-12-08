#include "SysCfgModule.h"
#include "TcpV4Server.h"
#include "SysCfgSockItem.h"
#include "PublicTool.h"
#include "Log.h"
#include <json/json.h>
#include <string.h>


enum CFGTYEPE
{
	FILE_TRANSMIT_START = 1,		//文件开始上传
	FILE_TRANSMIT_COMMENT,			//文件类容传输
	FILE_TRANSMIT_FAILER,			//取消文件上传
	SYS_CONF_GET,					//系统配置获取
    SYS_CONF_SET,                   //系统配置设置
    DEV_CONF_GET,					//设备配置获取
    DEV_CONF_SET,					//设备配置设置
	NET_CFG,						//网络设置
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
		case FILE_TRANSMIT_START:
		{
			Json::Value root;
			Json::Reader reader;
			
			if(!reader.parse(obj, root))
			{
				log_error("DataFormat ERROR");
				return -1;
			}
			int ret = m_file_transmit.TransmitStart(root["fileSize"].asInt(),root["MD5"].asString());
			char buff[32] = {0};
			sprintf(buff,"{\"ret\":%d}",ret);
			printf("buf = %s\n",buff);
			pSock->SendData(type, buff, strlen(buff));
			break;
		}
		case FILE_TRANSMIT_COMMENT:
		{
			int ret = m_file_transmit.TransmitComment((char*)obj.c_str(),obj.length());
			char buff[32] = {0};
			sprintf(buff,"{\"ret\":%d}",ret);
			pSock->SendData(type, buff, strlen(buff));
			break;
		}
		case FILE_TRANSMIT_FAILER:
		{
			m_file_transmit.TransmitFailer();
			break;
		}
		case DEV_CONF_GET:
		{
			cfgfile.append("Device.json");
			std::string str = PublicTool::ReadFile(cfgfile);
			pSock->SendData(type,(char*)str.c_str(),str.length());
			break;
		}
		case DEV_CONF_SET:
		{
			cfgfile.append("Device.json");
			int ret = PublicTool::WriteFile(cfgfile,obj);
			char buff[32] = {0};
			sprintf(buff,"{\"ret\":%d}",ret);
			pSock->SendData(type, buff, strlen(buff));
			break;
		}
		case SYS_CONF_GET:
		{
			cfgfile.append("syscfg.json");
			std::string str = PublicTool::ReadFile(cfgfile);
			pSock->SendData(type,(char*)str.c_str(),str.length());
			break;
		}
		case SYS_CONF_SET:
		{
			cfgfile.append("syscfg.json");
			int ret = PublicTool::WriteFile(cfgfile, obj);
			char buff[32] = {0};
			sprintf(buff,"{\"ret\":%d}",ret);
			pSock->SendData(type, buff, strlen(buff));
			break;
		}
		case NET_CFG:
		{
			cfgfile.append("NetConf.json");
			PublicTool::WriteFile(cfgfile, obj);
			break;
		}
		default:
		{
			log_error("SysCfgModule::HandSysCfgMsg unknow type = %d",type);
			return -1;
		}
	}
	//TODO:向客户端回数据
	return 0;
}

