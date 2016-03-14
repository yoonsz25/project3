#ifndef __HASHENTRY_H__
#define __HASHENTRY_H__

#include <string>
#include <list>
#include <vector>


class HashEntry{
	friend class AdList;
	private:
		std::string name;
		int dataPointer;
		std::list<std::string> friends;
	public:
		HashEntry():name("!") , dataPointer(-1) {};
		std::string getName(){return this->name;}
		int getDataPtr(){return this->dataPointer;}
		std::list<std::string> getList(){return this->friends;}
		void printFriends();
		std::vector<std::string> getFriends();

};
#endif
