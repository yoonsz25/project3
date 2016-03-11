#include "BTree.h"
#include "BTreeNode.h"
#include <iostream>
#include <string>

using namespace std;

BTree::BTree(){
    root = new BTreeNode();
    this->maxChild = 5;         //min 3, max 5
    this->maxLeaves = 3;        //min 2, max 3
    root->leaf = true;
   //nullptr evaluates to false in boolean operations
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

int split(BTreeNode* n, int i){
    int center = 0;
    BTreeNode* first, third, y = NULL;
    third = new BTreeNode();
    third->leaf = true;
    if(i<0){
        center = n->leaves[1];
        n->leaves[1] = *(new BTreeNode::Person());
        n->numLeaves--;
        first = new BTreeNode();
        first->leaf = false;
        n->leaf = true;
        third->leaves[0] = n->leaves[2];
        third->childPtr[0] = n->childPtr[2];
        third->numLeaves++;
        third->numChildren++;
        n->leaves[2] = *(new BTreeNode::Person());
        n->numChildren--;
        for(int i = 0; i<5; i++){
            n->childPtr[i] = NULL;
        }
        first->leaves[0] = center;
        first->childPtr[first->numLeaves] = n;
        first->childPtr[first->numLeaves+1] = third;
        first->numChildren++;
        root = first;
    }
    else{
        y = n->childPtr[i];
        center = y->leaves[1];
        y->leaves[1] = 0;
        y->numChildren--;
        third->leaves[0] = y->leaves[2];
        third->numChildren++;
        y->leaves[2]= 0;
        y->numChildren++;
        n->childPtr[i+1] = y;
        n->childPtr[i+1] = third;
    }
    return center;
}

//dataptr is the ptr to the file on disk (dataptr*53)
void BTree::insert(std::string a, int dataPtr){
     //initialize new person
    BTreeNode::Person *p = new BTreeNode::Person();
    p->name = a;
    p->dataPtr = dataPtr;

    //root is leaf
   
    BTreeNode *n = root;
    int indexToLeaf = 0;
    int i = 0;

    //root special case. Root is leaf and full 
    if(root->leaf == true && root->numLeaves == this->maxLeaves){
        splitRoot(this->root, p);
        return;
    }
    else{
    //search for where item goes;
	    while(n->leaf == false){
	      for(i = 0; i < (n->numKeys); i++){
	          if(a.compare(n->keys[i]) < 0){
	       		break;  
	          }
	          if(a.compare(n->keys[n->numKeys-1]) > 0){
	            i = n->numKeys;
	            break;
	          }
	      }
	      indexToLeaf = i;
	      n = n->childPtr[i];
	  	}
	}
   insertLeaf(n, p);
}


void BTree::insertLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex){

	if(n->numLeaves == this->maxLeaves){
		splitLeaf(n, p, leafIndex);
	}
	else{
	    n->leaves[n->numLeaves] = *p;
	    ++(n->numLeaves);
	    sort(n->leaves, n->numLeaves);
	}
}

void BTree::splitRoot(BTreeNode* r, BTreeNode::Person* p){
    BTreeNode* tmp = new BTreeNode();
  
    tmp->childPtr[0] = new BTreeNode();
    tmp->childPtr[0]->leaf = true;
    
    tmp->childPtr[1] = new BTreeNode();
    tmp->childPtr[1]->leaf = true;

    tmp->childPtr[0]->next = tmp->childPtr[1];
    tmp->childPtr[1]->next = nullptr;


    int midLeaf = (r->numLeaves)/2;
    BTreeNode::Person mid = r->leaves[midLeaf];
    //if p>mid its goes right. Less, it goes left
    //p comes before mid
    if(p->name.compare(mid.name) < 0){
    	cout << "1 entered\n" << endl;
        for(int i = 0; i <= midLeaf; i++){
            tmp->childPtr[0]->leaves[i] = r->leaves[i];
            tmp->childPtr[1]->leaves[i] = r->leaves[i+midLeaf];
            ++(tmp->childPtr[0]->numLeaves);
            ++(tmp->childPtr[1]->numLeaves);
        }
        tmp->childPtr[0]->leaves[midLeaf] = *p;
    }
    //p is after mid
    else{
    	cout << "else entered" << endl;
        for(int i = 0; i <= midLeaf; i++){
            tmp->childPtr[0]->leaves[i] = r->leaves[i];
            tmp->childPtr[1]->leaves[i] = r->leaves[i+midLeaf];
            ++(tmp->childPtr[0]->numLeaves);
            ++(tmp->childPtr[1]->numLeaves);

        }
        tmp->childPtr[1]->leaves[0] = *p;
    }
//    sort(tmp->childPtr[0]->leaves, tmp->childPtr[0]->numLeaves);
    sort(tmp->childPtr[1]->leaves, tmp->childPtr[1]->numLeaves);
    tmp->keys[0] = tmp->childPtr[1]->leaves[0].name;
    tmp->numChildren = 2;
    tmp->numKeys = 1;
    cout << "split\n" << endl;
    this->root = tmp;
   // print();
}

//no need for nlogn sort. Sorting 3 or less items
//should use a better sort if scaling
//bubble sort?
void BTree::sort(Person *leaves, int numLeaves){
	
	Person tmp;
	for(int i = 0; i < numLeaves; i++){
		for(int j = i; j < numLeaves; i++){
			if(leaves[i].name.compare(leaves[j].name) > 0){
				tmp = leaves[i];
				leaves[i] = leaves[j];
				leaves[j] = tmp;
			}
		}
	}
}


void BTree::print(){
    print(root);
}

void BTree::print(BTreeNode *n){
	//BTreeNode *n = root;
	if(n == nullptr){
		return;
	}
	if(n->leaf == false){
		for(int i = 0; i < n->numChildren; i++){
			print(n->childPtr[i]);
		}
	}
	if(n->leaf == true){
		for(int i =0; i < n->numLeaves; i++){
			cout << n->leaves[i].name << " and dataptr " << n->leaves[i].dataPtr << endl;
 		}
	}
}



