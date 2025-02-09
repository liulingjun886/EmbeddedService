#pragma once
#include "core/TimeEventHander.h"
#include "core/TimeNode.h"
#include "core/ASyncCallSupport.h"
#include "EMSServer.h"
#include "Device.h"
#include <vector>
#include "json/json.h"
#include "DieselManager.h"
#include "PVDeviceManager.h"
#include "ESSDeviceManager.h"
#include "DieselManager.h"
#include "LoadDeviceManager.h"


class PVDevice;
class ESSDevice;
class DieselDevice;
class LoadDevice;
class MeterDevice;


#pragma pack(1)

//能量调度条件
typedef struct stEnginerSchduleCondition
{
	int		threshold_dg_turnon;				//柴机启动阈值
	int		threshold_dg_turnoff;				//柴机关闭阈值
	int		threshold_ld_imp_turnoff;			//重要负载关闭阈值
	int		threshold_ld_imp_turnon;			//重要负载开启阈值
	int		threshold_ld_unimp_turnoff;			//次要负载关闭阈值
	int		threshold_ld_unimp_turnon;			//次要负载开启阈值
	int 	threshold_pv_turn_off;				//光伏关闭阈值
	int		threadhold_pv_limit;				//光伏限制为负载功率阈值
	int		threadhold_pv_unlimit;				//光伏不限功率阈值

	stEnginerSchduleCondition()
	{
		memset(this,0,sizeof(stEnginerSchduleCondition));
		threshold_dg_turnon = 150;
		threshold_dg_turnoff = 500;
		threshold_pv_turn_off = 950;
		threadhold_pv_limit = 930;
		threadhold_pv_unlimit = 860;
	}
}EnginerSchduleCondition;

//EMS本地数据
typedef struct stEMSData
{
	EnginerSchduleCondition esc;				//能量调度条件
}EMSData;

#pragma pack()


class EMSDevice : public Device, public EMSServer, public CTimeEventHander
{
public:
	EMSDevice();
	~EMSDevice();
	
public:	//作为主模块挂载入口
	virtual int GetThreadNum();
	//virtual int InitEMS();
	//virtual int StartEMS();
	
public:
	virtual int GetDataLen();
	virtual int ProcessTimeOutEvent(UINT32 nTimeId);		//定时器消息处理

protected:
	virtual int  SetDataPtr(char *ptr);
	virtual void StartDevice();
public:
	const EMSData* GetData();
	int  GetDeviceConnParam(int& connType,ASyncCallDataInst& pConnParam,const Json::Value& ConnParam = Json::Value::nullRef);//获取设备连接方式

public:
	void  EnergySchedule();					//能量管理

protected:
	int CreateDevices();									//创建子设备
	int CreatePvDevices(Json::Value& v);					//创建光伏系统设备
	int CreateEssDevices(Json::Value& v);					//创建储能设备
	int CreateDieselDevices(Json::Value& v);				//创建柴发设备
	int CreateLoadDevices(Json::Value& v);					//创建负载设备
	int CreateMeterDevices(Json::Value& v);					//创建电表设备
	void ExitPrograme();
private:
	void SetDevAsyncCallThread(CAsyncCallSupport* p, int nIndex);
protected:
	EMSData *m_ems_data;
	PVDeviceManager m_pvManager;					//光伏系统数组
	ESSDeviceManager m_essManager;					//储能系统数组
	DieselManager m_dieselManager;					//柴油发电系统数组
	LoadDeviceManager m_loadManager;				//负载数组
	std::vector<MeterDevice*> m_vec_meter;			//电表数组
	
	CTimer m_time_energy_schedule;					//能量调度定时器
};


