#include "PVDeviceManager.h"
#include "PVDevice.h"
#include "Log.h"

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

float PVDeviceManager::SetPower(UINT32 TotalP)
{
	UINT32 p = TotalP/m_vec_pv.size();
	log_debug("PVDeviceManager::SetPower = %d",p);
	for(std::vector<PVDevice*>::iterator it = m_vec_pv.begin();  it != m_vec_pv.end(); ++it)
	{
		(*it)->SetPower(p);
	}
}

float PVDeviceManager::DisPatchPower(float p)
{
	float totalP = GetPower();
	return p - totalP;
}


UINT32 PVDeviceManager::GetPower()
{
	UINT32 p = 0.f;
	for(std::vector<PVDevice*>::iterator it = m_vec_pv.begin(); it != m_vec_pv.end(); ++it)
	{
		p += (*it)->GetData()->m_ac_info.m_power_info.m_p;
	}
	return p;
}

