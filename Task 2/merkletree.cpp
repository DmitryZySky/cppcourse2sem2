#include "merkletree.h"
#include "picosha2.h"
using namespace std;

string Node::get_str() 
{
	return str;
}


Node::Node(vector<string> &blocks, int tl, int tr) 
{
	lhs = nullptr;
	rhs = nullptr;
	if (tl == tr) 
	{
		str = picosha2::hash256_hex_string(blocks[tl]);
	}
	else 
	{
		int tm = (tl + tr) >> 1;
		lhs = new Node(blocks, tl, tm);
		rhs = new Node(blocks, tm + 1, tr);
		str = picosha2::hash256_hex_string(lhs->get_str() + rhs->get_str());
	}
}

Node::~Node() 
{
	if (lhs != nullptr) delete lhs;
	if (rhs != nullptr) delete rhs;
}