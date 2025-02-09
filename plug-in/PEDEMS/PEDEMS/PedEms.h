#pragma once
#include "EMSDevice.h"

class CTcpV4Server;
class RunCfgSockItem;


class PedEms : public EMSDevice
{
private:
	PedEms();
	~PedEms();

public:
	static PedEms* GetInstance();
	
public:
	virtual int InitEMS();
	virtual int StartEMS();
	virtual int Initialize(int cmd, ASyncCallDataInst & initdata);

public:
	int HandRunCfgMsg(RunCfgSockItem* const pSock, int type, std::string& obj);

private:
	int InitEMSDevice();			//初始化自身
	int	InitRunCfgSrv();			//初始化上位机接口
	int BindMeterDevice();			//绑定电表设备
	
private:
	CTcpV4Server* m_pSrv;
};
