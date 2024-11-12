#pragma once
#include <vector>
#include <string.h>
#include <assert.h>
#include "UserTypeDefines.h"
#include "ToolLock.h"

class ShareCount
{
	public:
		ShareCount():m_ref(0)
		{
			
		}
		virtual ~ShareCount(){}
	public:
		void addref()
		{
			spin.Lock();
			++m_ref;
			spin.UnLock();
		}
		void decref()
		{
			int refCnt = 0;
			spin.Lock();
			refCnt = --m_ref;
			spin.UnLock();
			assert(0 <= refCnt);
			if(0 == refCnt)
				delete this;
		}
	private:
		int m_ref;
		CSpinLock spin;
}; 

/*class ShareData : public ShareCount
{
public:
	virtual UINT32 length() = 0;
	virtual char* data() = 0;
	virtual void fill(char c, UINT32 length) = 0;
};

class ASyncCallData : public ShareCount
{
public:
	ASyncCallData(ShareData* m_pData);
	~ASyncCallData();
private:
	ShareData* m_pData;
};*/

class ASyncCallDataInst;

class ASyncCallData : public ShareCount
{
	friend class ASyncCallDataInst;
private:
	ASyncCallData()
	{
		
	}
	
	virtual ~ASyncCallData()
	{
		
	}
private:
	UINT32 length()
	{
		return m_vec_data.size();
	}

	char* data()
	{
		return m_vec_data.data();
	}

	void fill(char c, UINT32 length)
	{
		m_vec_data.resize(length);
		memset(m_vec_data.data(),c,length);
	}
private:
	std::vector<char> m_vec_data;
};


class ASyncCallDataInst
{
public:
	static ASyncCallDataInst& GetInstance()
	{
		static ASyncCallDataInst data;
		return data;
	}
public:
	ASyncCallDataInst():m_pData(nullptr)
	{
		m_pData = new ASyncCallData;
		m_pData->addref();
	}

	ASyncCallDataInst(ASyncCallDataInst& other)
	{
		m_pData->decref();
		other.m_pData->addref();
		m_pData = other.m_pData;
	}
	
	ASyncCallDataInst& operator=(ASyncCallDataInst& other)
	{
		m_pData->decref();
		other.m_pData->addref();
		m_pData = other.m_pData;
		return *this;
	}
	
	~ASyncCallDataInst()
	{
		m_pData->decref();
	}

public:
	UINT32 length()
	{
		return m_pData->length();
	}

	char* data()
	{
		return m_pData->data();
	}

	void fill(char c, UINT32 length)
	{
		m_pData->fill(c, length);
	}
	
private:
	ASyncCallData* m_pData;
};