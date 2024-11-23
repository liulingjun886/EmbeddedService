#pragma once
#include "BMUDevice.h"
#include "Log.h"


class TestBmu : public BMUDevice
{
public:
	virtual int Initialize(int cmd, ASyncCallDataInst & initdata)
	{
		return 0;
	}
};