#pragma once

#include "TcpServer.h"

class CTcpV4Server : public CTcpServer
{
public:
	CTcpV4Server();
	~CTcpV4Server();
public:
	int Init();
	virtual int InPutNotify();
};