#pragma once
#ifndef MERKLETREE_H
#define MERKLETREE_H

#include <iostream>
#include <vector>
using namespace std;

class Node
{
	string str;
	Node *lhs, *rhs;

public:
	Node(vector<string> &blocks, int tl, int tr);
	string get_str();
	virtual ~Node();
};
#endif

