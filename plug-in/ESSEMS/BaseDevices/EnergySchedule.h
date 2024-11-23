#pragma once

class EMSDevice;

class EnergySchedule
{
public:
	EnergySchedule(EMSDevice* pEmsDevice):m_pEmsDevice(pEmsDevice)
	{
		
	}
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
	
protected:
	EMSDevice* const m_pEmsDevice;
};