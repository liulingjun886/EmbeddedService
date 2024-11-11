#pragma once

#include "core/ASyncCallSupport.h"

class DoDevice : public CAsyncCallSupport
{
public:
	DoDevice();
	~DoDevice();
public:
	int InitDo(int pin,int value=0);
	int SetDoValue(int value);

public:
	void DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst & pReq);
	
private:
	int m_n_pin;
	int m_n_value;
};