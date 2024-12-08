#pragma once
#include <vector>
class DieselDevice;

class DieselManager
{
public:
	DieselManager();
	~DieselManager();
	
public:
	void  AddDieselDev(DieselDevice* pDev);
	float SetPower(float p);
	float DisPatchPower(float p);
	float GetMaxPower();
	
private:
	std::vector<DieselDevice*> m_vec_diesel;
};