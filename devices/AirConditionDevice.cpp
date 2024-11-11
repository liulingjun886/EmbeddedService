#include "AirConditionDevice.h"



AirConditionDevice::AirConditionDevice():m_pAirConditionData(nullptr)
{
	
}

AirConditionDevice::~AirConditionDevice()
{
	if(m_bFreeDevdData)
		delete m_pAirConditionData;
}

int AirConditionDevice::SetDataPtr(char* ptr)
{
	if(ptr)
	{
		int nBasicDataLen = Device::SetDataPtr(ptr);
		m_pAirConditionData = new (ptr + nBasicDataLen) AirConditionData;
		return sizeof(AirConditionData)+nBasicDataLen;
	}

	m_pAirConditionData = new AirConditionData;
	return 0;
}

int AirConditionDevice::GetDataLen()
{
	return sizeof(AirConditionData) + Device::GetDataLen();
}

