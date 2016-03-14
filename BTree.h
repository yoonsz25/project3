#ifndef __BTREE_H__
#define __BTREE_H__

#include <string>
#include "BTreeNode.h"

class BTree{
    private:
        BTreeNode* root;
        int maxLeaves;
    public:
        BTree();
        
        //BTree(int child, int leaves);
        //come back and update return types of function. 
        //May need helper functions
    
       void insert(std::string a, int dataPtr); // a = name. dataptr = disk pointer. Both make up person
       void splitNode(BTreeNode* n, BTreeNode::Person *p, int leafIndex);
       void split(BTreeNode* n, BTreeNode::Person* p, int i);
       std::string smallestName(BTreeNode* n);
       void splitRoot(BTreeNode *leaf, BTreeNode::Person* p);
       void insertLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex = -1);
       void splitLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex);

       BTreeNode* make6(BTreeNode* full, BTreeNode::Person *p, int index);
       void splitLeafHelper(BTreeNode *n, BTreeNode::Person *p, int leafIndex);
       void sort(BTreeNode::Person *leaves, int numLeaves);
       
       void print();
       void print(BTreeNode* n);
       void listPrint();
    
       
       void printInternalNode(BTreeNode* n);
       
        
       BTreeNode::Person find(std::string a);
       BTreeNode* find(BTreeNode* n, std::string a);
       
      // std::string retrieveData(std::string a);
       void RangeQuery(std::string firstPerson, std::string lastPerson);
       
};

#endif