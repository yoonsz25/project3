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
            int numLeaves;
            int numChildren;
        
        public:
            BTreeNode();
            
};

#endif