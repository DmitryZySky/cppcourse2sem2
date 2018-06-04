#include "stdafx.h"
#include "Actions.h"
#include <fstream>
#include <windows.h>
#include <iostream>
#include "picosha2.h"
#include "Actions.h"
using namespace std;

Actions::Actions() : size(0), hashed(0) {}

bool Actions::push(vector<unsigned char> &block) 
{
	lock_guard<mutex> lock(mtx);
	if (size < 512) 
	{
		VectorHash b;
		swap(b.data, block);
		lst.push_front(b);
		++size;
		var.notify_all();
		return true;
	}
	return false;
}

bool Actions::hash() 
{
	unique_lock<mutex> locker(mtx);
	while (size == hashed && hashed < 512)
	{
		var.wait(locker);
	}
	if (hashed >= 512) return false;
	auto it = lst.begin();
	while (it->hash.size() != 0 && it != lst.end()) it++;
	it->hash.resize(picosha2::k_digest_size);
	picosha2::hash256(it->data, it->hash);
	++hashed;
	system("cls");
	std::cout << "waiting: " << size - hashed << "\nhashed:  " << hashed
		<< "\n" << hashed * 100 / 512 << "%";
	return hashed < 512;
}

void Actions::write(const char *filename) 
{
	ofstream file(filename, ios::binary);
	for (VectorHash &b : lst) 
	{
		for (unsigned char &ch : b.hash) file << ch;
	}
	file.close();
	cout << "\n";
	system("pause");
}
