#pragma once

#include "ShareData.h"

class CloudHandSink
{
public:
	virtual void OnConnSucess();
	virtual void OnConnFailer();
	virtual void OnConnLost();
	virtual void OnMsgArrive();
	virtual int  SendMsg(ASyncCallDataInst& data = ASyncCallDataInst::GetInstance());
};