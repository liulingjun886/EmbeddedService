#pragma once
#include <string>
#include <json/json.h>
#include "core/UserTypeDefines.h"

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

class Device;
class EMSDevice;

class PublicTool
{
public:
	static const std::string& GetProgramLocation();//获取可执行文件存放路径
	static bool FileIsExists(const std::string& file_name);//判断文件是否存在
	static void* OpenSo(const std::string& soPath); //加载SO库
	static void* LoadFunAddr(void* pHander,const std::string& funName);	//加载动态库方法
	static Device* CreateDevice(const std::string& soPath,const std::string& model);
	static std::string ReadFile(const std::string& filename);
	static int WriteFile(const std::string& filename,const std::string& file_content);
	static int LoadJsonCfg(Json::Value& root,const std::string& cfgfile);
	static INT64 GetNowMSec();
};