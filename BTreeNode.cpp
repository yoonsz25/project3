#include "BTreeNode.h"

BTreeNode::BTreeNode(){
    childPtr = new BTreeNode*[5];
    for(int i = 0; i < maxChild; i++)
        root->childPtr[i] = nullptr; 
    keys = new string[4];
    for (int i 0; i<4; i++)
        keys[i] = "?";
    leaves = new Person[3];
    for(int i = 0; i < maxLeaves; i++){
        Person *p = new Person;
        leaves[i] = p;
    }
    parent = nullptr;
    next = nullptr;
    leaf = false;
    numLeaves = 0;
    numChildren = 0;
    numKeys = 0;
}

