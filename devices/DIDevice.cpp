#include "DIDevice.h"
#include "Gpio.h"

#define TIMER_UPDATE 1

DiDevice::DiDevice():m_n_value(-1),m_n_pin(-1),m_n_update_time(200)
{
	m_timer_update.InitTimerNode(this, TIMER_UPDATE);
}

DiDevice::~DiDevice()
{
	
}

int DiDevice::InitDi(int nPinValue, int msec)
{
	m_n_update_time = msec;
	m_n_pin = nPinValue;
	if(GPIO::Export(m_n_pin))
		return -1;
	if(GPIO::Direction(nPinValue, IN))
		return -1;
	return 0;
}

int DiDevice::GetValue() const
{
	return m_n_value;
}

void DiDevice::Start()
{
	m_timer_update.StartTimer(m_n_update_time, -1);
}

void DiDevice::DoAsyncAction(INT32 cmd,  INT32 param, ASyncCallDataInst& pReq)
{
	int value = GPIO::Read(m_n_pin);
	SendSyncAction(cmd, value);
}

void DiDevice::DoSyncAction(INT32 cmd, INT32 param, ASyncCallDataInst& pResp)
{
	if(m_n_last_value == param)
	{
		m_n_value = param;
	}
	m_n_last_value = param;
}

int DiDevice::ProcessTimeOutEvent(UINT32 nTimeId)
{
	SendAsyncAction(TIMER_UPDATE);
	return 0;
}
