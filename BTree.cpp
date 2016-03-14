#include "BTree.h"
#include "BTreeNode.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cassert>

using namespace std;


 //nullptr evaluates to false in boolean operations

void BTree::printInternalNode(BTreeNode* n){
    if(!n->leaf){
        cout << "Is this a copied node? (if 6, true): " << n->maxChild << endl;
        cout << "These are the keys: " << n->numKeys << endl;
        for(int i=0; i<n->numKeys; i++){
            cout << n->keys[i] << ", ";
        }
        cout << "\nPointers present: " << n->numChildren << endl;
        for(int i = 0; i < n->numChildren; i++){
            if(n->childPtr[i] != nullptr){
                cout << "present ";
            }
        }
        cout <<"\n";
    }
    return;
}

BTree::BTree(){
    root = new BTreeNode();
    this->maxLeaves = 3;        //min 2, max 3
    root->leaf = true;
}

void BTree::split(BTreeNode* n, BTreeNode::Person* p, int index){
    //base case = parent is null
    if(n == nullptr){
        BTreeNode* origin = new BTreeNode();
        origin->childPtr[0] = root;
        root = origin;
        origin = root->childPtr[0];
        origin->parent = root;
        
        BTreeNode* newOne = new BTreeNode();
        root->childPtr[1] = newOne;
        newOne->parent = root;
        for(int i = 0; i<3; i++){
            newOne->childPtr[i] = origin->childPtr[i+3];
            newOne->childPtr[i]->parent = newOne;
            if(i<2)
                newOne->keys[i] = origin->keys[i+3];
        }
        
        newOne->numChildren=3;
        newOne->numKeys=2;
        root->numChildren = 2;
        root->keys[0] = newOne->childPtr[0]->leaves[0].name;
        root->numKeys = 1;
        
        //update origin
        BTreeNode* replacement = new BTreeNode();
        for(int i = 0; i <3; i++){
            replacement->childPtr[i] = origin->childPtr[i];
            replacement->childPtr[i]->parent = replacement;
        }
        for(int i =0; i<2; i++){
            replacement->keys[i] = origin->keys[i];
        }
        replacement->parent = root;
        replacement->numChildren = 3;
        replacement->numKeys = 2;
        root->childPtr[0] = replacement;
        //delete origin;
        return;
    }
    //base case = parent has room to take in.
    if(n->numChildren<5){
        cout << "Base case of parent not full" << endl;
        //code
        BTreeNode* origin = n->childPtr[index];
        for(int i = n->numChildren; i>index; i--){
            n->childPtr[i] = n->childPtr[i+1];
            if(i<n->numKeys)
                n->keys[i] = n->keys[i+1];
        }
        n->numChildren++;
        n->numKeys++;
        
        BTreeNode* newOne = new BTreeNode();
        newOne->parent = n;
        for(int i = 0; i<3; i++){
            newOne->childPtr[i] = origin->childPtr[i+3];
            if(i<2)
                newOne->keys[i] = origin->keys[i+3];
        }
        newOne->numChildren=3;
        newOne->numKeys=2;
        n->childPtr[index+1] = newOne;
        for(BTreeNode* tmp = newOne; newOne!=NULL; tmp=tmp->childPtr[0]){
            if(tmp->leaf)
                n->keys[index+1] = tmp->leaves[0].name;
        }
        
        BTreeNode* replacement = new BTreeNode();
        for(int i = 0; i <3; i++){
            replacement->childPtr[i] = origin->childPtr[i];
            if(i<2)
                replacement->keys[i] = origin->keys[i];
        }
        replacement->parent = n;
        replacement->numChildren = 3;
        replacement->numKeys = 2;
        n->childPtr[index] = replacement;
        //delete origin;
        return;
    }
    
    //recursive step
    assert(n->numChildren >= 5);
    cout << "recursion step" << endl;
    cout << index << endl;
    BTreeNode* copy = new BTreeNode(*n);
    if(copy->childPtr[index]->leaf){
        splitLeaf(copy->childPtr[index], p, index);
    }
    if(copy->parent == NULL){
        //delete n;
        root = copy;
        split(nullptr,p,0);
        return;
    }
    if(copy->parent->numChildren == 5){
        for(int j = 0; j < 5; j++){
            if(n->parent->childPtr[j] == n){
                //delete n;
                split(n->parent, p, j);
                break;
            }
        }
    }
    else{
        //split up internal node right above leaf. and update the parent.
        cout << "Else recursion" << endl;
        BTreeNode* origin = copy;
        n=copy->parent;
        int nToCopy = 0;
        for(; nToCopy < n->numChildren; nToCopy++){
            if(n->childPtr[nToCopy]->leaves[0].name.compare(copy->leaves[0].name)==0){
                break;
            }
        }
        for(int i = n->numChildren; i>nToCopy; i--){
            n->childPtr[i] = n->childPtr[i-1];
        }
        for(int i = n->numKeys; i>nToCopy;i--){
            n->keys[i] = n->keys[i-1];
        }
        n->numChildren++;
        n->numKeys++;    
        BTreeNode* newOne = new BTreeNode();
        newOne->parent = n;
        for(int i = 0; i<3; i++){
            newOne->childPtr[i] = origin->childPtr[i+3];
            newOne->childPtr[i]->parent = newOne;
            if(i<2)
                newOne->keys[i] = origin->keys[i+3];
        }
        newOne->numChildren=3;
        newOne->numKeys=2;
        n->childPtr[nToCopy+1] = newOne;
        n->keys[nToCopy+1] = newOne->leaves[0].name;

        BTreeNode* replacement = new BTreeNode();
        for(int i = 0; i <3; i++){
            replacement->childPtr[i] = origin->childPtr[i];
            replacement->childPtr[i]->parent = replacement;
            if(i<2)
                replacement->keys[i] = origin->keys[i];
        }
        replacement->parent = n;
        replacement->numChildren = 3;
        replacement->numKeys = 2;
        n->childPtr[index] = replacement;
        string newKey = "";
        for(BTreeNode* t = newOne->childPtr[0]; t!=NULL; t = t->childPtr[0]){
            if(t->leaf)
                newKey = t->leaves[0].name;
        }
        n->keys[nToCopy] = newKey;
        //update n's keys[]
        /*
        for(int i = n->numKeys-1; i>nToCopy; i--){
            cout << n->keys[i-1] << endl;
            n->keys[i] = n->keys[i-1];
        }
        cout << "This works" << endl;
        for(BTreeNode* t = newOne->childPtr[0]; t!=NULL; t = t->childPtr[0]){
            if(t->leaf)
                n->keys[nToCopy] = t->leaves[0].name;
        }
        */
        
        BTreeNode *t;
        
        //each key moves over one, to include a new key.
        //new key is the smallest value from newOne
        //and indexes nToCopy+1 ~ numKeys-1 moves over +1
        printInternalNode(n);
        for(int i =0; i<n->numKeys; i++){
            t = n->childPtr[i+1];
            while(!t->leaf){
                t = t->childPtr[0];
            }
            n->keys[i] = t->leaves[0].name;
        }
        
        printInternalNode(n);
        
        return;
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
	            i = n->numChildren-1;
	            break;
	          }
	      }
	      indexToLeaf = i;
	      n = n->childPtr[i];	      
	  	}
	}
   insertLeaf(n, p, indexToLeaf);
}
/*
void BTree::split(BTreeNode* n, string key, int index){
    
}
*/
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
    this->root = tmp;
   // print();
}
/* n = leaf(node) which is being split
*  p = person to insert
*  leafIndex = index into leaf from parent
*/
void BTree::splitLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex){
	assert(n->numLeaves == this->maxLeaves);
	if(n->parent->numChildren == n->parent->maxChild){
        split(n->parent, p, leafIndex);
		return;
	}
	BTreeNode *upperHalf = new BTreeNode();
	upperHalf->leaf = true;
	upperHalf->parent = n->parent;

	int midLeaf = 1;
 	BTreeNode::Person mid = n->leaves[1];

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
	++(n->parent->numKeys);
	for(int i = 0; i < n->parent->numKeys; i++){
        n->parent->keys[i] = n->parent->childPtr[i+1]->leaves[0].name;
	}
	//update next pointers in leaves
	if(leafIndex == n->parent->numChildren-2)
	    n->parent->childPtr[n->parent->numChildren-1]->next = n->parent->childPtr[n->parent->numChildren-2]->next;
	for(int i = 0; i < n->parent->numChildren-1; i++){
		n->parent->childPtr[i]->next = n->parent->childPtr[i+1];
	}
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
     //cout << "number of leaves in this node " << n->numLeaves<< endl;
    for(int i =0; i < n->numLeaves; i++){
      cout << n->leaves[i].name << " and dataptr " << n->leaves[i].dataPtr << endl;
    }
  }
}









