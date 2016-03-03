#include <string>

class BTree{
    private:
        struct Node{
            bool leaf; //dont really need. Will know if leaf by looking at next level. If null is leaf. Else not leaf
            
        }
        int child;
        int data;
    public:
        BTree();
        
}