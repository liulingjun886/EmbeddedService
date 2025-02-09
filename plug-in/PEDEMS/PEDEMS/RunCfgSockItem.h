#pragma once
#include "TcpSocketItem.h"

class RunCfgSockItem : public CTcpSocketItem
{
public:
	int onRecvCompelete(char* pData, UINT32 nLen);
	int SendData(int nType, char* pData, int nLen);
};

