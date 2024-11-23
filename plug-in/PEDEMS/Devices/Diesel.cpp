#include "Diesel.h"

DieselDevice::DieselDevice()
{
}

DieselDevice::~DieselDevice()
{
}

int DieselDevice::GetDataLen()
{
	return sizeof(DEData) + Device::GetDataLen();
}

int DieselDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_de_data = new (ptr + nBasicDataLen) DEData;
	return sizeof(DEData)+nBasicDataLen;
}

const DEData* DieselDevice::GetData()
{
	return m_de_data;
}