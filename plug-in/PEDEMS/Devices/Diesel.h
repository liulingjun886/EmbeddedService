#pragma once
#include "Device.h"
#include "DevStateDef.h"
#include "structdef.h"

enum 
{
	
};

#pragma pack(1)

// 柴发数据结构
typedef struct stDGData
{
	INT16 			m_work_state;			// 工作状态
	INT16			m_work_mode;			// 工作模式
	AcInfo	        m_ac_info;				// 交流侧信息
	UINT16          m_generator_rot_speed;  // 发动机转速
	INT16 			m_oil_level;			// 油位
	INT16 			m_oil_temp;				// 油温
	UINT16          m_generator_temp;       // 发电机温度
	UINT16			m_mpg;					// 油耗
	UINT16 			m_gen_day;				// 当日发电量
	UINT32   		m_gen_total;			// 累计发电量
	UINT32	  		m_run_time;				// 累计运行时间
	UINT16 			m_alarm[4]; 			// 故障告警
	stDGData()
	{
		memset(this,0,sizeof(stDGData));
	}

}DGData;

#pragma pack()

//柴机管理父类
class DieselDevice : public Device
{
public:
	DieselDevice();
	~DieselDevice();
	
public:
	const DGData* GetData();
	
public:
	virtual int GetDataLen();
	virtual int TurnOnOrOff(RUNSTATE s)=0;						//设置柴机开关机
	virtual int SetWorkMode(WORKMODE wm)=0;						//设置工作模式
	virtual int SetPower(UINT32 p){return 0;}					//设置有功功率 正放 负充 单位kw
	virtual char* GetLocalData(int& nlen);						//显示屏数据
	
protected:
	virtual int SetDataPtr(char * ptr);
	
protected:
	DGData    *m_dg_data; 										//这里用指针，方便后期做内存映射	
};

