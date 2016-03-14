#include "AdList.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <cassert>


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
int AdList::hash(string str, int seed){
    int hash = seed;                    
    for(int i =0; i<str.length(); i++)
        hash = (hash*101 + str[i])%TABLE_SIZE;
    return hash;
}

void AdList::insertHash(string name){
    int index = hash(name);
    int i = 1;
    while(arr[index].name.compare("!") != 0){
       // cout << arr[index].name.compare(name) << endl;
        index = hash(name,i);
        i++;
    }
    arr[index].name = name;
    arr[index].dataPointer = count;
}

int AdList::getHash(string name){
    int index = hash(name);
    int i = 0;
    while(arr[index].name.compare(name)!=0){
        index = hash(name,i++);
    }
    return index;
}
/*
    input[0]  == name of person
    input[1]  == age of person
    input[2]  == occupation of person
    input[3+] == friends of person
*/
void AdList::insertData(vector<string> input, FILE *pFile){
    insertHash(input[0]);
    writeToFile(pFile, input[0],input[1],input[2]);
    string originator = input[0];
    for(int i = 3; i < input.size(); i++){
        string friendI = input[i];
        addFriend(originator, friendI);
    }
}



void AdList::writeToFile(FILE *pFile, string name, string age, string occupation){
    const char *cName = name.c_str();
    const char *cAge = age.c_str();
    const char *cOccupation = occupation.c_str();
    int offset = this->count * 53;
    fseek(pFile, offset, SEEK_SET);     //find offset of file to start appending data
    fputs(cName, pFile);                //write name to disk
    fseek(pFile, offset+ 20, SEEK_SET);         //find spot to place age
    fputs(cAge, pFile);                 //write age to disk
    fseek(pFile, offset + 23, SEEK_SET);          //find spot to write occupation
    fputs(cOccupation, pFile);          //write occupation to disk
    ++this->count;

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
void AdList::addFriend(string &name, string &nameFriend){
    if(friendship(name, nameFriend)){
        return;
    }
    if(nameFriend.compare("") == 0)
        return;
    int index = getHash(name);
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
        if(arr[i].name.compare("!") != 0){
            cout << "Index " << i << ": " << arr[i].name << "\n"; 
            cout << "Friends:";
            list<string> s = arr[i].friends;
            cout << arr[i].name<<" "<<s.size()<< endl;
            for(list<string>::const_iterator it = s.begin(); it != s.end(); ++it){
                cout << " " << *it;
            }
            cout << endl;
        }
    }
}

void AdList::printAll(){
    for(int i =0; i < TABLE_SIZE; i++){
        if(arr[i].name.compare("!") != 0){
            printSingle(arr[i].name);
            cout << "\n"<< endl;
        }
    }
}
void AdList::printSingle(string name){
    int index = getHash(name);
    int offset = arr[index].dataPointer * 53;
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
    arr[index].printFriends();
}   

void AdList::ListFriendsInfo(string name){

    int index = getHash(name);
    vector<string> tmp = arr[index].getFriends();
    for(int i = 0; i < tmp.size(); i++){
        int friendIndex = getHash(tmp[i]);
        int offset = arr[friendIndex].dataPointer * 53;
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
        //arr[friendIndex].printFriends();
        fclose(pFile);
    }
    return;
}


HashEntry AdList::get(int i){
    if(i > this->TABLE_SIZE){
        cerr << "index out of bounds" << endl;
    }
    return this->arr[i];
}
int AdList::getSize(){
    return this->TABLE_SIZE;
}

int AdList::getCount(){
    return this->count;
}


