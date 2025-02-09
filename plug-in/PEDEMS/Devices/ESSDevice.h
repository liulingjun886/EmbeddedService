#pragma once
#include "Device.h"
#include "DevStateDef.h"
#include "structdef.h"

//光伏系统数据结构
#pragma pack(1)

// 储能系统数据结构
typedef struct stESSData 
{
	AcInfo			m_ac_info;				//交流侧信息
	UINT16 			m_soc;					//电池soc
	UINT16 			m_soh;					//电池soh
	UINT16 			m_work_state;			//工作状态
	UINT16 			m_work_mode;			//工作模式
	float  			m_batt_volt;			//电池电压
	float 			m_batt_curr;			//电池电流
	float 			m_max_charge_power;		//最大充电功率
	float 			m_max_discharge_power;	//最大放电功率
	float 			m_charge_p_day;			//当日充电量
	float 			m_discharge_p_day;		//当日放电量
	float 			m_charge_p_total;		//累计充电量
	float 			m_discharge_p_total;	//累计放电量
	UINT16 			m_cell_max_volt;		//单体最高电压
	UINT16 			m_cell_max_temp;		//单体最高温度
	UINT16 			m_cell_min_volt;		//单体最低电压
	UINT16 			m_cell_min_temp;		//单体最低温度
	UINT16 			m_alarm[8];				//故障告警

	stESSData()
	{
		memset(this,0,sizeof(stESSData));
	}
}ESSData;

#pragma pack()

//储能系统模块
class ESSDevice : public Device
{
public:
	ESSDevice();
	~ESSDevice();
	
public:
	const ESSData* GetData();
	
public:
	virtual int GetDataLen();
	virtual int TurnOnOrOff(RUNSTATE s)=0;				//设置开关机
	virtual int SetWorkMode(WORKMODE m)=0;				//设置储能运行模式
	virtual int SetPower(float p)=0;					//设置有功功率 正放 负充 单位kw
	virtual char* GetLocalData(int& nlen);				//获取本地显示屏数据
	
protected:
	virtual int SetDataPtr(char * ptr);
	
protected:
	ESSData    *m_ess_data; 								//这里用指针，方便后期做内存映射
};

