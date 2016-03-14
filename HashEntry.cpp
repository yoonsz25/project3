#include "HashEntry.h"
#include <iostream>
#include <string>
using namespace std;


void HashEntry::printFriends(){

   list<string> s = getList();
   for(list<string>::const_iterator it = s.begin(); it != s.end(); it++){
        cout << "," << *it ;
    }
}

vector<string> HashEntry::getFriends(){
	vector<string> friends;
	list<string> s = getList();
	int count = 0;
   	for(list<string>::const_iterator it = s.begin(); it != s.end(); ++it){
        cout << count <<endl;
        count++;
        friends.push_back(*it);
    }
    return friends;
}
