#pragma once
#include <vector>
#include <core/UserTypeDefines.h>

class PVDevice;

class PVDeviceManager
{
public:
	PVDeviceManager();
	~PVDeviceManager();

public:
	void  AddPvDev(PVDevice* pDev);
	float SetPower(UINT32 p);
	UINT32 GetPower();
	float DisPatchPower(float p);
	
private:
	float GetTotalPower();
	
private:
	std::vector<PVDevice*> m_vec_pv;
};