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
	float DisPatchPower(float p);
private:
	float GetMaxChargePower();
	float GetMaxDisChargePower();
private:
	std::vector<ESSDevice*> m_vec_ess;
};

