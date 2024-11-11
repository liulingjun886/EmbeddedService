#include "PublicTool.h"
#include <unistd.h>
#include <string.h>
#include "Log.h"
#include "Device.h"
#include "EMSDevice.h"
#include <errno.h>
#include <dlfcn.h>
#include <fstream>
#include <sstream>
#include <sys/time.h>

const std::string& PublicTool::GetProgramLocation()
{
	static std::string strProgramLocation = "";
	if(0 == strProgramLocation.length())
	{
		char buf[256] = {0};
		readlink("/proc/self/exe", buf, sizeof(buf));
		for(int i = strlen(buf); i >= 0; --i)
		{
			if(buf[i] != '/')
				buf[i] = 0;
			else
				break;
		}
		strProgramLocation = buf;
	}
	return strProgramLocation; // 0 is for heap memory
}

bool PublicTool::FileIsExists(const std::string& file_name)
{
    return (0 == access(file_name.c_str(), 0));
}

void* PublicTool::OpenSo(const std::string& soPath)
{
	void* pHander = dlopen(soPath.c_str(),RTLD_LAZY);
	if(!pHander)
	{
		log_error("%s load failer %d, %s", soPath.c_str(),errno, dlerror());
	}
	return pHander;
}

void* PublicTool::LoadFunAddr(void* pHander,const std::string& funName)
{
	void* pFun = dlsym(pHander,funName.c_str());
	if(!pFun)
	{
		log_error("load %s fun failer %s", funName.c_str(), dlerror());
	}
	return pFun;
}

Device* PublicTool::CreateDevice(const std::string& soPath, const std::string& model)
{
	void* pSoHander = OpenSo(soPath);
	if(!pSoHander)
		return nullptr;

	FunCreateDevice pCrete = (FunCreateDevice)LoadFunAddr(pSoHander, "CreateDevice");
	if(!pCrete)
		return nullptr;
	
	Device* pDev = pCrete(model);
	return pDev;
}

std::string PublicTool::ReadFile(const std::string& filename)
{
	std::string content = "";
	std::ifstream in(filename);
    if(!in.is_open())
        return "";
    std::ostringstream tmp;
    tmp << in.rdbuf();
    content = tmp.str();
    in.close();
    return content;
}

int PublicTool::WriteFile(const std::string& filename,const std::string& file_content)
{
	std::ofstream file(filename,std::ios::out);
    if(!file.is_open())
    {
        return -1;
    }
    file << file_content.c_str();
	file.flush();
    file.close();
    return 0;
}

int PublicTool::LoadJsonCfg(Json::Value& root,const std::string& cfgfile)
{
	std::string jsonCfg = PublicTool::ReadFile(PublicTool::GetProgramLocation() + "conf/" + cfgfile);
	Json::Reader reader;
	if(!reader.parse(jsonCfg,root))
	{
		return -1;
	}
	return 0;
}

INT64 PublicTool::GetNowMSec()
{
	static struct timeval tv;
	gettimeofday(&tv, NULL);
  	return tv.tv_sec *1000 + tv.tv_usec * 0.001;
}
/*
Device* PublicTool::CreateDevice1i(const std::string& soPath,int parma1)
{
	void* pSoHander = OpenSo(soPath);
	if(!pSoHander)
		return nullptr;

	FunCreateDevice1i pCrete = (FunCreateDevice1i)LoadFunAddr(pSoHander, "CreateDevice");
	if(!pCrete)
		return nullptr;
	
	Device* pDev = pCrete(parma1);
	return pDev;
}

Device* PublicTool::CreateDevice2i(const std::string& soPath,int parma1,int param2)
{
	void* pSoHander = OpenSo(soPath);
	if(!pSoHander)
		return nullptr;

	FunCreateDevice2i pCrete = (FunCreateDevice2i)LoadFunAddr(pSoHander, "CreateDevice");
	if(!pCrete)
		return nullptr;
	
	Device* pDev = pCrete(parma1,param2);
	return pDev;
}*/