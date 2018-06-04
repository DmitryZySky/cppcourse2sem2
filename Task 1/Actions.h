#pragma once

#ifndef PRODUCER_CONSUMER_ACTIONS_H
#define PRODUCER_CONSUMER_ACTIONS_H
#include "VectorHash.h"
#include <mutex>
#include <atomic>
#include <forward_list>
#include <condition_variable>
using namespace std;

class Actions 
{

	condition_variable var;
	forward_list<VectorHash> lst;
	mutex mtx;
	atomic<size_t> size, hashed;


public:
	Actions();
	bool push(vector<unsigned char> &block);
	bool hash();
	void write(const char* filename);
};
#endif

