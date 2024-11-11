#pragma once

#include "List.h"

#define PAGESIZE 4096

class CBuffer : public CListNode<CBuffer>
{
public:
	CBuffer(int idx):m_n_idx(idx),m_n_buff_len(0),m_n_data_len(0)
	{
		
	}
public:
	UINT GetIdx()
	{
		return m_n_idx;
	}
	
	void SetBuffLen(int nLen)
	{
		m_n_buff_len = nLen - sizeof(CBuffer);
	}
	
	void ResetDataLen()
	{
		m_n_data_len = 0;	
	}
	
	char* GetAppendAddr()
	{
		return &m_array_data[m_n_data_len];
	}

	UINT32 GetAvailableLen() const
	{
		return m_n_buff_len - m_n_data_len;
	}

	char* GetData() const
	{
		return &m_array_data[0]
	}

	UINT32 GetDataLen() const
	{
		return m_n_data_len;
	}
private:
	const UINT32 m_n_idx;
	UINT32 m_n_buff_len;
	UINT32 m_n_data_len;
	char m_array_data[0];
};

class CBufferPool
{
public:
	CBufferPoll();
	~CBufferPoll();
public:
	CBuffer* GetBuffer(int nId)
	{
		CListFIFO<CBuffer>& listobj = m_buff_list[nId];
		CBuffer* pBuff = listobj.Pop();
		if(!pBuff && GenNewBuffer(nId))
		{
			return nullptr
		}
		pBuff->ResetDataLen();
		return pBuff
	}

	void ReturnBuffer(CBuffer* pBuff)
	{
		AddBuffer(pBuff->GetIdx(),pBuff);
	}
private:
	int GenNewBuffer(int idx)
	{
		if(idx > 10)
			return -1;
		return GenIdxBuff(idx, 1 << (10 + idx));
	}

	int GenIdxBuff(int idx, int nBufferSize)
	{
		int nPageNum = 1,nBuffNum = 1;
		if(nBufferSize > PAGESIZE)
			nPageNum = nBufferSize / PAGESIZE;
		else
			nBuffNum = PAGESIZE / nBufferSize
		
		char* ptr = (char*)mmap(NULL, nPageNum * PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS /*MAP_HUGETLB*/, -1, 0);
		if(!ptr)
			return -1;
		
		for(int i = 0; i < nBuffNum; ++i)
		{
			CBuffer* pBuff = new (ptr) CBuffer(idx);
			pBuff->SetBuffLen(nBufferSize);
			AddBuffer(idx,pBuff);
			ptr += nBufferSize;
		}
		return 0;
	}

	void AddBuffer(int idx, CBuffer* pBuff)
	{
	//	pBuff->pNext = m_buff_list[idx].pNext;
	//	m_buff_list[idx].pNext = pBuff;
		m_buff_list[idx].Push(pBuff);
	}

	void ReleaseBuffer(CBuffer* pBuff)
	{
		munmap(pBuff, 1 << (10 + pBuff->GetIdx()));
	}
private:
	CListFIFO<CBuffer> m_buff_list[10];
};

