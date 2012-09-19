#include <iostream>
#include "FragmentedList.h"

using std::cout;

int main()
{
	while(1)
	{
	FragmentedList<int> fList(50);

	srand(time(NULL));

	for(int i = 0; i < 50; i++)
	{
		fList[i] = i;
	}
	cout << "initial list:\n";
	for(int i = 0; i < fList.size(); i++)
	{
		cout << fList[i] << "\n";
	}


	for(int i = 0; i < 35; i++)
		fList.erase(rand() % fList.size());


//	fList.erase(0);
	cout << "After erase: (fragmented)\n";
	for(int i = 0; i < fList.size(); i++)
	{
		cout << fList[i] << "\n";
	}

	fList.defrag();
	cout << "After Defrag:\n";
	for(int i = 0; i < fList.size(); i++)
	{
		cout << fList[i] << "\n";
	}

	for(int i = 0; i < 10; i++)
	{
		int iv = rand() % fList.size();
		cout << "inserting at: " << iv << "\n";
		fList.insert(-52, iv);
	}
	//fList.insert(-52,fList.size() - 1);
	//fList.insert(-52, 0);
	//fList.insert(-98, 0);
	//fList.insert(300, fList.size()-1);

	cout << "After Insert:\n";
	cout << "list size: " << fList.size() << "\n";
	for(int i = 0; i < fList.size(); i++)
	{
		cout << i << " ";
		cout << fList[i] << "\n";
	}

	}

	return 0;

}
