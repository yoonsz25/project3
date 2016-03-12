#include "BTreeNode.h"
#include <iostream>
//using namespace std;

BTreeNode::BTreeNode(){
    childPtr = new BTreeNode*[5];
    for(int i = 0; i < 5; i++) //was max child
        this->childPtr[i] = nullptr; 

    keys = new std::string[4];
    for (int i = 0; i<4; i++)
        this->keys[i] = "?";

    leaves = new Person[3];

    parent = nullptr;
    next = nullptr;
    leaf = false;
    numLeaves = 0;
    numChildren = 0;
    numKeys = 0;
}

