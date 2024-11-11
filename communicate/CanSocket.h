#pragma once

#include "core/PollerObject.h"
#include "core/ShareData.h"
struct can_frame;

class CanSocket : public CPollerObject
{
public:
	CanSocket();
	void SetCanInfo(int id, int bitrate);
	int SendCanData(can_frame* pData);
public:
	virtual int Init();
	virtual int InPutNotify();
	virtual int OutPutNotify(){return 0;};
	virtual int HunpUpNotiry(){return 0;};
	virtual int OnCanRecvComplete(can_frame* pData) = 0;
private:
	int m_n_canid;
	int m_n_can_bitrate;
	ASyncCallDataInst m_can_recv_data;
};