#pragma once
#include "Device.h"
#include "DevStateDef.h"
#include "structdef.h"
#include <core/UserTypeDefines.h>

//负荷
#pragma pack(1)

typedef struct stLoadData
{
	Priority    	m_priority;				//优先级
	RealPowerInfo 	m_stPowerInfo;			//功率数据
	//PhaseVolt 	  	m_stPhaseVolt;			//相电压数据
	LineVolt      	m_stLineVolt;			//线电压
	PhaseCurr	  	m_stLineCurr;			//相电流
	UINT16			m_work_state;			//工作状态
	UINT16			m_alarm[4];				//故障告警

	stLoadData()
	{
		memset(this,0,sizeof(stLoadData));
	}
}LoadData;

#pragma pack()

class LoadDevice : public Device
{
public:
	LoadDevice();
	~LoadDevice();

public:
	virtual int  GetDataLen();
	virtual int TurnOnOrOff(RUNSTATE s)=0;
	
public:
	const LoadData* GetData();	
	void  SetPriority(Priority priority);
	virtual char* GetLocalData(int& nlen);	//获取本地显示屏数据
protected:
	virtual int SetDataPtr(char * ptr);
	
protected:
	LoadData* m_ld_data;
};