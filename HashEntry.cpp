#include "HashEntry.h"
#include <iostream>
#include <string>
using namespace std;

void HashEntry::printFriends(){

	for(list<string>::const_iterator it = this->friends.begin(); it != this->friends.end(); ++it){
        cout << "," << *it;
    }
}