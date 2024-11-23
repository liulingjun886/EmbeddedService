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

