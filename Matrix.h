#include <iostream>
#include <jVector.h>

template <class T>
class Matrix
{
	public:
		Matrix();
		Matrix(int width, int length);
		~Matrix();

		void resize(int width, int length);
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
	assert(width >= 0);
	assert(length >= 0);
	resize(w,l);
}

template <class T>
Matrix<T>::~Matrix()
{

}

template <class T>
void Matrix<T>::resize(int width, int length)
{
	matrix.resize(width);
	for(int i = 0; i < width; i++)
	{
		matrix[i].resize(length);
	}
}
