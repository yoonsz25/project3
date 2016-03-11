#ifndef __BTREE_H__
#define __BTREE_H__

#include <string>
#include "BTreeNode.h"

class BTree{
    private:
        BTreeNode* root;
        int maxChild;
        int maxLeaves;
    public:
        BTree();
        
        //BTree(int child, int leaves);
        //come back and update return types of function. 
        //May need helper functions
    
       void insert(std::string a, int dataPtr); // a = name. dataptr = disk pointer. Both make up person
       void insertNonFull(BTreeNode *n, Person *p);
       int split(BTreeNode* n, int i);
       //void splitParent(BTreeNode *parent, int childNode);
       //void splitLeaf(BTreeNode *leaf, Person* p);
     //  BTreeNode* splitRoot();
//
       void insertLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex = -1);
       void splitLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex);
       void sort(Person *leaves, int numLeaves);
       
       void print();
       void print(BTreeNode* n);
        
       Person find(BTreeNode n, std::string a);
       std::string retrieveData(std::string a);
       void RangeQuery(std::string firstPerson, std::string lastPerson);
       
};

#endif