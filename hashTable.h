#ifndef _JLIB_HASH_TABLE_H_
#define _JLIB_HASH_TABLE_H_
#include <vector>
#include <string>

using std::vector;
using std::string;

class pair
{
	public:
		pair(string _key, string _val)
		{
			key = _key;
			val = _val;
		}
		string key;
		string val;
};

class jHashTable
{
	public:
		jHashTable(int size);
		void insert(string key, string value);
		int hash(string s);
		string find(const string &key);
		string &operator[](const string &key);
	private:
		vector<pair*> _table;

		int findOpen(const string &s);
};

#endif
