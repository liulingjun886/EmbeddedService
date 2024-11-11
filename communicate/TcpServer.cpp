#include "TcpServer.h"

CTcpServer::CTcpServer():m_str_ip(""),m_u16_port(0),m_p_create_fun(NULL)
{
	
}

CTcpServer::~CTcpServer()
{
	
}

int CTcpServer::setAddrInfo(const std::string& strIP, UINT16 port)
{
	m_str_ip = strIP;
	m_u16_port = port;
	return 0;
}

const std::string& CTcpServer::GetSerIp() const
{
	return m_str_ip;
}

UINT16 CTcpServer::GetPort() const
{
	return m_u16_port;
}

void CTcpServer::SetCreateFun(CreateSocketItem fun)
{
	m_p_create_fun = fun;
}



