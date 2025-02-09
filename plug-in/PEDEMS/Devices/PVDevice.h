#pragma once
#include "Device.h"
#include "DevStateDef.h"
#include "structdef.h"
#include <core/UserTypeDefines.h>

#pragma pack(1)

//光伏系统数据结构
typedef struct stPVData 
{
	UINT16			m_work_state;			//工作状态
	UINT16			m_work_mode;			//工作模式
	AcInfo			m_ac_info;				//交流侧数据
	UINT32			m_dc_power;				//直流功率
	UINT16			m_convert_effic;		//转换效率
	UINT32			m_run_time;				//运行时间
	UINT16			m_inner_temp;			//内部温度
	UINT16          m_gen_power_day;		//当日发电量
	UINT16 			m_gen_power_month;		//当月发电量
	UINT32			m_gen_power_year;		//年发电量
	UINT32			m_gen_power_total;		//累计发电量
	UINT16			m_alarm[4];				//告警数据

	stPVData()
	{
		memset(this,0,sizeof(stPVData));
	}
}PVData;

#pragma pack()

class PVDevice : public Device
{
public:
	PVDevice();
	~PVDevice();
	
public:
	virtual int  GetDataLen();
	virtual char* GetLocalData(int& nlen);	//获取本地显示屏数据
protected:
	virtual int SetDataPtr(char * ptr);
	
public:
	const PVData* GetData();
	
public:
	virtual int TurnOnOrOff(RUNSTATE s)=0;				//1 开机 0 关机
	virtual int SetPower(UINT32 p)=0;					//设置有功功率 正放 负充 单位kw	
protected:
	PVData    *m_pv_data; 								//这里用指针，方便后期做内存映射
};