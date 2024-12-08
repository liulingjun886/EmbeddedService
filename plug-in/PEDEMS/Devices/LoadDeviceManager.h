#pragma once
#include <vector>

class LoadDevice;

class LoadDeviceManager
{
public:
	LoadDeviceManager();
	~LoadDeviceManager();

public:
	void AddLoadDev(LoadDevice* pDev);
	float GetNeedPower();
	float SetPower(float p);
	
private:
	std::vector<LoadDevice*> m_vec_load;
};
