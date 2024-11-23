#pragma once
#include "EnergySchedule.h"
#include "core/TimeNode.h"
#include "core/TimeEventHander.h"

class EMSDevice;

class TOU : public EnergySchedule,public CTimeEventHander
{
public:
	TOU(EMSDevice* pEmsDev);
	~TOU();

	virtual void Start();
	virtual void Stop();
	virtual int ProcessTimeOutEvent(UINT32);
	
private:
	void CalPower();
	void ConsumePv();					//光伏消纳
	void NotConsumePv();				//光伏不消纳
	void ConsumDemand();				//放电考虑需量
	void NotConsumDemand();				//放电不考虑需量
	
private:
	CTimer m_pTouTimer;
	CTimer m_GridPowerMonitorTimer;
	float  m_tou_power;
	float  m_set_power;
	float  m_antibackflow;				//扣除的逆流值此值只会为负且只在放电时生效
	float  m_demand;					//扣除的需量值此值只会为正且只在充电时生效

	void (TOU::*m_pFunAntibackflow)();	//防逆流逻辑
	void (TOU::*m_pFunDemand)();		//防逆流逻辑
};