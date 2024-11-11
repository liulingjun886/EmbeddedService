#include "BMUDevice.h"

BMUDevice::BMUDevice():m_n_pack_no(0),m_n_volt_cnt(0),m_n_temp_cnt(0),
m_p_volt_data(nullptr),m_p_temp_data(nullptr)
{
}

BMUDevice::~BMUDevice()
{
	if(m_bFreeDevdData)
	{
		delete[] m_p_volt_data;
		delete[] m_p_temp_data;
	}
}

int BMUDevice::Initialize(int cmd, ASyncCallDataInst& initdata)
{
	return 0;
}

int  BMUDevice::GetPackNo() const
{
	return m_n_pack_no;
}

void BMUDevice::SetPackNo(int nIndex)
{
	m_n_pack_no = nIndex;
}

void BMUDevice::SetVoltTempNum(int voltNum, int tempNum)
{
	m_n_volt_cnt = voltNum;
	m_n_temp_cnt = tempNum;
}

int  BMUDevice::GetVoltNum()
{
	return m_n_volt_cnt;
}

int  BMUDevice::GetTempNum()
{
	return m_n_temp_cnt;
}

int BMUDevice::SetDataPtr(char* ptr)
{
	int nBasicDataLen = Device::SetDataPtr(ptr);
	if(ptr)
	{
		m_p_volt_data = (UINT16*)(ptr + nBasicDataLen);
		m_p_temp_data = (INT16*)(ptr + nBasicDataLen + (m_n_volt_cnt*sizeof(UINT16)));
		return nBasicDataLen + m_n_volt_cnt*sizeof(UINT16) + m_n_temp_cnt*sizeof(INT16);
	}
	m_p_volt_data = new UINT16[m_n_volt_cnt];
	m_p_temp_data = new INT16[m_n_temp_cnt];
	return 0;
}

UINT16* BMUDevice::GetVoltDataPtr()
{
	return m_p_volt_data;
}

INT16*  BMUDevice::GetTempDataPtr()
{
	return m_p_temp_data;
}

void BMUDevice::SetData(UINT16* pVoltData, INT16* pTempData)
{
	memcpy(m_p_volt_data,pVoltData,m_n_volt_cnt*2);
	memcpy(m_p_temp_data,pTempData,m_n_temp_cnt*2);
}

