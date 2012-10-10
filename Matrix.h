#include <iostream>
#include <assert.h>
#include "jVector.h"

typedef struct _coord
{
	int x;
	int y;
}coord;

template <class T>
class Matrix
{
	public:
		Matrix();
		Matrix(int width, int length);
		~Matrix();
		
		bool inMatrix(int x, int y);
		void resize(int width, int length);
		int width();
		int length();
		T &operator[] (coord at);
		T &operator() (int x, int y);
	private:
		jVector<jVector<T> > matrix;
		int w;
		int l;
};

template <class T>
Matrix<T>::Matrix()
{
	w = 0;
	l = 0;
}

template <class T>
Matrix<T>::Matrix(int width, int length)
{
	resize(width,length);
}

template <class T>
Matrix<T>::~Matrix()
{

}

template <class T>
void Matrix<T>::resize(int width, int length)
{
	assert(width >= 0);
	assert(length >= 0);
	matrix.resize(width);
	for(int i = 0; i < width; i++)
	{
		matrix[i].resize(length);
	}
	w = width;
	l = length;
}

template <class T>
bool Matrix<T>::inMatrix(int x, int y)
{
	return (x >= 0 && x < w && y >= 0 && y < l);
}

template <class T>
int Matrix<T>::width()
{
	return w;
}

template <class T>
int Matrix<T>::length()
{
	return l;
}
template <class T>
T &Matrix<T>::operator[] (coord c)
{
	return matrix[c.x][c.y];	
}
	
template <class T>
T &Matrix<T>::operator() (int x, int y)
{
	return matrix[x][y];	
}
