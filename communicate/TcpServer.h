#pragma once

#include "core/PollerObject.h"
#include <string>

class CTcpSocketItem;

typedef CTcpSocketItem* (*CreateSocketItem)();

template<typename T> CTcpSocketItem* CreateSocket()
{
	return new T;
}

class CTcpServer : public CPollerObject
{
public:
	CTcpServer();
	~CTcpServer();
public:
	virtual int OutPutNotify() {return 0;}
	virtual int HunpUpNotiry() {return 0;}
public:
	int setAddrInfo(const std::string& strIP, UINT16 port);
	void SetCreateFun(CreateSocketItem fun);
	const std::string& GetSerIp() const;
	UINT16 GetPort() const;
protected:
	std::string m_str_ip;
	UINT16 m_u16_port;
	CreateSocketItem m_p_create_fun;
};