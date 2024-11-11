#pragma once

#include "core/TimeNode.h"
#include "core/TcpV4Server.h"
#include <set>
#include <string>

struct salveInfo
{
	pid_t pid;
	std::string strIP;
	UINT16 nPort;
	UINT8 nTimeOutCnt;
};

class EMSMonitor : public CTimeEventHander
{
public:
	EMSMonitor();
	~EMSMonitor();
public:
	int Init();
	virtual int ProcessTimeOutEvent(UINT32 nTimeId);

private:
	int CheckSlavesOnline();
	int LoadSlaveNode();
	pid_t StartSlaveNode(std::string& ip, UINT16 port);
private:
	CTimer m_timer_check_online;
	CTcpV4Server* m_p_srv_monitor;
	CTcpV4Server* m_p_srv_conf;
	std::set<salveInfo> m_set_slave_info;
};