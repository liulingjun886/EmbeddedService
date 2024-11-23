#pragma once
#include "Device.h"

// 储能柜设备
class ESSDevice : public Device
{
public:
	int Initialize(int cmd, ASyncCallDataInst& initdata)
	{
		return 0;
	}
};

