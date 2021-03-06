// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//
#include <iostream>
#include <thread>
#include <random>
#include "Actions.h"
#include "picosha2.h"
using namespace std;

void produce(Actions &lst)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(0, 255);
	auto rand = bind(dist, gen);
	while (true)
	{
		vector<unsigned char> block(512 * 1024);
		for (unsigned char& ch : block)
		{
			ch = (unsigned char)rand();
		}
		if (!lst.push(block)) break;
	}
}

void consume(Actions &lst)
{
	while (lst.hash()) {}
}

int main()
{
	Actions lst;
	thread producer1(produce, ref(lst));
	thread producer2(produce, ref(lst));
	thread consumer1(consume, ref(lst));
	thread consumer2(consume, ref(lst));
	producer1.join();
	producer2.join();
	consumer1.join();
	consumer2.join();
	lst.write("C:\\hashs.bin");
	return 0;
}