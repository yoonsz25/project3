#include <string>

class BTree{
    private:
        struct Node{
            bool leaf; //dont really need. Will know if leaf by looking at next level. If null is leaf. Else not leaf
            Node** childPtr; // 5 pointers to children
            std::string* keys; //4 keys 
            Person* leaves; //3 datas
        };
        struct Person{
            std::string name;
            int dataPtr;
        };
        int child;
        int data;
    public:
        BTree();
        
        //come back and update return types of function. 
        //May need helper functions
        bool isLeaf();
        void insert(std::string name, int dataPtr);
        void print();
        
        void find(std::string name);
        std::string retrieveData(std::string name);
        void RangeQuery(std::string firstPerson, std::string lastPerson);
}