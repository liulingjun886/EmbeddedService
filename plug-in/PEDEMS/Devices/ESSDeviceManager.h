#pragma once
#include <vector>

class ESSDevice;


class ESSDeviceManager
{
public:
	ESSDeviceManager();
	~ESSDeviceManager();
public:
	void  AddEssDev(ESSDevice* pDev);
	float SetPower(float p);
	float GetPower();
	float DisPatchPower(float p);
	int   GetSoc();

	float GetMaxChargePower();
	float GetMaxDisChargePower();
	
private:
	std::vector<ESSDevice*> m_vec_ess;
};

