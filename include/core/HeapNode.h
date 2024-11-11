#pragma once
#include <unistd.h>

template<typename T> class CHeapNode
{
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

