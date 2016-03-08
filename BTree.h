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
    
     //  void insert(std::string a, int dataPtr); // a = name. dataptr = disk pointer. Both make up person
       void insertNonFull(BTreeNode *n, BTreeNode::Person *p);
       void splitParent(BTreeNode *parent, int childNode);
       void splitLeaf(BTreeNode *leaf, BTreeNode::Person* p);
     //  BTreeNode* splitRoot();
       void insertLeaf(BTreeNode *n, BTreeNode::Person *p);
       void sort(BTreeNode::Person *leaves, int numLeaves);
       
       void print();
        
     //  BTreeNode::Person find(BTreeNode* n, std::string a);
       std::string retrieveData(std::string a);
       void RangeQuery(std::string firstPerson, std::string lastPerson);
       
       // new functions for insert
       void insert(std::string a, int dataPtr);
       void insertNode(BTreeNode* n);
       //split splits the current b tree node into 2 and links them back to 1 b tree node
       BTreeNode* split(BTreeNode *n);
};

#endif