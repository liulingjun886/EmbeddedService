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


float ESSDeviceManager::GetMaxChargePower()
{
	
}

float ESSDeviceManager::GetMaxDisChargePower()
{
	
}

