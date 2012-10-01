#include <iostream>
#include <ctime>
#include <cstdlib>
#include "FragmentedList.h"


using std::cout;
#define LIST_SIZE 1000
int main()
{
	int i = 0;
    while ( true )
    {
      FragmentedList<int> fList(LIST_SIZE);
      srand(time(NULL));
      //srand(i);
	  cout << "\n\nSeed: " << i << "\n";
      for(int i = 0; i < LIST_SIZE; i++)
	  {
		  fList[i] = i;
	  }
	  //cout << "Initial List:\n";
	  for(int i = 0; i < fList.size(); i++)
	  {
		  //cout << " " << fList[i] << "\n";
	  }


	  for(int i = 0; i < 5; i++)
		  fList.erase(rand() % fList.size());


	  //cout << "\n\nAfter Erase: (fragmented)\n";
	  for(int i = 0; i < fList.size(); i++)
	  {
		  //cout << " " <<fList[i] << "\n";
	  }

	  fList.defrag();
	  //cout << "\n\nAfter Defrag:\n";
	  for(int i = 0; i < fList.size(); i++)
	  {
		  //cout << " " << fList[i] << "\n";
	  }

	  //cout << "\n\nInserting:\n";
       fList.dumpDebug();
      for(int i = 0; i < LIST_SIZE/4; i++)
	  {
		  int iv = rand() % fList.size();
		  //cout << " list[" << iv << "]\n";
		  fList.insert(-52, iv);
           fList.dumpDebug();
	  }
	  //fList.insert(-52,fList.size() - 1);
	  //fList.insert(-52, 0);

	  //cout << "\n\nAfter Insert:\n";
	  //cout << " List size: " << fList.size() << "\n";
	  for(int i = 0; i < fList.size(); i++)
	  {
		  //cout << " " << i << " : ";
		  //cout << fList[i] << "\n";
	  }
      	  //cout << "\n == END OF TESTS SET ONE == " << i;
	  i++;
      fList.dumpDebug();
      }
      return 0;
	FragmentedList<int> * defrag;	

	//cout << "\n\n Other Test \n\n";
	defrag = new FragmentedList<int>(5);

	for(int i = 0; i < 5; i++)
	{
		(*defrag)[i] = i;
	}

	(*defrag).erase(1);
	(*defrag).erase(2);
	(*defrag).insert(1,1);
	(*defrag).insert(3,3);

	
	for(int i = 0; i < (*defrag).size(); i++)
	{
		//cout << i <<" " << (*defrag)[i] << "\n";
	}
	//cout << "Defrag\n";
	(*defrag).defrag();	

	for(int i = 0; i < 5; i++)
	{
		//cout << (*defrag)[i] << "\n";
	}

	delete defrag;
	//cout << "\n\n Other Test 2 \n\n";
	defrag = new FragmentedList<int>(7);

	for(int i = 0; i < 7; i++)
	{
		(*defrag)[i] = i;
	}

	(*defrag).erase(1);
	(*defrag).erase(3);
	(*defrag).insert(1,1);
	(*defrag).insert(4,4);

	
	for(int i = 0; i < (*defrag).size(); i++)
	{
		//cout << i <<" " << (*defrag)[i] << "\n";
	}
	//cout << "Defrag\n";
	(*defrag).defrag();	

	for(int i = 0; i < (*defrag).size(); i++)
	{
		//cout << (*defrag)[i] << "\n";
	}

	delete defrag;
	

	// Below test still breaks
	//
	// The below test is when we use free spots


	//cout << "\n NEXT SOME OTHER\n\n";
	defrag = new FragmentedList<int>(5);

	for(int i = 0; i < 5; i++)
	{
		(*defrag)[i] = i;
	}

	(*defrag).erase(1);
	//(*defrag).erase(1);
	//(*defrag).insert(1,1);
	//(*defrag).insert(2,2);

	
	for(int i = 0; i < (*defrag).size(); i++)
	{
		//cout << i <<" " << (*defrag)[i] << "\n";
	}
	//cout << "Defrag\n";
	(*defrag).defrag();	
	//cout << "Print\n";
	for(int i = 0; i < (*defrag).size(); i++)
	{
		//cout << (*defrag)[i] << "\n";
	}
	return 0;
}
