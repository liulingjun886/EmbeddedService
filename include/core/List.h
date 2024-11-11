#pragma once

template <typename T> class CListFIFO;
template <typename T> class CListFILO;

template<typename T> class CListNode
{
	friend class CListFIFO<T>;
	friend class CListFILO<T>;
public:
	CListNode() :m_pNext(nullptr)
	{

	}

	~CListNode()
	{

	}

private:
	void setNext(CListNode<T>* pNext)
	{
		m_pNext = pNext;
	}

	CListNode<T>* getNext() const
	{
		return m_pNext;
	}

private:
	CListNode<T>* m_pNext;
};

template <typename T> class CListFILO
{
public:
	CListFILO():m_n_nodenum(0)
	{

	}

	~CListFILO()
	{
	}

	void Push(CListNode<T>* pNode)
	{
		pNode->setNext(m_p_header.getNext());
		m_p_header.setNext(pNode);
		++m_n_nodenum;
	}

	void Push(CListNode<T>& pNode)
	{
		//pNode.setNext(m_p_header.getNext());
		//m_p_header.setNext(&pNode);
		Push(&pNode);
	}

	T* Pop()
	{
		return static_cast<T*>(GetHeader());
	}

private:
	CListNode<T>* GetHeader()
	{
		if (nullptr == m_p_header.getNext())
			return nullptr;
		CListNode<T>* pNode = m_p_header.getNext();
		m_p_header.setNext(pNode->getNext());
		--m_n_nodenum;
		return pNode;
	}
private:
	CListNode<T> m_p_header;
	int m_n_nodenum;
};

template <typename T> class CListFIFO
{
public:
	CListFIFO() :pTail(&m_p_header),m_n_nodenum(0)
	{

	}

	~CListFIFO()
	{

	}

	void Push(CListNode<T>* pNode)
	{
		pNode->setNext(nullptr);
		pTail->setNext(pNode);
		pTail = pNode;
		++m_n_nodenum;
	}

	void Push(CListNode<T>& pNode)
	{
		//pNode.setNext(nullptr);
		//pTail->setNext(&pNode);
		//pTail = &pNode;
		Push(&pNode);
	}

	T* Pop()
	{
		return static_cast<T*>(GetHeader());
	}

	bool IsEmpty()
	{
		return (&m_p_header == pTail);
	}
private:
	CListNode<T>* GetHeader()
	{
		if (&m_p_header == pTail)
			return nullptr;
		CListNode<T>* pNode = m_p_header.getNext();
		if (pNode == pTail)
			pTail = &m_p_header;
		m_p_header.setNext(pNode->getNext());
		--m_n_nodenum;
		return pNode;
	}
protected:
	CListNode<T> m_p_header;
	CListNode<T>* pTail;
	int m_n_nodenum;
};
