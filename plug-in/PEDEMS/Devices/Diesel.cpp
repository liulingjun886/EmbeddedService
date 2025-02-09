#include "Diesel.h"

DieselDevice::DieselDevice()
{

}

DieselDevice::~DieselDevice()
{
	
}

const DGData* DieselDevice::GetData()
{
	return m_dg_data;
}

int DieselDevice::GetDataLen()
{
	return sizeof(DGData) + Device::GetDataLen();
}

int DieselDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	m_dg_data = new (ptr + nBasicDataLen) DGData;
	return sizeof(DGData)+nBasicDataLen;
}

char* DieselDevice::GetLocalData(int& nlen)
{
	nlen = sizeof(DGData);
	return (char*)m_dg_data;
}