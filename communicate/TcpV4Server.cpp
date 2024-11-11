#include "TcpV4Server.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "TcpSocketItem.h"
#include "core/Reactor.h"


CTcpV4Server::CTcpV4Server()
{
	m_str_ip = "";
	m_u16_port = 0;
}

CTcpV4Server::~CTcpV4Server()
{
	
}

int CTcpV4Server::Init()
{
	m_n_poll_id = ::socket(AF_INET, SOCK_STREAM, 0);
	if(0 >= m_n_poll_id)
		return -1;
	int one = 1;
	setsockopt(m_n_poll_id, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
	sockaddr_in addrServer = { 0 };
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(m_u16_port);
	int ret = inet_pton(AF_INET, m_str_ip.c_str(), &addrServer.sin_addr);
	if(ret <= 0)
	{
		return -1;
	}
	
	ret = ::bind(m_n_poll_id,(sockaddr*)&addrServer,sizeof(sockaddr_in));
	if (ret != 0)
	{
		return -1;
	}
	
	ret = listen(m_n_poll_id, SOMAXCONN);
	if (ret != 0)
	{
		return -1;
	}
	
	EnableInput();
	return 0;
}

int CTcpV4Server::InPutNotify()
{
	sockaddr_in clientIP = { 0 };
	static socklen_t ilen = sizeof clientIP;
	
	do
	{
		int connfd = ::accept(m_n_poll_id, (sockaddr*)&clientIP, &ilen);
		if (connfd < 0)
		{
			break;
		}
		
		if(!m_p_create_fun)
		{
			close(connfd);
			break;
		}
		
		CTcpSocketItem *pSocketItem = m_p_create_fun();
		if (NULL == pSocketItem)
		{
			close(connfd);
			break;
		}

		char szIp[16] = {0};
		const char* pStrRet = inet_ntop(AF_INET,&(clientIP.sin_addr),szIp,sizeof(szIp));
		pSocketItem->attach(connfd);
		pSocketItem->SetRemoteAddr(szIp, ntohs(clientIP.sin_port));
	}while(0);
	return 0;
}

