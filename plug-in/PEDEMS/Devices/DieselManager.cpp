#include "DieselManager.h"
#include "Diesel.h"

DieselManager::DieselManager()
{
	m_vec_diesel.clear();
}

DieselManager::~DieselManager()
{
	
}

void  DieselManager::AddDieselDev(DieselDevice* pDev)
{
	if(pDev)
		m_vec_diesel.push_back(pDev);
}

float DieselManager::SetPower(float p)
{
	int num = m_vec_diesel.size();
	p = p / num;
	for(std::vector<DieselDevice*>::iterator it = m_vec_diesel.begin(); it != m_vec_diesel.end(); ++it)
	{
		(*it)->SetPower(p);
	}
}

float DieselManager::DisPatchPower(float p)
{
	
}

float DieselManager::GetMaxPower()
{
	
}

UINT32 DieselManager::GetPower()
{
	UINT32 p = 0;
	for(std::vector<DieselDevice*>::iterator it = m_vec_diesel.begin(); it != m_vec_diesel.end(); ++it)
	{
		p += (*it)->GetData()->m_ac_info.m_power_info.m_p;
	}
	return p;
}

void DieselManager::Start()
{
	for(std::vector<DieselDevice*>::iterator it = m_vec_diesel.begin(); it != m_vec_diesel.end(); ++it)
	{
		(*it)->TurnOnOrOff(TURNON);
	}
}

void DieselManager::Stop()
{
	for(std::vector<DieselDevice*>::iterator it = m_vec_diesel.begin(); it != m_vec_diesel.end(); ++it)
	{
		(*it)->TurnOnOrOff(TURNOFF);
	}
}

