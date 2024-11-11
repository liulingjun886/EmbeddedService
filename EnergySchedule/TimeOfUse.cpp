#include "TimeOfUse.h"
#include "EMSDevice.h"
#include <sys/time.h>

static inline UINT32 calTime(int hour,int min)
{
	return hour*60 + min;
}

TOU::TOU(EMSDevice* pEmsDev):EnergySchedule(pEmsDev),m_tou_power(0.f),m_antibackflow(0.f)
{
	m_pTouTimer.InitTimerNode(this, 1);
	m_GridPowerMonitorTimer.InitTimerNode(this, 2);

	const EMSData* pEmsData = m_pEmsDevice->GetEmsData();					//获取负载功率
	m_pFunAntibackflow = (0 == pEmsData->m_u16_pv_consume ? &TOU::NotConsumePv : &TOU::ConsumePv);
	m_pFunDemand = (0 == pEmsData->m_u16_demand_dynamic ? &TOU::NotConsumDemand : &TOU::ConsumDemand);
}

TOU::~TOU()
{
	
}

void TOU::Start()
{
	m_pTouTimer.StartTimerSec(1, -1);
	m_GridPowerMonitorTimer.StartTimer(180, -1);
}

void TOU::Stop()
{
	m_pTouTimer.StopTimer();
	m_GridPowerMonitorTimer.StopTimer();
}

int TOU::ProcessTimeOutEvent(UINT32 nTimeId)
{
	switch(nTimeId)
	{
		case 1:
		{
			const EMSData* pEmsData = m_pEmsDevice->GetEmsData();
			const TOUTime (&pTou)[10] = pEmsData->m_sz_tou;
			INT16 set_power = 0;
			struct tm *p;
			time_t timep;
			time(&timep);
			p=localtime(&timep);

			int nowMin = calTime(p->tm_hour,p->tm_min);
			
			for(int i = 0; i < 10; ++i)
			{
				if(nowMin >= calTime(pTou[i].m_u16_starthour, pTou[i].m_u16_startmin) && nowMin <= calTime(pTou[i].m_u16_endhour, pTou[i].m_u16_endmin))
				{
					set_power = pTou[i].m_i16_Power;
					break;
				}
			}

			if(set_power != m_tou_power)
			{
				m_tou_power = set_power;
				CalPower();
			}
			break;
		}
		case 2:
		{
			CalPower();	//TOU模式下需要将需量和防逆流都考虑进去
			break;
		}
		default:
			break;
	}

	//CalPower();				
								
	return 0;
}

void TOU::CalPower()
{
	const EMSData* pEmsData = m_pEmsDevice->GetEmsData();
	UINT16 soc = m_pEmsDevice->GetSoc();
	(this->*m_pFunAntibackflow)();			//计算防逆流值
	(this->*m_pFunDemand)();
	float SetPower = m_tou_power + m_antibackflow + m_demand;
	
	if(SetPower < 0 && soc >= pEmsData->m_u16_maxsoc)
		SetPower = 0;
	else if(SetPower > 0 && soc >= pEmsData->m_u16_minsoc)
		SetPower = 0;

	m_pEmsDevice->SetPower(SetPower);

	/*if(m_tou_power > 0.f)
	{
		m_demand = 0.f;
		float gridmeterPower = m_pEmsDevice->GetGridMeterActivePower() + antibackflow;
		m_antibackflow += (gridmeterPower * 0.5);
		if(m_antibackflow > 0.1f)
			m_antibackflow = 0.f;
	}
	else if(m_tou_power < 0.f)
	{
		m_antibackflow = 0.f;
		float gridmeterPower = m_pEmsDevice->GetGridMeterActivePower() - demand;
		m_demand += (gridmeterPower * 0.5);
		if(m_demand < 0.f)
			m_demand = 0.f;
	}

	
	//首先保证防逆流和需量满足要求
	
	float SetPower = m_tou_power;

	if(loadP < 0.f)													//防逆流底线。考虑有光伏接入等因素的自消纳
	{
		SetPower = loadP;
	}
	else if(m_tou_power > 0.f)										//放电时防逆流
	{
		if(m_tou_power > loadP)
			SetPower = loadP;
		SetPower -= antibackflow;									//减去防逆流阈值
	}
	else if(m_tou_power < 0.f)										//充电启量控制
	{
		if((-m_tou_power) + loadP > demand)							//充电功率+负载功率 > 需量启动功率
		{
			SetPower = -(demand - loadP);
		}
	}
	m_pEmsDevice->SetPower(SetPower);*/
}

void TOU::ConsumePv()
{
	const EMSData* pEmsData = m_pEmsDevice->GetEmsData();
	float antibackflow = pEmsData->m_u16_power_antibackflow;				//防逆流阈值
	float gridmeterPower = m_pEmsDevice->GetGridMeterActivePower();

	float delta = gridmeterPower - antibackflow;

	m_antibackflow += delta * 0.5;

	if(m_antibackflow > 0)
		m_antibackflow = 0;
}

void TOU::NotConsumePv()
{
	if(m_tou_power <= 0)
	{
		m_antibackflow = 0;
		return;
	}

	ConsumePv();

	if(-m_antibackflow > m_tou_power)
		m_antibackflow = -m_tou_power;
}

void TOU::ConsumDemand()				//放电考虑需量
{
	const EMSData* pEmsData = m_pEmsDevice->GetEmsData();
	float demand = pEmsData->m_u16_power_demand;							//需量值
	float gridmeterPower = m_pEmsDevice->GetGridMeterActivePower();

	float delta = gridmeterPower - demand;

	m_demand += delta * 0.5f;
	if(m_demand < 0)
		m_demand = 0;
}

void TOU::NotConsumDemand()				//放电不考虑需量
{
	if(m_tou_power >= 0)
	{
		m_demand = 0;
		return;
	}

	ConsumDemand();

	if(-m_demand < m_tou_power)
		m_demand = -m_tou_power;
}

