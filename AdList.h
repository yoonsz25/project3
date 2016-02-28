#include <string>
#include <list>

class AdList{
	private:
		struct HashEntry{
			std::string name;
			int dataLocation;
			std::list<std::string> friends;
		};
		int count;


	public:
		int hash(std::string str, int seed=0);
		void insert(std::string name);
		void addFriend(std::string name, std::string nameFriend); //One friend
}