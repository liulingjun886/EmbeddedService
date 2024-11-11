#include "PCSDevice.h"

PCSDevice::PCSDevice():m_pcs_data(nullptr)
{
	
}

PCSDevice::~PCSDevice()
{
	if(m_bFreeDevdData)
	{
		delete m_pcs_data;
		m_pcs_data = nullptr;
	}
}

const PCSData* PCSDevice::GetData()
{
	return m_pcs_data;
}

int PCSDevice::SetDataPtr(char * ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	if(ptr)
	{
		m_pcs_data = new (ptr + nBasicDataLen) PCSData;
		return sizeof(PCSData)+nBasicDataLen;
	}

	m_pcs_data = new PCSData;
	return 0;
}

