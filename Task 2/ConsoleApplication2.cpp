// ConsoleApplication2.cpp: определяет точку входа для консольного приложения.
//

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "merkletree.h"
#include <string>
using namespace std;


string generate() 
{
	string ret = "";
	for (size_t i = 0; i < 512; ++i) 
	{
		ret += char('a' + rand() % 26);
	}
	return ret;
}

inline bool ready(mutex &mtx, queue<string> &que)
{
	lock_guard<mutex> guard(mtx);
	return que.size() >= 4;
}

int produce(mutex &mtx, queue<string> &que, condition_variable &var) 
{
	int ret = 0;
	int t = 128 * 4;
	for (size_t i = 0; i < t; ++i) 
	{
		string str = generate();
		mtx.lock();
		que.push(str);
		if (que.size() >= 4) var.notify_all();
		mtx.unlock();
	}
	return ret;
}

void consume(mutex &mtx, queue<string> &que, condition_variable &var) {
	ofstream file("hashblocks.txt");
	for (size_t i = 0; i < 128; ++i) 
	{
		while (!ready(mtx, que)) 
		{
			std::unique_lock<std::mutex> lock(mtx);
			var.wait(lock);
		}
		mtx.lock();
		std::vector<std::string> blocks;
		for (size_t j = 0; j < 4; ++j) 
		{
			blocks.push_back(que.front());
			que.pop();
		}
		mtx.unlock();
		Node *node = new Node(blocks, 0, 4 - 1);
		file << node->get_str() << "\n";
		delete node;
	}
	file.close();
}

int main() 
{
	srand(time(NULL));
	mutex mtx;
	queue<string> que;
	condition_variable var;
	thread consumer(consume, ref(mtx), ref(que), ref(var));
	thread producer(produce, ref(mtx), ref(que), ref(var));
	consumer.join();
	producer.join();
	return 0;
}


