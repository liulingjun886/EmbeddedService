#pragma once

#include "Device.h"
#include "DataPool.h"

class BMUDevice : public Device
{
public:
	BMUDevice();
	~BMUDevice();
	int Initialize(int cmd, ASyncCallDataInst& initdata);
	int  GetPackNo() const;
	void SetPackNo(int nIndex);

	void SetVoltTempNum(int voltNum, int tempNum);
	int  GetVoltNum();
	int  GetTempNum();
	int SetDataPtr(char* ptr);

	UINT16* GetVoltDataPtr();
	INT16*  GetTempDataPtr();

	void SetData(UINT16* pVoltData, INT16* pTempData);
	
private:
	int m_n_pack_no;	//pack序号
	int m_n_volt_cnt;	//电压计数个数
	int m_n_temp_cnt;	//温度计数个数
	
	UINT16* m_p_volt_data;
	INT16*  m_p_temp_data;
};