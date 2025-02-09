#include "LoadDevice.h"

LoadDevice::LoadDevice()
{

}

LoadDevice::~LoadDevice()
{
	m_ld_data->~LoadData();
}

int  LoadDevice::GetDataLen()
{
	return sizeof(LoadData) + Device::GetDataLen();
}

int LoadDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_ld_data = new (ptr + nBasicDataLen) LoadData;
	return sizeof(LoadData)+nBasicDataLen;
}

const LoadData* LoadDevice::GetData()
{
	return m_ld_data;
}

void  LoadDevice::SetPriority(Priority priority)
{
	m_ld_data->m_priority = priority;
}

char* LoadDevice::GetLocalData(int& nlen)
{
	nlen = sizeof(LoadData);
	return (char*)m_ld_data;
}

