#pragma once
#include "Device.h"

class ESSDevice : public Device
{
public:
	int Initialize(int cmd, ASyncCallDataInst& initdata)
	{
		return 0;
	}
};