#include "BCMUDevice.h"

BCMUDevice::BCMUDevice():m_pBcmu(nullptr)
{
	
}


BCMUDevice::~BCMUDevice()
{
	if(m_bFreeDevdData)
	{
		delete m_pBcmu;
		m_pBcmu = nullptr;
	}
}

const BCMUData* BCMUDevice::GetData()
{
	return m_pBcmu;
}


int BCMUDevice::SetDataPtr(char* ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	if(ptr)
	{
		m_pBcmu = new (ptr + nBasicDataLen) BCMUData;
		return sizeof(BCMUData)+nBasicDataLen;
	}

	m_pBcmu = new BCMUData;
	return 0;
}

int BCMUDevice::GetDataLen()
{
	return sizeof(BCMUData)+Device::GetDataLen();
}

void BCMUDevice::SetBcmuNo(int nIndex)
{
	m_n_bcmu_no = nIndex;
}

int  BCMUDevice::GetBcmuNo() const
{
	return m_n_bcmu_no;
}

