#ifndef _AVLTREE_H
#define _AVLTREE_H

#ifndef NULL
#define NULL 0
#endif

#include "AVLHelpers.h"

template <class T> class AVLTree
{ 
private:	
	Node<T>* root;

public:
	AVLTree(void);
	Node<T>* Add(const T& dat); 
	bool Delete(const T& dat);
	bool Search(const T& dat);
	void Clear(void);

private:
	void clear_internal(Node<T> *node);
};

// Initializes a new AVL-Tree, starting with null root pointer
template <class T> AVLTree<T>::AVLTree(void)
{
	root = NULL;
}

// At first it attempts to find the new elelemnt in the tree. If found - there is a collision and
// the function returns found node. Otherwise - adds a new point and return a null-pointer.
template <class T> Node<T>* AVLTree<T>::Add(const T& dat)
{
	if (root == NULL)
	{
		root = new Node<T>(dat);
		return NULL;
	}
	Node<T> *P, *Q, *R, *S, *TT;
	char a;
	TT = NULL;
	P = S = root;
	do {
		if (dat->key < P->dat->key)
		{
			Q = P->left;
			if (Q == NULL)
			{
				Q = new Node<T>(dat);
				P->left = Q;
				break;
			}
		}
		else if (dat->key > P->dat->key)
		{
			Q = P->right;
			if (Q == NULL)
			{
				Q = new Node<T>(dat);
				P->right = Q;
				break;
			}
		}
		else return P;
		if (Q->d != NULL)
		{
			TT = P;
			S = Q;
		}
		P = Q;
	} while(true);
	a = (dat->key < S->dat->key ? -1 : 1);
	R = P = S->Child(a);
	while (P != Q)
	{
		if (dat->key < P->dat->key)
		{
			P->d = -1;
			P = P->left;
		}
		else
		{
			P->d = 1;
			P = P->right;
		}
	}
	if (S->d != a)
	{
		S->d += a;
		return NULL;
	}
	if (R->d == a)
	{
		P = R;
		if (a == 1)
		{
			S->right = R->left;
			R->left = S;
		}
		else
		{
			S->left = R->right;
			R->right = S;
		}
		S->d = R->d = 0;
	}
	else
	{
		P = R->Child(-a);
		if (a == 1)
		{
			R->left = P->right;
			P->right = R;
			S->right = P->left;
			P->left = S;
		}
		else
		{
			R->right = P->left;
			P->left = R;
			S->left = P->right;
			P->right = S;
		}
		if (P->d == a)
		{
			S->d = -a;
			R->d = 0;
		}
		else if (P->d == 0)
		{
			S->d = 0;
			R->d = 0;
		}
		else
		{
			S->d = 0;
			R->d = a;
		}
		P->d = 0;
	}
	if (S != root)
	{
		if (TT->right == S)
			TT->right = P;
		else
			TT->left = P;
	}
	else
		root = P;
	return NULL;
}

// Deletes a node from the tree.
template <class T> bool AVLTree<T>::Delete(const T& dat)
{
	if (root == NULL)
		return false;

	Stack<Node<T>*> Stack_Node;
	Stack<char> Stack_Dir;
	Stack_Node.Push(NULL);
	Stack_Dir.Push(1);
	Node<T> *P, *Q, *S, *TT; 
	char a, b;
	int stat;
	P = root;

	do
	{
		if (dat->key < P->dat->key)
		{
			Q = P->left;
			if (Q == NULL)
				return false;
			Stack_Node.Push(P);
			Stack_Dir.Push(-1);
		}
		else if (dat->key > P->dat->key)
		{
			Q = P->right;
			if (Q == NULL)
				return false;
			Stack_Node.Push(P);
			Stack_Dir.Push(1);
		}
		else
			break;
		P = Q;
	} while (true);

	if ((P->left != NULL) && (P->right != NULL))
	{
		Q = P->left;
		Stack_Node.Push(P);
		Stack_Dir.Push(-1);
		while (Q->right != NULL)
		{
			Stack_Node.Push(Q);
			Stack_Dir.Push(1);
			Q = Q->right;
		}
		b = 1;
		P->dat->key = Q->dat->key;
		P = Q;
	}
	else if (P->left == NULL)
		b = -1;
	else
		b = 1;

	Q = Stack_Node.Pop();
	a = Stack_Dir.Pop();

	if (Q == NULL)
		root = P->Child(-b);
	else
	{
		if (a == 1)
			Q->right = P->Child(-b);
		else
			Q->left = P->Child(-b);
	}

	delete P;

	while (Q != NULL)
	{
		TT = Stack_Node.Pop();
		b = Stack_Dir.Pop();
		if (Q->d == a)
		{
			Q->d = 0;
			stat = -1;
		}
		else if (Q->d == 0)
		{
			Q->d = -a;
			stat = 0;
			return true;
		}
		else
		{
			P = Q->Child(-a);
			a = -a;
			if (P->d != -a)
			{
				if (a == 1)
				{
					Q->right = P->left;
					P->left = Q;
				}
				else
				{
					Q->left = P->right;
					P->right = Q;
				}
				if (TT != NULL)
				{
					if (b == 1)
						TT->right = P;
					else
						TT->left = P;
				}
				else
					root = P;
				if (P->d == a)
					P->d = Q->d = 0;
				else
				{
					stat = 1;
					Q->d = a;
					P->d = -a;
					return true;
				}
				stat = 1;
			}
			else
			{
				S = P->Child(-a);
				if (a == 1)
				{
					P->left = S->right;
					S->right = P;
					Q->right = S->left;
					S->left = Q;
				}
				else
				{
					P->right = S->left;
					S->left = P;
					Q->left = S->right;
					S->right = Q;
				}
				if (S->d == a)
				{
					Q->d = -a;
					P->d = 0;
				}
				else if (S->d == 0) 
					Q->d = P->d = 0;
				else
				{
					Q->d = 0;
					P->d = a;
				}
				S->d = 0;
				if (TT != NULL)
				{
					if (b == 1)
						TT->right = S;
					else
						TT->left = S;
				}
				else
					root = S;
				stat = 2;
			}
		}
		P = Q;
		Q = TT;
		a = b;
	}
	Stack_Node.Clear();
	Stack_Dir.Clear();
	return true;
}

// Attempts to locate a node in the tree and returns boolean value indicating result of the search operation.
template <class T> bool AVLTree<T>::Search(const T& dat)
{
	if (root == NULL) return false;
	Node<T>* P = root;
	do
	{
		if (dat->key < P->dat->key) P = P->left;
		else if (dat->key > P->dat->key) P = P->right;
		else return true;
		if (P == NULL) 
			return false;
	} while(true);
}

// Clears the contents of the tree.
template <class T> void AVLTree<T>::Clear(void)
{
	clear_internal(root);
	root = NULL;
}

// Clears contents of a sub-tree with given root node
template <class T> void AVLTree<T>::clear_internal(Node<T> *node)
{
	if (node == NULL)
		return;
	clear_internal(node->left);
	clear_internal(node->right);
	delete [] node->dat;
	delete node;
}

#endif
