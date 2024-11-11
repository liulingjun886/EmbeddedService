#include "DehumidifierDevice.h"

DehumidifierDevice::DehumidifierDevice():m_pHumidifyData(nullptr)
{
	
}

DehumidifierDevice::~DehumidifierDevice()
{
	if(m_bFreeDevdData)
	{
		delete m_pHumidifyData;
		m_pHumidifyData = nullptr;
	}
}

int DehumidifierDevice::SetDataPtr(char* ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	if(ptr)
	{
		m_pHumidifyData = new (ptr + nBasicDataLen) DehumidifierData;
		return sizeof(DehumidifierData)+nBasicDataLen;
	}

	m_pHumidifyData = new DehumidifierData;
	return 0;
}

const DehumidifierData* DehumidifierDevice::GetData()
{
	return m_pHumidifyData;
}

