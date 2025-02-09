#include "ESSDeviceManager.h"
#include "ESSDevice.h"

ESSDeviceManager::ESSDeviceManager()
{
	m_vec_ess.clear();
}

ESSDeviceManager::~ESSDeviceManager()
{
	
}

void  ESSDeviceManager::AddEssDev(ESSDevice* pDev)
{
	m_vec_ess.push_back(pDev);
}

float ESSDeviceManager::SetPower(float p)
{
	return p;
}

float ESSDeviceManager::GetPower()
{
	float p = 0.f;
	for(std::vector<ESSDevice*>::iterator it = m_vec_ess.begin(); it != m_vec_ess.end(); ++it)
	{
		p += (*it)->GetData()->m_ac_info.m_power_info.m_p;
	}
	return p;
}


float ESSDeviceManager::DisPatchPower(float p)
{
	if(0.f == p)
		return 0;
	
	float power = 0.f;
	if(p < 0.f)
	{
		power = GetMaxChargePower();
		if(-p < power)
		{	
			SetPower(p);
			return 0.f;
		}
		else
		{
			SetPower(-power);
			return p+power;
		}
	}
	else
	{
		power = GetMaxDisChargePower();
		if(power > p)
		{
			SetPower(p);
			return 0.f;
		}
		else
		{
			SetPower(power);
			return p - power;
		}
	}
	return 0.f;
}

int   ESSDeviceManager::GetSoc()
{
	int nSoc = 0,nNum = 0;
	for(std::vector<ESSDevice*>::iterator it = m_vec_ess.begin(); it != m_vec_ess.end(); ++it)
	{
		nSoc += (*it)->GetData()->m_soc;
		++nNum;
	}

	if(nNum > 0)
	{
		return nSoc/nNum;
	}

	return 0;
}

float ESSDeviceManager::GetMaxChargePower()
{
	
}

float ESSDeviceManager::GetMaxDisChargePower()
{
	
}

