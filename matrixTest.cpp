#include "Matrix.h"

int main()
{
	Matrix<int> m(5,5);

	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 5; j++)
			m(i,j) = (i * j) + 1;

	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
			cout << m(i,j);
		cout << "\n";
	}
}
