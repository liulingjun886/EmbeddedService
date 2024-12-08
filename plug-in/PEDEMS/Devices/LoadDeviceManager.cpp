#include "LoadDeviceManager.h"
#include "LoadDevice.h"

LoadDeviceManager::LoadDeviceManager()
{
	m_vec_load.clear();
}

LoadDeviceManager::~LoadDeviceManager()
{
	
}

void LoadDeviceManager::AddLoadDev(LoadDevice* pDev)
{
	if(pDev)
		m_vec_load.push_back(pDev);
}

float LoadDeviceManager::GetNeedPower()
{
	float needPower = 0.f;
	for(std::vector<LoadDevice*>::iterator it = m_vec_load.begin(); it != m_vec_load.end(); ++it)
	{
		needPower += (*it)->GetData()->needp;
	}
	return needPower;
}

float LoadDeviceManager::SetPower(float p)
{
	
}
