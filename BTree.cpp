#include "BTree.h"
#include "BTreeNode.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cassert>

using namespace std;

string BTree::smallestName(BTreeNode* n){
    if(n->leaf)
        return n->leaves[0].name;
    return smallestName(n->childPtr[0]);
}

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

//n is the full internal node that needs to be split
void BTree::splitNode(BTreeNode* n, BTreeNode::Person *p, int leafIndex){
    BTreeNode* first = n;
    //create new internal node. right above leaves or no?
    BTreeNode* second = new BTreeNode();
    //move latter half of n's data into new internal node.
    if(first->childPtr[0]->leaf){
        first = new BTreeNode(*n);
        splitLeafHelper(first->childPtr[leafIndex],p,leafIndex);
        //split first into first and second.
        for(int i = 3; i < 6; i++){
            if(i<5){
                second->keys[i-3] = first->keys[i];
                second->numKeys++;
                first->numKeys--;
            }
            //first is not size 6 yet...
            second->childPtr[i-3] = first->childPtr[i];
            second->numChildren++;
            first->childPtr[i] = NULL;
            first->numChildren--;
            //seg fault
            second->childPtr[i-3]->parent = second;
        }
        first->numKeys = 2;
        /*
        if(!first->childPtr[0]->leaf){
            first->childPtr[leafIndex+1] = first->childPtr[leafIndex];
            first->numChildren++;
        }*/
    }
    //fill out first up to six children
    else if (!first->childPtr[0]->leaf){
        for(int i = 3; i < 6; i++){
            if(i<5){
                second->keys[i-3] = first->keys[i];
                second->numKeys++;
                first->numKeys--;
            }
            //first is not size 6 yet...
            second->childPtr[i-3] = first->childPtr[i];
            second->numChildren++;
            first->childPtr[i] = NULL;
            first->numChildren--;
            //seg fault
            second->childPtr[i-3]->parent = second;
        }
        first->numKeys = 2;
        /*
        for(int i = first->numChildren-1; i > leafIndex; i--){
            if(i!=0)
                first->keys[i] = first->keys[i-1];
            first->childPtr[i+1] = first->childPtr[i];
        }
        first->keys[leafIndex] = p->name;
    //new Childptr assignment is after second is made. Along with numChildren++
        //first->childPtr[leafIndex+1] = first->childPtr[leafIndex];
        //first->numChildren++;
        first->numKeys++;
        first->childPtr[leafIndex+1] = first->childPtr[leafIndex];
        first->numChildren++;*/
    }
    
    //if n's parent is full -> recursive call.
    if(first->parent != NULL && first->parent->numChildren == 5){
        cout << "\n\nrecursion is called\n\n";
        //six algorithm
        BTreeNode::Person* newKey = new BTreeNode::Person();
        for(int i = 0; i < 5; i++){
            if(smallestName(first).compare(smallestName(first->parent->childPtr[i]))==0){
                newKey->name = smallestName(second);
                BTreeNode* top = new BTreeNode(*first->parent);
                for(int j = top->numChildren-1; j> i; j--){
                    if(j>0)
                        top->keys[j] = top->keys[j-1];
                    top->childPtr[j+1] = top->childPtr[j];
                }
                top->keys[i] = newKey->name;
                top->numKeys++;
                top->childPtr[i]= first;
                top->childPtr[i+1] = second;
                first->parent = top;
                second->parent = top;
                top->numChildren++;
                if(top->parent == NULL){
                    root=top;
                }
                splitNode(top,newKey,i);
                return;
            }
        }
    }
    //if n was root, make a new root.
    if(n == root){
        root = new BTreeNode();
        root->keys[0] = smallestName(second);
        root->numKeys++;
        root->childPtr[0] = first;
        root->childPtr[1] = second;
        root->numChildren=2;
        first->parent = root;
        second->parent = root;
    }
    //else squeeze in new internal node.
    else{
        BTreeNode* top = first->parent;
        int i = top->numChildren-1;
        while(smallestName(first).compare(smallestName(top->childPtr[i]))!=0){
            if(i!=0)
                top->keys[i] = top->keys[i-1];
            top->childPtr[i+1] = top->childPtr[i];
            i--;
        }
        top->childPtr[i] = first;
        top->childPtr[i+1] = second;
        second->parent = top;
        top->keys[i] = smallestName(second);
        top->numKeys++;
        top->numChildren++;
    }
}
/*
BTreeNode* BTree::make6(BTreeNode* full, BTreeNode::Person *p, int index){
    assert(full->numChildren == 5);
    BTreeNode *six = new BTreeNode(*full);
    splitLeafHelper(full->childPtr[index], p, index);
	BTreeNode *left = new BTreeNode();
	BTreeNode *right = new BTreeNode();
	BTreeNode *parent = new BTreeNode();
	left->parent = parent;
	right->parent = parent;
	
	//move childs to correct location
	for(int i = 0; i < 3; i++){
	    left->childPtr[i] = six->childPtr[i];
	    left->numChildren++;
	    left->childPtr[i]->parent = parent;
	    right->childPtr[i] = six->childPtr[i+3];
	    right->numChildren++;
	    right->childPtr[i]->parent = parent;
	}
	//move keys to left and right
	for(int i = 0; i < 2; i++){
	    left->keys[i] = six->keys[i];
	    left->numKeys++;
	    right->keys[i] = six->keys[i+3];
	    right->numKeys++;
	}
	
	//update parent
	parent->childPtr[0] = left;
	parent->childPtr[1] = right;
	parent->keys[0] = six->keys[2];
	return parent;
}
*/
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
}
/* n = leaf(node) which is being split
*  p = person to insert
*  leafIndex = index into leaf from parent
*/
void BTree::splitLeaf(BTreeNode *n, BTreeNode::Person *p, int leafIndex){
	assert(n->numLeaves == this->maxLeaves);
	if(n->parent->numChildren == 5){
        splitNode(n->parent,p,leafIndex);
		return;
	}
	else{
	    splitLeafHelper(n, p, leafIndex);
	}
	
}
void BTree::splitLeafHelper(BTreeNode *n, BTreeNode::Person *p, int leafIndex){
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
		n->parent->childPtr[i] = n->parent->childPtr[i-1];
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
     cout << "number of leaves in this node " << n->numLeaves<< endl;
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
	if(!root->leaf)
	    printInternalNode(tmp);
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
				/*
				char cname[20];
                char age[3];
                char occupation[30];
                FILE *pFile;
                pFile = fopen("ProfileData.txt", "r");
                fseek(pFile, offset, SEEK_SET);
                fgets(cname, 20, pFile);
                fseek(pFile, (20+offset), SEEK_SET);
                fgets(age, 3, pFile);
                fseek(pFile, (23+offset), SEEK_SET);
                fgets(occupation, 30, pFile);
                cout << cname << "," << age << "," << occupation;
                fclose(pFile);
				*/
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



