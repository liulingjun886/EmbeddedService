#include "PVDeviceManager.h"
#include "PVDevice.h"

PVDeviceManager::PVDeviceManager()
{
	m_vec_pv.clear();
}

PVDeviceManager::~PVDeviceManager()
{
	
}

void  PVDeviceManager::AddPvDev(PVDevice* pDev)
{
	if(pDev)
		m_vec_pv.push_back(pDev);
}

float PVDeviceManager::SetPower(float p)
{
	
}

float PVDeviceManager::DisPatchPower(float p)
{
	float totalP = GetTotalPower();
	return p - totalP;
}


float PVDeviceManager::GetTotalPower()
{
	float p = 0.f;
	for(std::vector<PVDevice*>::iterator it = m_vec_pv.begin(); it != m_vec_pv.end(); ++it)
	{
		p += (*it)->GetData()->p;
	}
	return p;
}

