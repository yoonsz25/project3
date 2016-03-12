#include "BTree.h"
#include "BTreeNode.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cassert>

using namespace std;


 //nullptr evaluates to false in boolean operations

BTree::BTree(){
    root = new BTreeNode();
    this->maxChild = 5;         //min 3, max 5
    this->maxLeaves = 3;        //min 2, max 3
    root->leaf = true;
}

/*
BTreeNode::Person BTree::find(BTreeNode* n, string a){
    if(n==NULL){
        return NULL;
    }
    if(n->leaf == false){
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
    if(n->leaf == true){
        for(int i = 0; i<n->numLeaves; i++){
            if(a.compare(n->leaves[i].name)==0)
                return n->leaves[i];
        }
        return NULL;
    }
}
*/

void BTree::split(BTreeNode* n, BTreeNode::Person* p, int index){
    assert(n->numChildren == 5);
    if(n->parent!=NULL && n->parent->numChildren == 5){
        for(int j = 0; j < 5; j++){
            if(n->parent->childPtr[j] == n){
                //call split(n->parent, "string new key that should be inserted", int index);
                break;
            }
        }
    }
    BTreeNode* newOne = new BTreeNode();
    for(int i = 0; i < 3; i++){
        newOne->childPtr[i] = n->childPtr[i+2];
        if(i<2)
            newOne->keys[i] = n->keys[i+2];
    }
    newOne->numChildren = 3;
    newOne->numKeys = 2;
    newOne->parent = n->parent;
    //alex's code
    for(int i = n->parent->numChildren; i>index; i--){
		n->parent->childPtr[i+1] = n->parent->childPtr[i];
	}
	++(n->parent->numChildren);
	n->parent->childPtr[i+1] = newOne;
	//update keys
	for(int i = n->numKeys; i>index; i--){
		n->parent->keys[i+1] = n->parent->keys[i];
	}
	n->parent->keys[index] = newOne->leaves[1].name;
	++(n->parent->numKeys);
	for(int i = 0; i <numKeys; i++)
	    n->parent->keys[i] = n->parent->childPtr[i+1]->childPtr[0]->leaves[0].name;
	
    for(int i = 2; i < 4; i++){
        n->keys[i] = "?";
        n->childPtr[i+1] = NULL;
    }
    n->numChildren = 3;
    n->numKeys = 2;
    
    if(n->childPtr[2]->leaves[1].compare(p.name)>0 && index != 2){
        //* n = leaf(node) which is being split
        //p = person to insert
        // leafIndex = index into leaf from parent
        n->childPtr[2] = NULL;
        n->numChildren--;
        n->keys[1] = "?";
        n->numKeys--;
        splitLeaf(n->childPtr[index], p, index);
    }
    else if(index!=2 && n->childPtr[2]->leaves[1].compare(p.name)<0){
        newOne>childPtr[0] = NULL;
        newOne->numChildren--;
        newOne->keys[1] = "?";
        newOne->numKeys--;
        splitLeaf(newOne->childPtr[index-2],p,index-2);
    }
    if(index==2){
        
    }
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
    int x = 0;

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
	            i = n->numKeys-1;
	            break;
	          }
	      }
	      indexToLeaf = i;
	      n = n->childPtr[i];	      
	  	}
	}
   insertLeaf(n, p, indexToLeaf);
}

void BTree::splitRoot(BTreeNode* r, BTreeNode::Person* p){
    BTreeNode* tmp = new BTreeNode();
  
    tmp->childPtr[0] = new BTreeNode();
    tmp->childPtr[0]->leaf = true;
    
    tmp->childPtr[1] = new BTreeNode();
    tmp->childPtr[1]->leaf = true;

    tmp->childPtr[0]->next = tmp->childPtr[1];
    tmp->childPtr[1]->next = nullptr;

    tmp->childPtr[0]->parent = tmp;
    tmp->childPtr[1]->parent = tmp;


    int midLeaf = (r->numLeaves)/2;
    BTreeNode::Person mid = r->leaves[midLeaf];
    //if p>mid its goes right. Less, it goes left
    //p comes before mid
    assert(p->name.compare(mid.name) != 0);
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
    sort(tmp->childPtr[0]->leaves, tmp->childPtr[0]->numLeaves);
    sort(tmp->childPtr[1]->leaves, tmp->childPtr[1]->numLeaves);
    tmp->keys[0] = tmp->childPtr[1]->leaves[0].name;
    tmp->numChildren = 2;
    tmp->numKeys = 1;
    cout << "split\n" << endl;
    this->root = tmp;
   // print();
}

/* n = leaf(node) which is being split
*  p = person to insert
*  leafIndex = index into leaf from parent
*/
void BTree::splitLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex){
	assert(n->numLeaves == this->maxLeaves);
	cout << n->parent->keys[0]<< "\n"<<endl;
	if(n->parent->numChildren == this->maxChild){
		cout<<"write node spliting algorithm" << endl;
		return;
	}
	BTreeNode *upperHalf = new BTreeNode();
	upperHalf->leaf = true;
	upperHalf->parent = n->parent;

	int midLeaf = 1;
 	BTreeNode::Person mid = n->leaves[2];


 	for(int i = 0; i <= midLeaf; i++){
            upperHalf->leaves[i] = n->leaves[i+midLeaf];
            ++(upperHalf->numLeaves);
    }
    --(n->numLeaves);
	if(p->name.compare(mid.name) < 0){
		n->leaves[midLeaf] = *p;
		sort(n->leaves, n->numLeaves);
	}
	else{
		upperHalf->leaves[0] = *p;
		sort(upperHalf->leaves, upperHalf->numLeaves);
	}
	//make space for new leaf;
	for(int i = n->parent->numChildren; i>leafIndex; i--){
		n->parent->childPtr[i+1] = n->parent->childPtr[i];
	}
	++(n->parent->numChildren);
	n->parent->childPtr[leafIndex+1] = upperHalf;
	//update keys
	for(int i = n->numKeys; i>leafIndex; i--){
		n->parent->keys[i+1] = n->parent->keys[i];
	}

	n->parent->keys[leafIndex] = upperHalf->leaves[1].name;
	++(n->parent->numKeys);

}

void BTree::insertLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex){
	if(n->numLeaves == this->maxLeaves){
	    //trying split function, if it doesn't work do splitLeaf
		splitLeaf(n, p, leafIndex);
	}
	else{
	    n->leaves[n->numLeaves] = *p;
	    ++(n->numLeaves);
	    sort(n->leaves, n->numLeaves);
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
    cout << n->numLeaves << endl;
    for(int i =0; i < n->numLeaves; i++){
      cout << n->leaves[i].name << " and dataptr " << n->leaves[i].dataPtr << endl;
    }
  }
}