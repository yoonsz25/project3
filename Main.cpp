#include "BTree.h"
#include "AdList.h"
#include <sstream>
#include <fstream>
#include <iostream>


using namespace std;

vector<string> split(string str, char delimiter)
{
 vector<string> internal;
 stringstream ss(str);       // turn the string into a stream.
 string tok="";
 while(getline(ss, tok, delimiter))
 {
 	internal.push_back(tok);
 }

 return internal;
}


//To not overwrite what is file already, use r+ instead of w for fopen.
int main()
{
 	AdList* pointerA = new AdList();
 	AdList a = *(pointerA);
 	ifstream f;
 	FILE *pFile;
 	const char *profileName = "ProfileData.txt";
 	f.open("inputs/Generated1.txt", ios::in);

 	if(!f) 
 		cerr << "File not found" << endl;
 	else
 	{
 		string line;
 		pFile = fopen(profileName, "w");
 		while(std::getline(f, line))
 		{
 			vector<string> words = split(line, ',');
 			a.insertData(words, pFile);
 		}
		 fclose(pFile);
	}
	BTree *t = new BTree();
	HashEntry h;
	int count = 0;
	for(int i = 0; i < a.getSize(); i++){
		h = a.get(i);
		if(h.getName().compare("!") != 0){
			cout << "    "<<count<<": "  << h.getName() << endl;
			t->insert(h.getName(), h.getDataPtr());
			++count;
			//t->listPrint();
			if(count >49)
				t->print();
			cout <<"\n\n";
		}
		if(count == 100)
			break;
	}
	//everythin should not be inserted
	pFile = fopen(profileName, "r+");
//	a.printAll(pFile);
	fclose(pFile);


	return 0;
} 