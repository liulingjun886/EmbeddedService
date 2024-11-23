#include "TcpSocketItem.h"
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <string.h>


CBuffCache::CBuffCache():_mem(NULL), _block_size(0), _data_head(0), _data_len(0)
{
	
}

CBuffCache::~CBuffCache()
{
	clear();
}

char* CBuffCache::data()
{
    if (_data_len == 0)
        return NULL;

    assert(_data_head < _block_size);
    return _mem + _data_head;
}

UINT32 CBuffCache::data_len()
{
	return _data_len;
}

void CBuffCache::append(const char* data, UINT32 data_len)
{
    if (0 == _block_size)
    {
    	fix_block_size(data_len);
		_mem = (char*)malloc(_block_size);
        if(!_mem)
            throw(0);

        memcpy(_mem, data, data_len);

        _data_head = 0;
        _data_len = data_len;
        return;
    }
   
    if (data_len + _data_head + _data_len <= _block_size)
    {
        memcpy(_mem + _data_head + _data_len, data, data_len);
        _data_len += data_len;
    }
    else if (data_len + _data_len > _block_size)
    {
		fix_block_size(data_len);
		char* mem = (char*)malloc(_block_size);
        if(!mem)
        	throw(0);

        memcpy(mem, _mem + _data_head, _data_len);
        memcpy(mem + _data_len, data, data_len);
		free(_mem);
        _mem = mem;
        _data_head = 0;
        _data_len += data_len;
    }
    else
    {
        memmove(_mem, _mem+_data_head, _data_len);
        memcpy(_mem+_data_len, data, data_len);

        _data_head = 0;
        _data_len += data_len;
    }
}

void CBuffCache::skip(UINT32 length)
{
    if (_mem == NULL)
        return;
    else if (length >= _data_len)
    {
    	if(_block_size > (1 << 10))
    	{
    		clear();
    	}
       	else
		{
			_data_head = 0;
			_data_len = 0;
		}
    }
    else
    {
        _data_head += length;
        _data_len -= length;
    }
}

void CBuffCache::clear()
{
	if (_mem == NULL)
        return;
	
	free(_mem);
    _mem = NULL;
	_data_head = 0;
    _data_len = 0;
    _block_size = 0;
}

void CBuffCache::fix_block_size(UINT32 data_len)
{
	if(0 == _block_size)
		_block_size = 1 << 10;
	
	while(_block_size < data_len)
	{
		_block_size = _block_size << 1;
	}

	_block_size = 1024;//ClassSize::Roundup(_block_size);
}

#define MAX_RECV_BUFF 65536

int CTcpSocketItem::InPutNotify()
{
	//std::cout << "InPutNotify" << std::endl;
	int iRecv = 0,nRet = 0;
	char szBuff[MAX_RECV_BUFF] = {0};
	do
	{
		iRecv = RecvNetData(szBuff,MAX_RECV_BUFF);
		if(iRecv > 0)
			m_recv_buff.append(szBuff, iRecv);
		else
			return iRecv;
		do
		{
			nRet = onRecvCompelete(m_recv_buff.data(), m_recv_buff.data_len());
			if(nRet > 0)
				m_recv_buff.skip(nRet);
			else if(0 == nRet)
				break;
			else
				return -1;
		}while(m_recv_buff.data_len() > 0);

	} while (MAX_RECV_BUFF == iRecv );
	
	return 0;
}

int CTcpSocketItem::OutPutNotify()
{
	if(!m_b_connected)
	{
		onConnected();
	}
	
	if(m_send_buff.data_len() > 0)
	{
		int ret = send(m_n_poll_id,m_send_buff.data(),m_send_buff.data_len(),0);
		if(ret > 0)
		{
			m_send_buff.skip(ret);
			if(m_send_buff.data_len() <= 0)
				DisableOutPut();
		}
		else
		{
			HunpUpNotiry();
		}
	}
	return 0;
}

int CTcpSocketItem::SendData(const char* pData, UINT32 nLen)
{
	m_send_buff.append(pData, nLen);
	int ret = send(m_n_poll_id,m_send_buff.data(),m_send_buff.data_len(),0);
	if(ret > 0)
	{
		m_send_buff.skip(ret);
		if(m_send_buff.data_len() > 0)
			EnableOutput();
		else
			DisableOutPut();
	}
	else
	{
		HunpUpNotiry();
	}
}

int  CTcpSocketItem::RecvNetData(char* pBuff, UINT32 nBuffSize)
{
	int nRet = 0;
loop:	
	nRet = recv(m_n_poll_id,pBuff,nBuffSize,0);
	switch (nRet)
	{
		case 0:
		{
			return -1;
		}
		case -1:
		{
			switch (errno)
			{
				case EAGAIN:
				{
					return 0;
				}
				case EINTR:
				{
					goto loop;
				}
				default:
				{
					return -1;
				}
			}
		}
		default:
			return nRet;
	}
}

void CTcpSocketItem::Close()
{
	if(-1 != m_n_poll_id)
		close(m_n_poll_id);
	m_n_poll_id = -1;
}


void CTcpSocketItem::attach(int fd)
{
	m_n_poll_id = fd;
	onConnected();
}


void CTcpSocketItem::SetRemoteAddr(const std::string& ip, UINT16 port)
{
	m_str_ip = ip;
	m_n_port = port;
}

