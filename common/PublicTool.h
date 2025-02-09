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
	static const std::string& GetProgramLocation();//获取可执行文件存放路径,
	static const std::string& GetNullString();
	static bool FileIsExists(const std::string& file_name);//判断文件是否存在
	static int  ExecShellCommand(const std::string& file_name);			//删除文件
	static void* OpenSo(const std::string& soPath,bool global = false); //加载SO库
	static void* LoadFunAddr(void* pHander,const std::string& funName);	//加载动态库方法
	static Device* CreateDevice(const std::string& soPath,const std::string& model,bool global = false);//创建设备
	static std::string ReadFile(const std::string& filename);			//读取文件
	static int WriteFile(const std::string& filename,const std::string& file_content);//写文件
	static int LoadJsonCfg(Json::Value& root,const std::string& cfgfile);//	加载json文件
	static INT64 GetNowMSec();	//获取当时间的毫秒数
	static UINT32 MakeUint32CDAB(UINT32 data);
	static UINT32 MakeUint32CDAB(UINT16 data1, UINT16 data2);
};