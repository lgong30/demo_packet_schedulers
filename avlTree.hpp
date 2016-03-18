/*
	C++ Implementation for AVL tree.
	Shamelessly copy some parts from http://www.geeksforgeeks.org/avl-tree-set-2-deletion/ 
	version 1.0.0
	https://github.com/xlong88/bst

*/
#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "bst.hpp"

//! A class for AVL Tree
/*!
 	Generic AVL Tree.
	Can be used with an customized comparator instead of the natural order,
	but the generic Value type must still be comparable.
*/
template <class T,class Compare = std::less<T> >
class AVL_Tree: public BST<T,Compare>
{
public:
	//! A constructor 
	AVL_Tree(Compare uLess = Compare()):BST<T,Compare>(uLess){}
	//! A constructor
	AVL_Tree(std::vector<T>& data,Compare uLess = Compare()):BST<T,Compare>(data,uLess){}
	//! A function to insert a new element in the AVL Tree
	void insert(T& data)
	{
		root = insert(root,data);
	}
	//! A recursive function to insert an element in the subtree rooted at current, perform rotation if necessary
	node<T>* insert(node<T> *current,T& data)
	{
		//! insert the new element
		if (current == NULL)
			return (new node<T>(data));


		if (Less(data,current->data))
			current->left = insert(current->left,data);
		else
			current->right = insert(current->right,data);

		//! update height
		current->height = std::max(height(current->left),height(current->right)) + 1;

		//! check whether rotation is needed
		int balance = heightDif(current);

		if (balance > 1)
		{//! left-heavy
			if (Less(data,current->left->data))
			{//! left-heavy
				return right_rotate(current);
			}
			else
			{//! right-heavy
				current->left = left_rotate(current->left);
				return right_rotate(current);
			}
		}
		else if (balance < -1)
		{//! right-heavy
			if (Less(data,current->right->data))
			{//!left-heavy
               current->right = right_rotate(current->right);
               return left_rotate(current);
			}
			else
			{//! right-heavy
				return left_rotate(current);
			}

		}
		return current;

	}
	//! A function to perform left rotate at current node
	node<T>* left_rotate(node<T>* current)
	{
		node<T>* right = current->right;
		current->right = right->left;
		right->left = current;

		current->height = std::max(height(current->left),height(current->right)) + 1;
		right->height = std::max(height(right->left),height(right->right)) + 1s;

		return right;
	}
	//! A function to perform right rotate at current node
	node<T>* right_rotate(node<T>* current)
	{
		node<T>* left = current->left;
		current->left = left->right;
		left->right = current;

		current->height = std::max(height(current->left),height(current->right)) + 1;
		left->height = std::max(height(left->left),height(left->right)) + 1;
	}
	int height()
	{
		return height(root);
	}
	//! A function to obtain the height of current node
	int height(node<T>* current)
	{
		if (current == NULL) return -1;
		return current->height;
	}
	//! A function to obtain the height difference between left and right child
	int heightDif(node<T>* current)
	{
		if (current == NULL) return 0;
		return height(current->left) - height(current->right);

	}
	//! A function to remove data in the AVL Tree
	void remove(T& data)
	{
		root = remove(root,data);
	}
	//! A function to remove element data from the subtree rooted at current, perform rotation if necessary
	node<T>* remove(node<T>* current,T& data)
	{
		if (current == NULL)
			throw new std::runtime_error("Cannot remove non-exist element");

		if (data == current->data)
		{
			if (!current->left)
				return remove(current->right,data);
			else if (!current->right)
				return remove(current->left,data);
			else
			{
				current->data = retrievalData(current->left);
				current->left = remove(current->left,current->data);
			}
		}

		if (current == NULL) return current;

		current->height = std::max(height(current->left),height(current->right)) + 1;

		int balance = heightDif(current);

		if (balance > 1)
		{// left-heavy
			if (heightDif(current->left) >= 0)
			{// left-heavy or balance
				return right_rotate(current);
			}
			else
			{// right-heavy
				current->left = left_rotate(current->left);
				return right_rotate(current);
			}

		}
		else if (balance < - 1)
		{// right-heavy
			if (heightDif(current->right) <= 0)
			{// right-heavy or balance
				return left_rotate(current);
			}
			else
			{// left-heavy
				current->right = right_rotate(current->right);
				return left_rotate(current);
			}
		}
        return current;

	}
};

#endif