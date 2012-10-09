#ifndef __J_VECTOR_H__
#define __J_VECTOR_H__

#include <iostream>
#include <assert.h>

using std::cout;
template <class T>
class jVector
{
public:
	jVector();
	jVector(int initSize);
	~jVector();

	void resize(int n);
	T &operator[] (int i);

	T erase(int index);

	void insert(T item, int index);
	void push_back(T item);

	int size();

private:
	T *lis;
	int Size;
	bool autoShrink;
};

template <class T>
jVector<T>::jVector()
{
	lis = NULL;
	Size = 0;
}

template <class T>
jVector<T>::jVector(int initSize)
{
	assert(initSize >= 0);
	lis = NULL;
	Size = 0;
	resize(initSize);
}

template <class T>
jVector<T>::~jVector()
{
	free(lis);
}

template <class T>
void jVector<T>::resize(int n)
{
	assert(n >= 0);
	if(lis == NULL)
	{
		lis = new T[n];
	}	
	else
	{
		if(n > Size)
		{
			lis = new(lis) T[n];
		}
		if(autoShrink && n < Size)
		{
			lis = new(lis) T[n];
		}
	}
	Size = n;
}

template <class T>
T &jVector<T>::operator[] (int i )
{
	assert(i >= 0);
	return *(lis + (sizeof(T) * i));
}

template <class T>
int jVector<T>::size()
{
	return Size;
}

template <class T>
T jVector<T>::erase(int index)
{
	for(; index < Size - 1; index++)
		(*this)[index] = (*this)[index+1];
	Size--;
}

template <class T>
void jVector<T>::push_back(T p)
{
	lis = new(lis) T[++Size];
	(*this)[Size - 1] = p;
}

template <class T>
void jVector<T>::insert(T item, int index)
{
	lis = new(lis) T[++Size];
	for(int i = Size-1; i > index; i--)
		lis[i] = lis[i-1];
	lis[index] = item;
}

#endif
