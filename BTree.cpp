#include "BTree.h"
#include "BTreeNode.h"
#include <iostream>
#include <string>

using namespace std;

BTree::BTree(){
    root = new BTreeNode;
    this->maxChild = 5;         //min 3, max 5
    this->maxLeaves = 3;        //min 2, max 3
    root->leaf = true;
    for(int i = 0; i < maxChild; i++)
        root->childPtr[i] = nullptr;    //nullptr evaluates to false in boolean operations
    for(int i = 0; i < maxLeaves; i++){
        Person *p = new Person;
        leaves[i] = p;
    }
           
}


BTree::Person BTree::find(BTreeNode* n, string a){
    if(n==NULL){
        return NULL;
    }
    if(!n->leaf){
        int location = a.compare(n->keys[0]);
        int nextOne =0;
        if(location<0){
            return find(n->childPtr[0],a);
        }
        for(int i = 0; i<numChildren-2; i++){
            location = a.compare(n->keys[i]);
            nextOne = a.compare(n->keys[i+1]);
            if (location >= 0 && nextOne < 0){
                return find(n->childPtr[i+1], a);
            }
        }
        return find(n->childPtr[numChildren-1], a);
    }
    if(n->leaf){
        for(int i = 0; i<n->numLeaves; i++){
            if(a.compare(n->leaves[i].name)==0)
                return n->leaves[i];
        }
        return NULL;
    }
}


//dataptr is the ptr to the file on disk (dataptr*53)

void BTree::insert(std::string a, int dataPtr){
    //first find where the name should go.
    Person *p = new Person;
    p.name = a;
    p.dataPtr = dataPtr;
    if(this->root->numChildren == 5){
        //split the BTreeNode
        
        
        //What goes here
    }
    else{
        insertNonFull(root, p);
    }
}

void BTree::insertNonFull(BTreeNode *n, Person *p){
    if(n->leaf){
        insertLeaf(n, p);
    }
    else{
        //traverse through tree to find where p goes;
    }
}

void BTree::insertLeaf(BTreeNode *n, Person *p){
    if(n->leaves[0].name.compare("!") == 0){
        n->leaves[0] = *p;
    }
    Person *tmp = new Person;
   // need to sort the items in this loop
   // also need to check num leaves. If numLeavs == maxLeaves. Need to split nodes... Difficult?
   
    for(int i = 0; i < this->maxLeaves; i++){
    }
}

