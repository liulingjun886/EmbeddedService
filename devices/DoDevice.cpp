#include "DoDevice.h"
#include "Gpio.h"

DoDevice::DoDevice():m_n_pin(-1),m_n_value(-1)
{
	
}

DoDevice::~DoDevice()
{
	
}

int DoDevice::InitDo(int nPinValue,int nValue)
{
	m_n_pin = nPinValue;
	GPIO::Export(m_n_pin);
	if(GPIO::Direction(nPinValue, OUT))
		return -1;
	return 0;
}

int DoDevice::SetDoValue(int value)
{
	if(value == m_n_value)
		return 0;
	
	SendAsyncAction(value);
	return 0;
}

void DoDevice::DoAsyncAction(INT32 cmd, INT32 param, ASyncCallDataInst & pReq)
{
	GPIO::Write(m_n_pin, (int)cmd);
}