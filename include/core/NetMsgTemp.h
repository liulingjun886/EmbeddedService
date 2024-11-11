#pragma once

#include "UserTypeDefines.h"

template <typename T> struct NetMsgTemp
{
    NetMsgTemp()
    {
        //m_cMagic = 0x55;
        m_nLen = sizeof(T);
    }

    void SetCommand(UINT16 u16Main, UINT16 u16Sub)
    {
        m_u16Main = u16Main;
        m_u16Sub = u16Sub;
    }

    void SetLen(UINT32 nLen) // 针对非定长数据
    {
         m_nLen = nLen;
    }

    T* GetData()
    {
        return &m_szData;
    }

    UINT32 m_u32MsgId;
    UINT16 m_u16Main;
    UINT16 m_u16Sub;
    UINT32 m_nLen;
    T m_szData;
};

template <typename T> struct NetMsgSerToSer
{
	NetMsgSerToSer():m_u16Dst(0)
	{
		
	}
	
	void SetDst(UINT16 u16Dst)
	{
		m_u16Dst = u16Dst;
	}

	NetMsgTemp<T>* GetData()
	{
		return &m_tag_data;
	}

	T* GetOriData()
	{
		return m_tag_data.GetData();
	}
	
	UINT16 m_u16Dst;
	NetMsgTemp<T> m_tag_data;
};