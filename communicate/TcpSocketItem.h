#pragma once

#include "core/PollerObject.h"
#include "core/TimeNode.h"
#include "core/Reactor.h"
#include <string>

class CBuffCache
{
public:
    CBuffCache();
    ~CBuffCache();

    char* data();
    UINT32 data_len();
    void append(const char* data, UINT32 data_len);
    void skip(UINT32 length);
	void clear();
	void fix_block_size(UINT32 data_len);
private:
	//CLockMemPool& _mp;
    //内存基址
    char* _mem;
    //内存大小
    UINT32 _block_size;
    //实际使用内存起始偏移量
    UINT32 _data_head;
    //实际使用内存长度
    UINT32 _data_len;
};

class CTcpSocketItem : public CPollerObject //适用于V4 V6 的服务端套接字
{
public:
	CTcpSocketItem():m_b_reconnect(false),m_b_connected(false)
	{
		
	}
	~CTcpSocketItem()
	{
		Close();
	}
public:
	virtual int Init()
	{
		m_b_reconnect = true;
		return 0;
	}
	
	virtual int InPutNotify();
	virtual int OutPutNotify();
	virtual int HunpUpNotiry()
	{
		return onDisConnected(); 
	}
	
	virtual int ConnectToHost()
	{
		EnableOutput();
		return 0;
	}

	virtual void onConnected()
	{
		m_b_connected = true;
		DisableOutPut();
		EnableInput();
	}

	virtual int onDisConnected()
	{
		delete this;
		return -1;
	}

	virtual int onRecvCompelete(char* pData, UINT32 nLen)
	{
		return nLen;
	}

	virtual int SendData(const char* pData, UINT32 nLen);
public:
	int  RecvNetData(char* pBuff,UINT32 nBuffSize);
	void Close();	
	void attach(int fd);
	const std::string& GetRemoteIp() const {return m_str_ip;}
	UINT16 GetRemotePort() const {return m_n_port;}
	void SetRemoteIpStr(const std::string& ip) {m_str_ip = ip;}
	void SetRemotePort(UINT16 port) {m_n_port = port;}
	void SetRemoteAddr(const std::string& ip, UINT16 port);
	
protected:
	bool m_b_reconnect;
	bool m_b_connected;
	std::string m_str_ip;
	UINT16 m_n_port;
	CBuffCache m_recv_buff;
	CBuffCache m_send_buff;
};