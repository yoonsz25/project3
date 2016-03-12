#include "BTree.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

int main()
{
	BTree b;
	//BTreeNode b;
	b.insert("bob", 2);
	b.insert("alex", 3);
	b.insert("yoon", 1);
	b.print();
	b.insert("john", 8);
	b.insert("jane", 5);
	b.insert("sara", 7);
	b.print();
	return 0;
}