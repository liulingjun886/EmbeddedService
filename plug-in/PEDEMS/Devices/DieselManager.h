#pragma once
#include <vector>
#include <core/UserTypeDefines.h>

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
	UINT32 GetPower();						//获取当前柴发输出功率
	
	void	Start();
	void 	Stop();
private:
	std::vector<DieselDevice*> m_vec_diesel;
};