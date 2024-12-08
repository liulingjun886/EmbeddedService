#pragma once
#include <vector>

class PVDevice;

class PVDeviceManager
{
public:
	PVDeviceManager();
	~PVDeviceManager();

public:
	void  AddPvDev(PVDevice* pDev);
	float SetPower(float p);
	float DisPatchPower(float p);
	
private:
	float GetTotalPower();
	
private:
	std::vector<PVDevice*> m_vec_pv;
};