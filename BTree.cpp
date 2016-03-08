#include "BTree.h"
#include "BTreeNode.h"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

BTree::BTree(){
    root = new BTreeNode();
    this->maxChild = 5;         //min 3, max 5
    this->maxLeaves = 3;        //min 2, max 3
    root->leaf = true;
   //nullptr evaluates to false in boolean operations
}

/*
BTreeNode::Person BTree::find(BTreeNode* n, string a){
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
*/





void BTree::insertLeaf(BTreeNode *n, BTreeNode::Person *p){
   // need to sort the items in this loop
   // also need to check num leaves. If numLeavs == maxLeaves. Need to split nodes... Difficult?
   if(n->leaf == false){
   		cout << "this node is not a leaf. Why is insertLeaf being called on it. Requires debugging." << endl;
   }
   
   if(n->numLeaves == this->maxLeaves){
          splitLeaf(n, p);  
   }
    else{
       for(int i = 0; i < this->maxLeaves; i++){
       		if(n->leaves[i].name.compare("!") == 0){
       			n->leaves[i] = *p;
       			cout << p->name << ": succesfully inserted into index " << i << endl;
       			++(n->numLeaves);
                cout << n->numLeaves << endl;
       			sort(n->leaves, n->numLeaves);
       		    return;
       	    }
        }
    }
}

//preconditions: if the target leaf is full.
//postcondition: new leaf is inserted into the BTree
void BTree::splitLeaf(BTreeNode *leaf, BTreeNode::Person* p){
  /*
    BTreeNode* up = leaf->parent;
    BTreeNode* sib = leaf->next;
    
    BTreeNode* splitted = new BTreeNode();
    splitted->leaf = true;
    splitted->parent = up;
    //if this doesn't work - make a sort() function to sort array of Person
    if(sib!=NULL){
        splitted->next = sib;
        leaf=>next = splitted;
    }
    if(p->name.compare(leaf->leaves[1].name) >= 0){
        if(p->name.compare(leaf->leaves[2].name) >= 0){
            splitted->leaves[1] = p;
            splitted->leaves[0] = leaf->leaves[2];
        }
        else{
            splitted->leaves[0] = p;
            splitted->leaves[1] = leaf->leaves[2];
        }
    }
    else{
        splitted->leaves[0] = leaf->leaves[1];
        splitted->leaves[1] = leaf->leaves[2];
        if(p->name.compare(leaf->leaves[0].name) <= 0){
            leaf->leaves[1] = leaf->leaves[0];
            leaf->leaves[0] = p;
        }
        else{
            leaf->leaves[1] = p;
        }
    }
    leaf->leaves[2] = NULL;
    leaf->numLeaves = 2;
    splitted->numLeaves = 2;
    for(int i = 0; i < numKeys-1; i++){
        if(up->keys[i].compare(splitted->leaves[0].name)<=0 && up->keys[i+1].compare(splitted->leaves[0].name) > 0){
            string temp = up->keys[i+1];
            up->keys[j] = splitted->leaves[0].name;
            for(int j = i+1; j < numKeys; j++){
                
            }
        }
    }
    */
}


//n->parent[childNode] is the node which is full. Need to split that one.
void BTree::splitParent(BTreeNode *parent, int childNode){

    //debugging stuff
    //first check if parent is full. 
    if(parent->numChildren != 5){
        cout << "this parent has less than 5 children. Does it need to be split still" << endl;
    }
    //parent(current node) is full. Check if its parent is full
    if(parent->parent->numChildren == 5){
        //code to split. Recursion? Iterativiely? 
        splitParent(parent->parent, 1 );
    }
    
    //since parent of parent does not have 5 children. We can split current node and move up
    else{
        //5 child ptrs
        BTreeNode *tmp = new BTreeNode();
        tmp->numChildren = ceil((parent->numChildren)/2);
        tmp->numKeys = parent->numKeys/2;
        
        //for(int i = ceil(parent->numchildren/2); i<parent->numChildren; i++)
        int tmpChildCounter = 0;
        //set the fields for the tmp BTreeNode;
        for(int i = 3; i < parent->numChildren; i++){
            if(i < this->maxChild-1){
                tmp->keys[tmpChildCounter] = parent->keys[i];
            }
            tmp->childPtr[tmpChildCounter] = parent->childPtr[i];
            parent->childPtr[i] = nullptr;
            ++tmpChildCounter;
        }
        
        //set
    }


}

//no need for nlogn sort. Sorting 3 or less items
//should use a better sort if scaling
//bubble sort?
void BTree::sort(BTreeNode::Person *leaves, int numLeaves){
	
	BTreeNode::Person tmp;
	for(int i = 0; i < numLeaves; i++){
		for(int j = i; j < numLeaves; j++){
			if(leaves[i].name.compare(leaves[j].name) > 0){
				tmp = leaves[i];
				leaves[i] = leaves[j];
				leaves[j] = tmp;
			}
		}
	}
}


/*
    two functions 
    insert - does all insert
    need helper that inserts btreenode
    split- only splits and returns btreenode
*/

//dataptr is the ptr to the file on disk (dataptr*53)

void BTree::insert(std::string a, int dataPtr){
    //first find where the name should go.
    BTreeNode::Person *p = new BTreeNode::Person();
    p->name = a;
    p->dataPtr = dataPtr;

    BTreeNode *n = root;
    int keyNum = 0;
 
    //search for where item goes;
    while(keyNum < n->numKeys && n->leaf == false){
        if(n->keys[keyNum].compare(a) < 0){
            n = n->childPtr[keyNum];
            keyNum = 0;
        }
        if(n->numKeys == keyNum+1 && n->keys[keyNum].compare(a) > 0){
            n = n->childPtr[keyNum+1];
            keyNum = 0;
        }
        else
            ++keyNum;
    }
    // now n should be at BTreeNode which is a leaf
    insertLeaf(n, p);

}


void insertNode(BTreeNode* n);
void split(BTreeNode *n);






