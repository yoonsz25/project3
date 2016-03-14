#include "BTreeNode.h"
#include <iostream>
//using namespace std;

BTreeNode::BTreeNode(){
    maxChild = 5;
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

BTreeNode::BTreeNode(const BTreeNode &n){
    maxChild=6;
    childPtr = new BTreeNode*[6];
    for(int i = 0; i <5; i++){
        this->childPtr[i] = n.childPtr[i];
        this->childPtr[i]->parent = this;
    }
    this->childPtr[6] = nullptr;
    keys = new std::string[5];
    for(int i =0; i<4; i++){
        this->keys[i] = n.keys[i];
    }
    this->keys[4] = "?";
    leaves = new Person[3];
    this->parent = n.parent;
    this->next = n.next;
    this->leaf = n.leaf;
    this->numChildren = n.numChildren;
    this->numLeaves = n.numLeaves;
    this->numKeys=n.numKeys;
}

BTreeNode::~BTreeNode(){
    for(int i = 0; i < this->numChildren; i++){
        this->childPtr[i] = nullptr;
    }
    this->parent = nullptr;
    delete[] childPtr;
    delete[] keys;
    delete[] leaves;
}