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
	
}

float DieselManager::DisPatchPower(float p)
{
	
}


float DieselManager::GetMaxPower()
{
	
}

