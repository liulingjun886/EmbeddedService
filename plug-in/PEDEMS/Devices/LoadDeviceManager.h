#pragma once
#include <vector>
#include "LoadDevice.h"
#include "DevStateDef.h"

class LoadDeviceManager
{
public:
	LoadDeviceManager();
	~LoadDeviceManager();

public:
	void AddLoadDev(LoadDevice* pDev);
	float GetNeedPower();
	float SetPower(float p);

	void  	StartLoad(Priority priority);
	void	StopLoad(Priority priority);
	
private:
	std::vector<LoadDevice*> m_vec_load;
};
