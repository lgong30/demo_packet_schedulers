/*
	C++ Implementation for binary search tree (BST).
	Shamelessly copy some parts from https://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/code/BST.java
	version 1.0.0
	https://github.com/xlong88/bst

*/

#ifndef BST_HPP
#define BST_HPP

#include <iostream>
#include <vector>
#include <functional> // for less
#include <stdexcept> // for run time error


//! The data structure for each node in the binary search tree
template <class T>
class node{
public:
	//! data field
	T data; 
	//! left child
	node *left;
	//! right child
	node *right;
#ifdef AVL_TREE_HPP
	//! the height of this node in the AVL tree
	int height;
#endif
	//! A constructor
	node(T& data_)
	{
		data = data_;
		left = NULL;
		right = NULL;
#ifdef AVL_TREE_HPP
		height = 0;
#endif
	}
};


//! The class for BST 
/*!
 	Generic binary search tree.
	Can be used with an customized comparator instead of the natural order,
	but the generic Value type must still be comparable.
*/
template <class T,class Compare=std::less<T> >
class BST{
	//! node in BST
	node<T> *root;
	//! comparison function
	Compare Less;
	//! A function to insert an element into the subtree rooted at current
	void insert(node<T> *current,T& data)
	{
        if (Less(current->data,data))
        	{
        		if (current->right == NULL)
        			current->right = new node<T>(data);
        		else
        			insert(current->right,data);
        	}
        else
        {
        	if (current->left == NULL)
        		current->left = new node<T>(data);
        	else
        		insert(current->left,data);
        }
	}
	//! A function to check whether the element toSearch is in the subtree rooted at current or not
	node<T>* contains(node<T> *current,T& toSearch)
	{
		if (current == NULL) return NULL;
		if (current->data == toSearch) return current;
		else if (Less(toSearch,current->data)) return contains(current->left,toSearch);
		else return contains(current->right,toSearch);
	}
	//! A function to remove the element toRemove from the subtree rooted at current
	node<T>* remove(node<T> *current,T& toRemove)
	{
		if (current == NULL) throw new std::runtime_error("The element does not exists!");
        if (current->data == toRemove)
        {
        	if (! current->left) 
        		return current->right;
        	else if (current->right)
        		return current->left;
        	else
        	{
        		current->data = retrievalData(current->left);
        		current->left = remove(current->left,current->data);
        	}
        }
        else if (Less(toRemove,current->data))
        	current->left = remove(current->left,toRemove);
        else
        	current->right = remove(current->right,toRemove);

        return current;
	}
	//! A function to retrieve the next element of the current node
	T retrievalData(node<T> *current)
	{
		if (current == NULL) throw new std::runtime_error("Cannot retrieval data from an empty node");
		while (current->right) current = current->right;
		return current->data;
	}
	//! A recursive function to postorder traverse the subtree rooted at current 
	void explore(node<T> *current,bool saveFlag,std::vector<T>& savedElements)
	{
		if (current == NULL) return;
		explore(current->left,saveFlag,savedElements);
		if (saveFlag) 
			savedElements.push_back(current->data);
		else
			std::cout << current->data << " ";
		explore(current->right,saveFlag,savedElements);
	}
public:
	//! A constructor to create an empty BST
	explicit BST(Compare uLess = Compare())
	{
		root = NULL;
		Less = uLess;
	}
	//! A constructor to create a BST from a vector
	explicit BST(std::vector<T>& nums,Compare uLess = Compare())
	{
		root = NULL;
		Less = uLess;
		for (auto data: nums)
			insert(data);
	}
	//! A function to test whether the BST is empty or not
	bool empty()
	{
		return (root == NULL);
	}
	//! insert data into the BST
	/*!
	Note that, not like in "https://www.cs.cmu.edu/~adamchik/15-121/lectures/Trees/code/BST.java", here it is allowed to insert duplicate element.
	*/
	void insert(T& data)
	{
        if (root == NULL)
        	root = new node<T>(data);
        else
        	insert(root,data);
	}
	//! A function to query whether element toSearch is in the BST or not
	bool search(T& toSearch)
	{
		return contains(toSearch);
	}
	//! the same as search()
	bool contains(T& toSearch)
	{
        return !(contains(root,toSearch) == NULL);
	}
	//! A function to remove the element toRemove
	/*!
	Note that, if there is not such element, it will throw a run time error. And if there are multiple ones, it will only remove one of them.
	*/
	void remove(T& toRemove)
	{
		return remove(root,toRemove);
	}
	//! A function to return the height of the BST, i.e., the length of the longest path from the root to any of the leaves in the BST (# of edges)
	int height()
	{
		return height(root);
	}
	//! A function to return the height of the subtree rooted at current, i.e., the length of the longest path from the current to any of its leaf-descendants in the BST
	/*!
	Note that, the height of any leaf node is 0.
	*/
	int height(node<T> *current)
	{
		if (current == NULL) return -1;
		return std::max(height(current->left),height(current->right)) + 1;
	}
    //! A function to obtain the width of the BST, which is the maximum number of nodes in any level of the BST
	int width()
	{
		int maxWid = 0;
		for (int i = 0;i <= height();++ i)
		{
			int tmp = width(root,i);
			maxWid = std::max(maxWid,tmp);
		}
		return maxWid;
	}
    //! A function to obtain  number of node on a given level
	int width(node<T> *current,int depth)
	{
		if (current == NULL) return 0;
		else if (depth == 0) return 1;
		else return width(current->left,depth - 1) + width(current->right,depth - 1);
	}
    //! A function to obtain the diameter of the BST
    /*!
    The diameter is the length of the longest path between any two leaves in the BST.
    */
	int diameter()
	{
		return diameter(root);
	}
    //! A function to obtain the diameter of the subtree rooted at current
	int diameter(node<T> *current)
	{
		if (current == NULL) return 0;

		int pLen0 = height(current->left) + height(current->right) + 3;

		int pLen1 = std::max(diameter(current->left),diameter(current->right));

		return std::max(pLen0,pLen1);
	}
    //! A function to obtain the maximum element in the BST
	T max()
	{
		return max(root);
	}
    //! A function to obtain the maximum element in the subtree rooted at current
	T max(node<T> *current)
	{
		if (current == NULL) throw new std::runtime_error("Cannot obtain maximum element from an empty subtree");
		if (current->right) return max(current->right);
		else return current->data;
	}
    //! A function to obtain the minimum element in the BST
	T min()
	{
		return min(root);
	}
    //! A function to obtain the minimum element in the subtree rooted at current
	T min(node<T> *current)
	{
		if (current == NULL) throw new std::runtime_error("Cannot obtain minimum element from an empty subtree");
		if (current->left) return min(current->left);
		else return current->data;
	}
	//! A function to query the next element of data, i.e., the smallest element that is larger than data
	T next(T& data)
	{
 		node<T>* current = contains(root,data);
 		if (current == NULL) throw new std::runtime_error("Cannot obtain the next element for a non-exist element");
 		if (current->right)
 		{
 			return min(current->right);
 		}
 		else
 			throw new std::runtime_error("Cannot obtain next element for the maximum element");
	}
	//! A function to query the previous element of data, i.e., the largest element that is smaller than data
	T previous(T& data)
	{
		node<T>* current = contains(root,data);
 		if (current == NULL) throw new std::runtime_error("Cannot obtain the previous element for a non-exist element");
 		if (current->left)
 		{
 			return max(current->left);
 		}
 		else
 			throw new std::runtime_error("Cannot obtain previous element for the minimum element");
	}
	//! A function to print out the BST in postorder
	void print()
	{
	   std::vector<T> notUsed;
       explore(root,false,notUsed);
       std::cout << std::endl;
	}
	//! A function to convert the BST to an ordered vector
    std::vector<T> toSortedVector()
    {
    	std::vector<T> myVec;
    	explore(root,true,myVec);
    }

};

#endif