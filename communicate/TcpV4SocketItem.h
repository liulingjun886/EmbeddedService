#pragma once
#include "TcpSocketItem.h"

class CTcpV4SocketItem : public CTcpSocketItem
{
public:
	virtual int ConnectToHost()
	{
		Close();
		m_n_poll_id = socket(AF_INT4,SOCK_STREAM,0);

		if(-1 == m_n_poll_id)
			return -1;
		
		sockaddr_in servaddr = {0};
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(m_n_port);
		int n = inet_pton(AF_INET,m_str_ip.c_str(),&servaddr.sin_addr);
		if(n <= 0)
			return -1;
		connect(m_n_poll_id, (sockaddr*)&servaddr, sizeof(servaddr));
		return CTcpSocketItem::ConnectToHost();
	}
};