void BTree::listPrint(){
	BTreeNode* tmp = this->root;
	while(tmp->leaf == false){
		tmp = tmp->childPtr[0];
	}
	cout << "       First node is: \n";
	assert(tmp->leaf == true);
	while(tmp != nullptr){
	    cout << "\nNew Node with size " << tmp->numLeaves << ": \n";
		for(int i = 0; i < tmp->numLeaves; i++){
      		cout << tmp->leaves[i].name << " and dataptr " << tmp->leaves[i].dataPtr << endl;
		}
		if(tmp->next != NULL && tmp->parent != tmp->next->parent){
		    cout << "\n     next internal node is used." << endl;
		    printInternalNode(tmp->next->parent);
		}
		tmp = tmp->next;
	}
}


BTreeNode::Person BTree::find(string name){
	BTreeNode *leafNode = find(this->root, name);
	assert(leafNode->leaf == true);

	for(int i = 0; i < leafNode->numLeaves; i++){
		if(leafNode->leaves[i].name.compare(name) == 0){
			return leafNode->leaves[i];
			}
	}
	cout << "name not found in Btree. " << endl;
	BTreeNode::Person p;
	return p;
}


/*returns the node containing leaves where name should be. 
	- Name does not have to be inside that node. Just returns where the name should be.
* n = node in the Btree
* name = name of the person we are finding.
*/
BTreeNode* BTree::find(BTreeNode* n, string name){

	int i = 0;
	BTreeNode *tmp = n;
	while(tmp->leaf == false){
		for(i = 0; i < tmp->numKeys; i++){
		 if(name.compare(tmp->keys[i]) < 0){
       		break;  
          }
          if(name.compare(tmp->keys[tmp->numKeys-1]) > 0){
            i = n->numKeys-1;
            break;
          }
		}
		tmp = tmp->childPtr[i];
	}
	assert(tmp->leaf == true);
	return tmp;
}

//prints out all names firstPerson < x < lastPerson
void BTree::RangeQuery(string firstPerson, string lastPerson){
	BTreeNode *first = find(this->root, firstPerson);
	cout << "write the code which prints out all the information about the people. Requires the file pointer" << endl;
	
	int i;

	for(i = 0; i < first->numLeaves; i++){
		if(firstPerson.compare(first->leaves[i].name) <= 0){
			break;
		}
	}

	while(first != nullptr){
		for(; i < first->numLeaves; i++){
			if((firstPerson.compare(first->leaves[i].name) <= 0) && 
							(lastPerson.compare(first->leaves[i].name) >= 0)) {
				cout << first->leaves[i].name << " and dataptr  " << first->leaves[i].dataPtr << endl;
			}
			else{
				//return because our current person is not between the 2 parameters
				return;
			}
		}
		i = 0;
		first = first->next;
	}
}



