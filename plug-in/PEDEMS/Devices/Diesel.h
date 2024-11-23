#pragma once
#include "Device.h"
#include "DevStateDef.h"

#pragma pack(1)

typedef struct stDEData
{
	float p;
}DEData;

#pragma pack()

//柴机管理父类
class DieselDevice : public Device
{
public:
	DieselDevice();
	~DieselDevice();
	
public:
	const DEData* GetData();
	
public:
	virtual int GetDataLen();
	virtual int TurnOnOrOff(RUNSTATE s)=0;			//设置柴机开关机
	virtual int SetPower(float p)=0;					//设置有功功率 正放 负充 单位kw
	
protected:
	virtual int SetDataPtr(char * ptr);
	
protected:
	DEData    *m_de_data; 								//这里用指针，方便后期做内存映射	
};

