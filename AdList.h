#ifndef __ADLIST_H__
#define __ADLIST_H__

#include <string>
#include <list>
#include <vector>
#include <stdio.h>
#include "HashEntry.h"

class AdList{
	private:
		
		HashEntry *arr;
		int count;
		int TABLE_SIZE;

		void insertHash(std::string name);

	public:
		AdList();
		int hash(std::string str, int seed=0);
		void insertData(std::vector<std::string>, FILE *pFile);
		void writeToFile(FILE *pFile, std::string name, std::string age, std::string occupation);
	
		void addFriend(std::string &name, std::string &nameFriend); //One friend
		void updateFriend(std::string a, std::string b); //updates both friends' list
		bool friendship(std::string a, std::string b); //true if a is a friend of b
	
		void print();
		void printSingle(std::string name);
		void printAll();
		void ListFriendsInfo(std::string name);
		int getHash(std::string name);
		
		HashEntry get(int i);
		int getSize();
		int getCount();
};

#endif