#include "hashTable.h"

jHashTable::jHashTable(int size)
{
	_table.resize(size);
}

int jHashTable::hash(string s)
{
	int h = 0;
	int tv = 1;
	for(int i = 0; i < 5 && i < s.length(); i++)
	{
		h += s[i] * tv;
		tv *= 2;
	}
	return h % _table.size();
}

int jHashTable::findOpen(const string &s)
{
	int h = hash(s);
	int j = h;
	int fail = 0;
	while(_table[h] != NULL)
	{
		fail++;
		h = (j + (fail * fail)) % _table.size();
	}
	return h;
}

void jHashTable::insert(string key, string val)
{
	int o = findOpen(key);
	_table[o] = new pair(key, val);
}

string jHashTable::find(const string &key)
{
	int h = hash(key);
	int j = h;
	int fail = 0;
	while(_table[h] != NULL && _table[h]->key != key)
	{
		fail++;
		h = (j + (fail * fail)) % _table.size();
	}
	return _table[h]->val;
}

string &jHashTable::operator[] (const string &key)
{
	int h = hash(key);
	int j = h;
	int fail = 0;
	while(_table[h] != NULL && _table[h]->key != key)
	{
		fail++;
		h = (j + (fail * fail)) % _table.size();
	}
	if(_table[h] == NULL)
	{
		_table[h] = new pair(key, "");
		return _table[h]->val;
	}
	else
		return _table[h]->val;
}
