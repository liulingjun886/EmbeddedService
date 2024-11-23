#pragma once
#include "Device.h"
#include "DevStateDef.h"

//光伏系统数据结构
#pragma pack(1)

typedef struct stESSData 
{
	float p;
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
	virtual int TurnOnOrOff(RUNSTATE s)=0;			//设置开关机
	virtual int SetRunMode(RUNMODE m)=0;				//设置储能运行模式
	virtual int SetPower(float p)=0;					//设置有功功率 正放 负充 单位kw
	
protected:
	virtual int SetDataPtr(char * ptr);
	
protected:
	ESSData    *m_ess_data; 								//这里用指针，方便后期做内存映射
};

