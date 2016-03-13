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
	b.insert("zack", 2);
	b.insert("alex", 3);
	b.insert("tom", 1);
	b.insert("john", 8);
	b.insert("mat", 5);
	b.insert("sara", 7);
	
    b.insert("jack", 5);
	b.insert("mack", 7);
	b.insert("erin", 5);
	b.insert("derek", 7);
	b.insert("bob", 13);
	b.insert("carly", 20);
	b.print();
	//b.RangeQuery("iris", "sara");
	return 0;
}