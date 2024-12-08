#pragma once
#include "Device.h"
#include "DevStateDef.h"

//光伏系统数据结构
typedef struct stPVData 
{
	float p;
}PVData;

class PVDevice : public Device
{
public:
	PVDevice();
	~PVDevice();
	
public:
	virtual int  GetDataLen();
	
protected:
	virtual int SetDataPtr(char * ptr);
	
public:
	const PVData* GetData();
	
public:
	virtual int TurnOnOrOff(RUNSTATE s)=0;				//1 开机 0 关机
	virtual int SetP(float p)=0;						//设置有功功率 正放 负充 单位kw
	virtual int SetP(float pA, float pB, float pC)=0;	//设置分三相设置有功功率
	virtual int SetAppaP(float p)=0;					//设置无功功率
	virtual int SetPowerFactor(float factor)=0;			//设置功率因数
	virtual int GetRatePower()=0;						//获取PCS额定功率
	
protected:
	PVData    *m_pv_data; 								//这里用指针，方便后期做内存映射
};