#include "AdList.h"
#include <iostream>
#include <string>

using namespace std;

//constructor: name is automatically = "!"
AdList::AdList(){
    TABLE_SIZE = 211;
    count = 0;
    arr = new HashEntry[TABLE_SIZE];
    for(int i = 0; i<TABLE_SIZE;i++){
        arr[i].name = "!";
    }
}

//returns the hash index for a certain string.
int AdList::hash(string str, int seed=0){
    int hash = seed;
    for(int i =0; i<str.length(); i++)
        hash = hash*101 + str[i];
    return hash%TABLE_SIZE;
}

void AdList::insertHash(string name){
    int index = hash(name);
    for(int i = index; i < this->TABLE_SIZE; i++){
        if(arr[i].compare("!") == 0){
            index = i;
            break;
        }
        if(i == (this->TABLE_SIZE-1)){
            i = 0;
        }
    }
    
    arr[index].name = name;
    arr[index].dataPointer = count;
    cout << "index is: " << index << ", pointer to data is: " << arr[index].dataPointer << "count is: " << this->count << endl;
}

void AdList::insertData(string line){
    ++this->count;
    string name ="";
    
    insertHash(name);

}

//update's "name"'s friends list by adding nameFriend.
void AdList::addFriend(string name, string nameFriend){
    int index = hash(name);
    int i = 0;
    while(arr[index].name.compare(name)!=0){
        index = hash(name,i++);
    }
    arr[index].friends.push_back(nameFriend);
}

//friend A adds friend B to A's list and B adds A to B's list.
void AdList::updateFriend(string a, string b){
    addFriend(a,b);
    addFriend(b,a);
    return;
}

void AdLis::print(){
    
}