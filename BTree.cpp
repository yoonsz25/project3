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
    this->maxChild = 5;         //min 3, max 5
    this->maxLeaves = 3;        //min 2, max 3
    root->leaf = true;
}
void BTree::split(BTreeNode* n, BTreeNode::Person* p, int index){
    assert(n->numChildren == 5);
    //split the parent.
    if(n->parent!=NULL && n->parent->numChildren == 5){
        for(int j = 0; j < 5; j++){
            if(n->parent->childPtr[j] == n){
                //call split(n->parent, "string new key that should be inserted", int index); overload
                break;
            }
        }
    }
    
    //create the newOne that'll be the new leaf
    BTreeNode* newOne = new BTreeNode();
    for(int i = 0; i < 3; i++){
        newOne->childPtr[i] = n->childPtr[i+2];
        if(i<2)
            newOne->keys[i] = n->keys[i+2];
    }
    newOne->numChildren = 3;
    newOne->numKeys = 2;
    newOne->parent = n->parent;
    
    //update the parent
    if(n->parent != NULL){
        for(int i = n->parent->numChildren; i>index; i--){
    		n->parent->childPtr[i+1] = n->parent->childPtr[i];
        }
    	++(n->parent->numChildren);
    	n->parent->childPtr[index+1] = newOne;
    	//update keys
	    for(int i = n->numKeys; i>index; i--){
	    	n->parent->keys[i+1] = n->parent->keys[i];
	    }
	    n->parent->keys[index] = newOne->leaves[1].name;
	    ++(n->parent->numKeys);
	    for(int i = 0; i <n->parent->numKeys; i++)
	    n->parent->keys[i] = n->parent->childPtr[i+1]->childPtr[0]->leaves[0].name;
    }
    
    //if n is root
    if(n->parent == NULL){
        this->root = new BTreeNode();
        root->childPtr[0] = n;
        root->childPtr[1] = newOne;
        root->numChildren = 2;
        n->parent = root;
        newOne->parent = root;
    }
    
    for(int i = 2; i < 4; i++){
        n->keys[i] = "?";
        n->childPtr[i+1] = NULL;
    }
    n->numChildren = 3;
    n->numKeys = 2;
    if(n->keys[1].compare(p->name)>0){
        //* n = leaf(node) which is being split
        //p = person to insert
        // leafIndex = index into leaf from parent
        n->childPtr[2] = NULL;
        n->numChildren--;
        n->keys[1] = "?";
        n->numKeys--;
        splitLeaf(n->childPtr[index], p, index);
    }
    else if(n->keys[2].compare(p->name)<0){
        for(int i = 0; i < 2; i++){
            newOne->childPtr[i] = newOne->childPtr[i+1];
            if(i==0)
                newOne->keys[i] = newOne->keys[i+1];
        }
        newOne->childPtr[2] = NULL;
        newOne->numChildren--;
        newOne->keys[1] = "?";
        newOne->numKeys--;
        splitLeaf(newOne->childPtr[index-2],p,index-2);
    }
    else if(index==2){
        splitLeaf(n->childPtr[2],p,2);
        newOne->childPtr[0] = n->childPtr[2]->next;
        n->keys[2] = "?";
        n->numKeys--;
        n->childPtr[3] = NULL;
        n->numChildren--;
    }
    //connects last leaf node to the first leaf node of newOne
    n->childPtr[2]->next = newOne->childPtr[0];
    if(n->parent == root){
        root->keys[0] = newOne->childPtr[0]->leaves[0].name;
        root->numKeys = 1;
    }
    cout<< "first internal node: " << endl;
    printInternalNode(n);
    cout<<"Second internal node: " << endl;
    printInternalNode(newOne);
    
    cout << "New Root: " << endl;
    printInternalNode(root);
    
    cout << "\n\n\n" << "TESTING:    " << endl;
    for(BTreeNode* axis = n->childPtr[0]; axis != NULL; axis = axis->next){
        cout << "new Node: ";
        for(int i = 0; i < axis->numLeaves; i++){
            cout << axis->leaves[i].name << ", ";
        }
        cout << "\n";
    }
    cout << "\n";
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
    this->root = tmp;
   // print();
}

/* n = leaf(node) which is being split
*  p = person to insert
*  leafIndex = index into leaf from parent
*/
void BTree::splitLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex){
	assert(n->numLeaves == this->maxLeaves);
	
	if(n->parent->numChildren == this->maxChild){
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
	for(int i = 0; i < n->parent->numChildren-1; i++){
		n->parent->childPtr[i]->next = n->parent->childPtr[i+1];
	}
	n->parent->childPtr[(n->parent->numChildren)-1]->next = nullptr;
	
	cout <<"check end --\n";
	    printInternalNode(n->parent);
	    cout <<"\n";
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
	assert(tmp->leaf == true);
	while(tmp != nullptr){
		for(int i = 0; i < tmp->numLeaves; i++){
      		cout << tmp->leaves[i].name << " and dataptr " << tmp->leaves[i].dataPtr << endl;
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