#pragma once
#include "TcpSocketItem.h"

class SysCfgSockItem : public CTcpSocketItem
{
public:
	int onRecvCompelete(char* pData, UINT32 nLen);
};