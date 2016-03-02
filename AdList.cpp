#include "AdList.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>


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

void AdList::insertData(vector<string> input, const char *profileName){
    insertHash(input[0]);
    writeToFile(profileName, input[0],input[1],input[2]);
    ++this->count;
}

void AdList::writeToFile(const char *profileName, string name, string age, string occupation){
    File *pFile;
    pFile = fopen(profileName, "w");
    fseek(pfile, 0, SEEK_END); //find end of file to start appending data
    fputs(name, pFile); //write name to disk
    fseek(pFile, 20, SEEK_SET); //find spot to place age
    fputs(age, pFile); //write age to disk
    fseek(pFile, 3, SEEK_SET); //find spot to write occupation
    fputs(occupation, pFile); //write occupation to disk
    fclose(pFile);
}
//returns true if a is a frined of b
bool AdList::friendship(string a, string b){
    int index = hash(a);
    int i = 0;
    while(arr[index].name.compare(a)!=0){
        index = hash(a,i++);
    }
    for(list<string>::iterator it = arr[index].friends.begin(); it != arr[index].friends.end(); it++){
        string check = *it;
        if (b.compare(check) == 0)
            return true;
    }
    return false;
}

//update's "name"'s friends list by adding nameFriend.
void AdList::addFriend(string name, string nameFriend){
    if(friendship(name, nameFriend)){
        return;
    }
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

void AdList::print(){
    for(int i =0; i < TABLE_SIZE; i++){
        cout << "Index " << i << ": " << arr[i].name << 
            "\nFriends: ";
        for(list<string>::iterator it = arr[i].friends.begin(); it != arr[i].friends.end(); it++){
            cout << " " << *it;
        }
        cout << "\n";
    }
}