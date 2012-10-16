#include "jVector.h"

#define VEC_SIZE 100

#define DO(n) for(int i = 0; i < n; i++)

int main()
{

	jVector<int> v(5);
	
	for(int i = 0; i < 5; i++)
	{
		v[i] = i;
	}	

	for(int i = 0; i < 5; i++)
		cout << v[i] << "\n";

	cout << "inserting 10.\n";
	v.insert(10,3);
	//v.push_back(10);


	DO(v.size())
		cout << v[i] << "\n";


	return 0;

	cout << "starting tests!\n";
	jVector<int> vec(VEC_SIZE);
	srand(time(NULL));
	cout << "vector initialized.\n";
	for(int i = 0; i < VEC_SIZE; i++)
	{
		vec[i] = (rand() % 400) - 200;
	}
	
	for(int i = 0; i < vec.size(); i++)
		cout << vec[i] << "\n";
	cout << "\n";
	for(int i = 0; i < VEC_SIZE / 2; i++)
	{
		vec.erase(rand() % vec.size());	
	}

	for(int i = 0; i < vec.size(); i++)
		cout << vec[i] << "\n";
	cout << "\n";
	for(int i = 0; i < VEC_SIZE / 2; i++)
	{
		vec.push_back(rand() % 400);	
	}

	for(int i = 0; i < vec.size(); i++)
		cout << vec[i] << "\n";


}
