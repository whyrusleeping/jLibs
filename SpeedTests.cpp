#include "FragmentedList.h"
#include <list>
#include <vector>
#include <sys/times.h>
#include <iostream>

#define TEST_LIST_SIZE 100000
#define RAND_ERASE_NUM 30000

using std::cout;
using std::list;
using std::vector;

typedef struct _results
{
	unsigned int InitTime;
	unsigned int RandEraseTime;
	unsigned int RandInsertTime;
}Results;

int main()
{
	struct tms fl_st_cpu;
	struct tms fl_en_cpu;
	Results FragListRes;
	Results StlListRes;
	Results StlVecRes;
	
	srand(time(NULL));

	cout << "testing fragmented list.\n";
	times(&fl_st_cpu);
	FragmentedList<int> FragList(TEST_LIST_SIZE);
	for(int i = 0; i < TEST_LIST_SIZE; i++)
		FragList[i] = rand() % 10000;
	times(&fl_en_cpu);
	FragListRes.InitTime = fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;

	times(&fl_st_cpu);
	for(int i = 0; i < 	RAND_ERASE_NUM; i++)
		FragList.insert(rand() % 6000, rand() % FragList.size());
	times(&fl_en_cpu);
	FragListRes.RandInsertTime =  fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;

	times(&fl_st_cpu);
	for(int i = 0; i < 	RAND_ERASE_NUM; i++)
		FragList.erase(rand() % FragList.size());
	times(&fl_en_cpu);
	FragListRes.RandEraseTime =  fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;


	cout << "now testing list.\n";
	times(&fl_st_cpu);
	list<int> StlList(TEST_LIST_SIZE);
	for(list<int>::iterator i = StlList.begin(); i != StlList.end(); i++)
		*i = rand() % 10000;
	times(&fl_en_cpu);
	StlListRes.InitTime = fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;

	times(&fl_st_cpu);
	list<int>::iterator it;
	for(int i = 0; i < 	RAND_ERASE_NUM; i++)
	{
		it = StlList.begin();
		advance(it, rand() % StlList.size());
		StlList.insert(it, rand() % StlList.size());
	}	
	times(&fl_en_cpu);
	StlListRes.RandInsertTime =  fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;


	times(&fl_st_cpu);
	for(int i = 0; i < 	RAND_ERASE_NUM; i++)
	{
		it = StlList.begin();
		advance(it, rand() % StlList.size());
		StlList.erase(it);
	}
	times(&fl_en_cpu);
	StlListRes.RandEraseTime =  fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;


	cout << "now testing vector.\n";
	//Stl Vector Testing
	times(&fl_st_cpu);
	vector<int> Vec(TEST_LIST_SIZE);
	for(int i = 0; i < TEST_LIST_SIZE; i++)
		Vec[i] = rand() % 10000;
	times(&fl_en_cpu);
	StlVecRes.InitTime = fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;

	times(&fl_st_cpu);
	for(int i = 0; i < 	RAND_ERASE_NUM; i++)
		Vec.insert(Vec.begin() + (rand() % 6000), rand() % Vec.size());
	times(&fl_en_cpu);
	StlVecRes.RandInsertTime =  fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;

	times(&fl_st_cpu);
	for(int i = 0; i < 	RAND_ERASE_NUM; i++)
		Vec.erase(Vec.begin() + (rand() % Vec.size()));
	times(&fl_en_cpu);
	StlVecRes.RandEraseTime =  fl_en_cpu.tms_utime - fl_st_cpu.tms_utime;



	cout << "Fragmented List Times:\n";
	cout << "Init:\t\t\t" << FragListRes.InitTime << "us\n";
	cout << "Random Delete:\t"	<< FragListRes.RandEraseTime << "us\n";
	cout << "Random Insert:\t" << FragListRes.RandInsertTime << "us\n";

	cout << "STL List Times:\n";
	cout << "Init:\t\t\t" << StlListRes.InitTime << "us\n";
	cout << "Random Delete:\t" << StlListRes.RandEraseTime << "us\n";
	cout << "Random Insert:\t" << StlListRes.RandInsertTime << "us\n";

	cout << "STL Vector Times:\n";
	cout << "Init:\t\t\t" << StlVecRes.InitTime << "us\n";
	cout << "Random Delete:\t" << StlVecRes.RandEraseTime << "us\n";
	cout << "Random Insert:\t" << StlVecRes.RandInsertTime << "us\n";


}
