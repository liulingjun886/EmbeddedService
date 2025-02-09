#include "PVDevice.h"

PVDevice::PVDevice()
{
}

PVDevice::~PVDevice()
{
}

int  PVDevice::GetDataLen()
{
	return sizeof(PVData)+Device::GetDataLen();
}

int PVDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_pv_data = new (ptr + nBasicDataLen) PVData;
	return sizeof(PVData)+nBasicDataLen;
}

const PVData* PVDevice::GetData()
{
	return m_pv_data;
}

char* PVDevice::GetLocalData(int& nlen)
{
	nlen = sizeof(PVData);
	return (char*)m_pv_data;
}

