#include "ESSDevice.h"

ESSDevice::ESSDevice()
{
}

ESSDevice::~ESSDevice()
{
}

const ESSData* ESSDevice::GetData()
{
	return m_ess_data;
}

int ESSDevice::GetDataLen()
{
	return sizeof(ESSData) + Device::GetDataLen();
}

int ESSDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_ess_data = new (ptr + nBasicDataLen) ESSData;
	return sizeof(ESSData)+nBasicDataLen;
}


