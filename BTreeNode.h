#ifndef __BTREENODE_H__
#define __BTREENODE_H__

#include <string>
 

            
class BTreeNode{
       friend class BTree;
      
        private:
           
            struct Person{
                Person(){
                    name = "!";
                    dataPtr = -1;
                }
                std::string name;
                int dataPtr;
            };

            bool leaf;
            BTreeNode** childPtr; // 5 pointers to children
            std::string* keys; //4 keys
            Person* leaves; //3 datas

            BTreeNode *parent; // points to parent of current BTreeNode. If root, points to nullptr
            BTreeNode *next;   // points to the next BTreeNode with leaves for easy range queries. If leaf is false, this should point to nullptr

            int numLeaves;
            int numChildren; // dont need if we have numKeys
            int numKeys;
        
        public:
            
            BTreeNode();
            BTreeNode(const BTreeNode &n);
            
};

#endif