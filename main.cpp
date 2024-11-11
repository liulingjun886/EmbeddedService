#include "core/Reactor.h"
#include <iostream>
#include <unistd.h>
#include "Log.h"
#include "PublicTool.h"
#include "EMSDevice.h"
#include "json/json.h"
#include "SysCfgModule.h"

int main(int argc, char* argv[])
{	
	int nThreadNum = 1;
	int nLogLevel = LOG_INFO;
	std::string logFileName = "log";
	std::string productName = "";		//产品名称
	std::string module = "";			//产品型号

	//加载系统配置文件
	Json::Value root;
	if(!PublicTool::LoadJsonCfg(root, "syscfg.json"))
	{			
		if(root["LogLevel"].isInt())
			nLogLevel = root["LogLevel"].asInt();
			
		if(root["Product"].isString())
			productName = root["Product"].asString();

		if(root["Model"].isString())
			module = root["Model"].asString();
	}

	//根据参数初始化CReeactor模块和日志模块
	CReactor::GetInstance()->Init(1);
	Log::GetInstance()->SetThreadIndex(0);
	Log::GetInstance()->InitLog(logFileName.c_str(),(LOG_LEVEL)nLogLevel);

	//创建系统配置模块
	if(SysCfgModule::GetInstance()->Init())
	{
		log_error("SysCfgModule Init failer");
	}
	
	//创建并启动EMS模块
	do
	{
		if("" == productName)
			break;
		std::string emsSo = PublicTool::GetProgramLocation()+"plug_in/";
		emsSo.append(productName);
		EMSDevice* pEms = dynamic_cast<EMSDevice*>(PublicTool::CreateDevice(emsSo+".so",module));
		if(!pEms)
			break;
		
		CReactor::GetInstance()->AddThreads(pEms->GetThreadNum());
		
		if(pEms->InitEMS())
		{
			log_error("InitEms failer ...");
			break;
		}

		if(pEms->StartEMS())
		{
			log_error("StartEms failer ...");
			break;
		}
	}while(false);

	CReactor::GetInstance()->RunLoop();
	return 0;
}
