/*
	C++ Implementation for Max Heap 
	version 1.0.0
	https://github.com/xlong88/maxHeap
*/

#ifndef MAX_HEAP_HPP
#define MAX_HEAP_HPP

#include <iostream>
#include <vector>
#include <functional> // std::less
#include <assert.h>     /* assert */
#include <stdexcept> // underflow overflow

//! The MaxHeap class
/*!
 	Generic max binary heap using an array.
	Can be used with an customized comparator instead of the natural order,
	but the generic Value type must still be comparable.
*/
template <class T,class Compare=std::less<T> >
class maxHeap{
	//! capacity of the heap
	int capacity;
	//! # of elements
	int size;
	//! elements
	T *h;
	//! comparison function
	/*! The comparison function can be user-defined, if unspecified, std::less<T> would be used. 
	*/
	Compare Less; 
	//! A function to build a maxHeap from an unordered array
	void build_max_heap()
	{
		for (int i = (size - 1) / 2 + 1;i >= 1;-- i)
			max_heapify(i);
	}
	//! A function to make the sub-tree rooted at node i a maxHeap, if the sub-trees rooted at all its descendants are already maxHeap 
	void max_heapify(int i)
	{
		int j;
		while (2 * i <= size)
		{
			j = i;
			if (h[j] < h[2 * i]) j = 2 * i;
			if (2 * i + 1 <= size && h[j] < h[2 * i + 1]) j = 2 * i + 1;
			if (j == i) break;
			swap(i,j);
			i = j;
		}
	}
	//! A function to exchange h[i] and h[j]
	inline void swap(int i,int j)
	{
		T tmp = h[i];
		h[i] = h[j];
		h[j] = tmp;
	}
	//! A function to check whether the array satisfies the maxHeap property or not
	bool is_max_heap()
	{
		if (empty()) return true;
		for (int i = 0;2 * i <= size;++ i)
			{
				if (h[2 * i] > h[i]) return false;
				if (2 * i + 1 <= size && h[2 * i + 1] > h[i]) return false;
			}
		return true;
	}
	//! A function to make the maxHeap still satisfy the maxHeap property when inserting a node at i.
	void swim(int i)
	{
		int parent;
		while (i > 1)
		{
			parent = i / 2;
			if (h[parent] < h[i]) 
				swap(parent,i);
			else
				break;
			i = parent;
		}

	}
	//! The same as max_heapify()
	void sink(int i)
	{
       max_heapify(i);
	}

public:
	//! A constructor
	explicit maxHeap(Compare uLess = Compare())// constructor
	{
		capacity = 100;
		size = 0;
		h = new T[capacity];
		Less = uLess;
	}
	//! A constructor with capacity specified
	explicit maxHeap(int capacity_,Compare uLess = Compare())// constructor
	{
		capacity = capacity_ + 1;
		size = 0;
		h = new T[capacity];
		Less = uLess;
	}
	//! A constructor
	explicit maxHeap(T *data,int len,Compare uLess = Compare())// constructor
	{
		std::vector<T> vecData(data,data + len);
		maxHeap(vecData,uLess);
	}
	//! A constructor
	explicit maxHeap(std::vector<T>& data, Compare uLess = Compare())// constructor
	{
		size = data.size();
		capacity = size + 1;
		h = new T[capacity];
		for (int i = 1;i <= size;++ i)
			h[i] = data[i - 1];

		Less = uLess;
		build_max_heap();
		assert(is_max_heap());
	}
	//! A function to print out the elements in the heap
	void print()
	{
		for (int i = 1;i <= size;++ i)
		{
	        std::cout << h[i] << " ";
		}
		std::cout << std::endl;
	}
	//! A function to insert a value in the heap
	void insert(T x)
	{
		if (size >= capacity - 1) resize(2 * capacity);
		++ size;
		h[size] = x;
        swim(size);
        assert(is_max_heap());
	}
	//! A function to answer the query of the maximum element in the heap
	T max()
	{
        if (empty())
        {
        	throw new std::underflow_error("MaxHeap underflow.");
        }
        return h[1];
	}
    //! A function to answer the query of the maximum element in the heap amd then remove the maximum element 
	T extract_max()
	{
		T maxElem = max();
        
		if (size > 1)
		{
			swap(1,size);
			-- size;
			sink(1);
		}
		else
		{
		    -- size;
		}

		assert(is_max_heap());
		if (size > 0 && size == (capacity - 1) / 4) resize(capacity / 2);

		return maxElem;
	}
    //! A function to track whether the heap is empty
	bool empty()
	{
		return (size == 0);
	}
    //! A function to change the capacity of the heap
	void resize(int capacity_)
	{
		assert(capacity_ >= size + 1);
        capacity = capacity_;
        T *tmp = new T[capacity];
        for (int i = 1;i <= size;++ i)
        	tmp[i] = h[i];
        h = tmp;
	}
};



#endif



