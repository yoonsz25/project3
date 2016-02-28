#include <string>
#include <list>


class AdList{
	private:
		struct HashEntry{
			std::string name;
			int dataPointer;
			std::list<std::string> friends;
		};
		HashEntry *arr;
		int count;
		int TABLE_SIZE;

		void insertHash(std::string name);

	public:
		AdList();
		int hash(std::string str, int seed=0);
		void insertData(std::string line);
		void addFriend(std::string name, std::string nameFriend); //One friend
		void updateFriend(std::string a, std::string b) //updates both friends' list
		void print();
}