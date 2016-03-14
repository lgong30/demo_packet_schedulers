#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include "maxHeap.hpp"

template <class TYPE,class Compare = std::less<TYPE> >
class PriorityQueue: public maxHeap<TYPE,Compare> {
public:
	PriorityQueue(Compare uLess = Compare())
	{
		maxHeap(uLess);
	}
	PriorityQueue(int capacity_,Compare uLess = Compare())
	{
		maxHeap(capacity_,uLess);
	}
	void Enqueue(TYPE t)
	{
		insert(t);
	}
	TYPE PeekMin()
	{
		return max();
	}
	void PopMin()
	{
		extract_max();
	}
};


#endif