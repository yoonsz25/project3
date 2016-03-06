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
    root->parent = nullptr;
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


    
}



void BTree::insertLeaf(BTreeNode *n, Person *p){
   // need to sort the items in this loop
   // also need to check num leaves. If numLeavs == maxLeaves. Need to split nodes... Difficult?
   if(n->leaf == false){
   		cout << "this node is not a leaf. Why is insertLeaf being called on it. Requires debugging." << endl;
   }

   for(int i = 0; i < this->maxLeaves; i++){
   		if(n->leaves[i].compare("!") == 0){
   			n->leaves[i] = p;
   			cout << p.name << ": succesfully inserted into index " << i << endl;
   			sort(n->leaves, i+1);
   			return;
   		}
   }
   cout << "item not succesfully inserted into leaves. Thus, leaves are full and we have to fix the tree" << endl;
   
}


//parent[childNode] is the node which is full. Need to split that one.
void BTree::splitChild(BTreeNode *parent, int childNode){







}

//no need for nlogn sort. Sorting 3 or less items
//should use a better sort if scaling
//bubble sort?
void BTree::sort(Person *leaves, int numLeaves){
	
	Person tmp;
	for(int i = 0; i < numLeaves; i++){
		for(int j = i; j < numLeaves; i++){
			if(leaves[i]->name.compare(leaves[j]->name) > 0){
				tmp = leaves[i];
				leaves[i] = leaves[j];
				leaves[j] = tmp;
			}
		}
	}
}







