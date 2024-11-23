#include "LiquidCoolerDevice.h"

LiquidCoolerDevice::LiquidCoolerDevice():m_LiquidCoolerData(nullptr)
{
	
}

LiquidCoolerDevice::~LiquidCoolerDevice()
{
	if(m_bFreeDevdData)
	{
		delete m_LiquidCoolerData;
		m_LiquidCoolerData = nullptr;
	}
	
}

void LiquidCoolerDevice::PowerOnOrOff(LiquidCoolerWorkState state)
{
	(void)state;
	return;
}

void LiquidCoolerDevice::SetWorkMode(LiquidCoolerWorkMode mode)
{
	(void)mode;
	return;
}

int  LiquidCoolerDevice::SetDataPtr(char* ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	if(ptr)
	{
		m_LiquidCoolerData = (LiquidCoolerData *)(ptr + nBasicDataLen);
		return sizeof(LiquidCoolerData)+nBasicDataLen;
	}

	m_LiquidCoolerData = new LiquidCoolerData;
	return 0;
}

const LiquidCoolerData* LiquidCoolerDevice::GetData()
{
	return m_LiquidCoolerData;
}
