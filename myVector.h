#ifndef __J_VECTOR_H__
#define __J_VECTOR_H__

#include <iostream>
#include "Person.h"

using std::cout;
template <class T>
class jVector
{
public:
	jVector();
	~jVector();

	void resize(int n);
	T &operator[] (int i);

	T erase(int index);


	void push_back(T p);

	int Size();

private:
	T *lis;
	int size;
	bool autoShrink;
};


jVector::jVector()
{
	lis = NULL;
	size = 0;
}

jVector::~jVector()
{
	free(lis);
}

template <class T>
void jVector::resize(int n)
{
	if(lis == NULL)
	{
		lis = (T *)malloc(sizeof(T) * n);
	}	
	else
	{
		if(n > size)
		{
			lis = (T *)realloc(lis, sizeof(T) * n);
		}
		if(autoShrink && n < size)
		{
			lis = (T *)realloc(lis, sizeof(T) * n);
		}
	}
	size = n;
}

template <class T>
T &jVector::operator[] (int i )
{
	return *(lis + sizeof(T) * i);
}

int jVector::Size()
{
	return size;
}

Person jVector::erase(int index)
{
	for(; index < size - 1; index++)
		(*this)[index] = (*this)[index+1];
	size--;
}

template <class T>
void jVector::push_back(T p)
{
	lis = (Person *)realloc(lis, ++size * sizeof(Person));
	(*this)[size - 1] = p;
}
#endif
