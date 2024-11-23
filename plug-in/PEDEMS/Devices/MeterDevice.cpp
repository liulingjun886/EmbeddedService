#include "MeterDevice.h"

MeterDevice::MeterDevice():m_pMeterData(nullptr)
{
	
}

MeterDevice::~MeterDevice()
{
	
}

const MeterData* MeterDevice::GetData()
{
	return m_pMeterData;
}

int  MeterDevice::GetDataLen()
{
	return sizeof(MeterData) + Device::GetDataLen();
}

int MeterDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_pMeterData = new (ptr + nBasicDataLen) MeterData;
	return sizeof(MeterData)+nBasicDataLen;
}

