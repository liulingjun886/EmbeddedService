#pragma once

#include <unistd.h>
#include <iostream>

template<typename T> class CMinHeap 
{
public:
	CMinHeap():min(NULL),nelts(0){}
	~CMinHeap(){}
public:
	T* GetMinNode()
	{
		return min;
	}

	void Insert(T* newnode)
	{
		newnode->Reset();
		T** parent;
		T** child;
		unsigned int path;
		unsigned int n;
		unsigned int k;

		/* Calculate the path from the root to the insertion point.  This is a min
		 * heap so we always insert at the left-most free node of the bottom row.
		 */
		path = 0;
		for (k = 0, n = 1 + nelts; n >= 2; k += 1, n /= 2)
			path = (path << 1) | (n & 1);

		/* Now traverse the heap using the path we calculated in the previous step. */
		parent = child = &min;
		while (k > 0) {
			parent = child;
			if (path & 1)
				child = &(*child)->right;
			else
				child = &(*child)->left;
			path >>= 1;
			k -= 1;
		}

		/* Insert the new node. */
		newnode->parent = *parent;
		*child = newnode;

		nelts += 1;
		//newnode->active = true;

		/* Walk up the tree and check at each node if the heap property holds.
		 * It's a min heap so parent < child must be true.
		 */
		while (newnode->parent != NULL && ((*newnode) < *(newnode->parent)))
			heap_node_swap(newnode->parent, newnode);
	}

	void Remove(T* node)
	{
		T* smallest;
		T** max;
		T* child;
		unsigned int path;
		unsigned int k;
		unsigned int n;

		if (0 == nelts)
			return;

		/* Calculate the path from the min (the root) to the max, the left-most node
		 * of the bottom row.
		 */
		path = 0;
		for (k = 0, n = nelts; n >= 2; k += 1, n /= 2)
			path = (path << 1) | (n & 1);

		int i = 0;
		/* Now traverse the heap using the path we calculated in the previous step. */
		max = &min;
		while (k > 0) {
			if (path & 1)
				max = &(*max)->right;
			else
				max = &(*max)->left;
			path >>= 1;
			k -= 1;
		}

		//node->active = false;
		nelts -= 1;
		
		/* Unlink the max node. */
		child = *max;
		*max = NULL;

		if (child == node) {
			/* We're removing either the max or the last node in the tree. */
			if (child == min) {
				min = NULL;
			}
			return;
		}

		/* Replace the to be deleted node with the max node. */
		child->left = node->left;
		child->right = node->right;
		child->parent = node->parent;

		if (child->left != NULL) {
			child->left->parent = child;
		}

		if (child->right != NULL) {
			child->right->parent = child;
		}

		if (node->parent == NULL) {
			min = child;
		}
		else if (node->parent->left == node) {
			node->parent->left = child;
		}
		else {
			node->parent->right = child;
		}

		/* Walk down the subtree and check at each node if the heap property holds.
		 * It's a min heap so parent < child must be true.  If the parent is bigger,
		 * swap it with the smallest child.
		 */
		for (;;) {
			smallest = child;
			if (child->left != NULL && (*(child->left) < *(smallest)))
				smallest = child->left;
			if (child->right != NULL && (*(child->right) < (*smallest)))
				smallest = child->right;
			if (smallest == child)
				break;
			heap_node_swap(child, smallest);
		}

		/* Walk up the subtree and check that each parent is less than the node
		 * this is required, because `max` node is not guaranteed to be the
		 * actual maximum in tree
		 */
		while (child->parent != NULL && (*(child) < *(child->parent)))
			heap_node_swap(child->parent, child);
	}

	void Dequeue()
	{
		Remove(min);
	}

	unsigned GetNelts()
	{
		return nelts;
	}
private:
	void heap_node_swap(T* parent,T* child) 
	{
		T* sibling;
		SwapNode(parent, child);
		parent->parent = child;
		if (child->left == child) {
			child->left = parent;
			sibling = child->right;
		}
		else {
			child->right = parent;
			sibling = child->left;
		}
		if (sibling != NULL)
			sibling->parent = child;

		if (parent->left != NULL)
			parent->left->parent = parent;
		if (parent->right != NULL)
			parent->right->parent = parent;

		if (child->parent == NULL)
			min = child;
		else if (child->parent->left == parent)
			child->parent->left = child;
		else
			child->parent->right = child;
	}

	inline void SwapNode(T* pNode1, T* pNode2)
	{
		T* pTemp;
		pTemp = pNode1->left;
		pNode1->left = pNode2->left;
		pNode2->left = pTemp;

		pTemp = pNode1->right;
		pNode1->right = pNode2->right;
		pNode2->right = pTemp;

		pTemp = pNode1->parent;
		pNode1->parent = pNode2->parent;
		pNode2->parent = pTemp;
	}
private:
	T* min;
	unsigned int nelts;
};


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