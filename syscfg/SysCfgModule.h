#pragma once
#include <string>
#include "FileTransmit.h"

class CTcpV4Server;

class SysCfgSockItem;

class SysCfgModule
{
public:
	SysCfgModule();
	~SysCfgModule();
	static SysCfgModule* GetInstance();
	
public:
	int Init();
	int HandSysCfgMsg(SysCfgSockItem* const pSock, int type, std::string& obj);
	
private:
	CTcpV4Server* m_pSrv;
	FileTransmite m_file_transmit;
};