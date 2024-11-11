/*
	设备基类，此类主要管理设备基础信息例如：sn,型号，设备名称，在线状态及启用状态
	此类还维护了一套设备层级的拓朴结构
*/
#pragma once

#include <vector>
#include <string>
#include "core/ShareData.h"


enum RUNSTATE
{
	OFFLINE = 0, //离线
	ONLINE		//在线
};

enum DEVICE_WORKING_STATE
{
	DISABLE=0,	//禁用
	ENABLE,		//启用
};

enum COMMUNITE_TYPE
{
	SYNC=0,		//同步方式
	ASYNC,		//异步方式
};

struct DevBasicInfo;
class Device;

typedef Device* (*FunCreateDevice)(const std::string& param);

#pragma pack(1)
struct DevBasicInfo
{
	UINT16 m_n_type;							//设备类型
	char   m_str_device_sn[20];					//设备SN
	char   m_str_model[20];						//设备型号
	char   m_str_name[20];						//设备名称
	RUNSTATE  m_n_run_state;					//设备在线状态
};
#pragma pack()

class Device
{
public:
	Device();
	virtual ~Device();
	
public:
	int  InitData(char* ptr = nullptr);				//初始化设备数据类存
	int  Init(const std::string& sn, const std::string& model, const std::string& name, int cmd=0, ASyncCallDataInst& initdata = ASyncCallDataInst::GetInstance());//初始化基本信息
	const DevBasicInfo* GetDevBasicInfo();
	void SetParent(Device* pDev);
	Device* GetParent();
	Device* GetTopDevice();
	void AddChild(Device* pDev);
	const std::vector<Device*>& GetChildren();
	bool SetRunState(RUNSTATE state);
	void SetEnableState(DEVICE_WORKING_STATE state);
	DEVICE_WORKING_STATE GetEnableState() const;
	void SetQuicklyInterval(UINT32 msec);
	void SetStandardlyInterval(UINT32 msec);
	void SetSlowlyInterval(UINT32 msec);
	
public:
	virtual int GetDataLen();				//获取数据长度
	virtual int GetSysFault();				//是否产生系统故障要停止 0-不需要 其他-需要
	
protected:
	virtual int  Initialize(int cmd, ASyncCallDataInst& initdata)=0;	//子设备初始化
	virtual int  SetDataPtr(char* ptr);			//设置数据接口指针
	virtual void StartDevice(){}      			//启用设备
	virtual void StopDevice(){}	        		//停用设备

protected:
	DevBasicInfo* m_pDevBasicInfo;				//设备基础信息
	bool		  m_bFreeDevdData;				//是否需要回收设备信息
	Device* m_p_parent;							//父设备
	std::vector<Device*> m_vec_children; 		//子设备
	UINT32 		  m_quickly_interval;			//紧急数据查询间隔毫秒
	UINT32		  m_standardly_interval;		//正常数据查询间隔毫秒
	UINT32 		  m_slowly_interval;			//不紧急数据查询间隔秒
private:
	DEVICE_WORKING_STATE m_n_state_enable;		//启用状态
};