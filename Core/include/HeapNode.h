#pragma once
#include <unistd.h>

template<typename T> class CMinHeap;

template<typename T> class CHeapNode
{
	friend class CMinHeap<T>;
	T* left;
	T* right;
	T* parent;
private:
	void Reset()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
	}
public:
	CHeapNode() :left(NULL), right(NULL), parent(NULL)
	{

	}
};

