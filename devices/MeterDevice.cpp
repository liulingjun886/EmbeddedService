#include "MeterDevice.h"

MeterDevice::MeterDevice():m_pMeterData(nullptr)
{
	
}

MeterDevice::~MeterDevice()
{
	if(m_bFreeDevdData)
	{
		delete m_pMeterData;
		m_pMeterData = nullptr;
	}
}

const MeterData* MeterDevice::GetData()
{
	return m_pMeterData;
}

int MeterDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_pMeterData = new (ptr + nBasicDataLen) MeterData;
	return sizeof(MeterData)+nBasicDataLen;
}

