 #include "BTree.h"
 #include "AdList.h"
 #include <sstream>
 #include <fstream>
 #include <iostream>
 #include <list>
  
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
 
 void insertFile(ofstream &f, list<string> s, string name){
 	for(list<string>::const_iterator it = s.begin(); it != s.end(); it++){
        f << name << ",";
        if((*it).compare("") == 0)
			f << name << endl;
        else
        	f << *it << endl;
    }
 }

 //To not overwrite what is file already, use r+ instead of w for fopen.
  int main(int argc, char **argv)
  {
  		const char *profileName = "ProfileData.txt";
  		const char *input = "inputs/Generated1.txt";
  		if(argc == 3){
  			input = argv[1];
  			profileName = argv[2];

  		}
  		else if(argc != 1){
  			cerr << "Requires 0 inputs. " << endl;
  			cerr <<"Or: input file and output file (arg 1 and arg 2 respectively)" << endl;
  			exit(1);
  		}
  		
	 	AdList* pointerA = new AdList();
	 	AdList a = *(pointerA);
	 	ifstream f;
	 	FILE *pFile;
	 	
	 	f.open(input, ios::in);

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



		ofstream myfile;
		BTree *t = new BTree();
		HashEntry h;
		for(int i = 0; i < a.getSize(); i++){
			h = a.get(i);
			if(h.getName().compare("!") != 0){
				t->insert(h.getName(), h.getDataPtr());

			}

		}
		//t->listPrint();
		//t->RangeQuery("Alex", "John");
	try{
		while(true)
		{
			string str;
			cin >> str;
			if(cin.eof())
			{
				break;
			}
			if(str.compare("exit") == 0)
			{
				break;
			}
			else if(str.compare("updateedges") == 0)
			{
				myfile.open("data.txt");
				HashEntry h;
				for(int i = 0; i < a.getSize(); i++){
					h = a.get(i);
					if(h.getName().compare("!") != 0)
						insertFile(myfile, h.getList(), h.getName());
				}
				myfile.close();
			}
			else if(str.compare("insert") == 0)
			{
				cout << "Format: name age occupation" << endl;
				string age;
				string name;
				string occupation;	
				cin >> name >> age >> occupation;
				vector<string> info;
				info.push_back(name);
				info.push_back(age);
				info.push_back(occupation);
		
				pFile = fopen(profileName, "r+");
				a.insertData(info, pFile);
				fclose(pFile);
				t->insert(name, a.getCount());
			}
			else if(str.compare("addfriend") == 0)
			{
				string friend1, friend2;
				cin >> friend1 >> friend2;
				a.updateFriend(friend1, friend2);
			}
			else if(str.compare("printBTree") == 0){
				t->print();
			}
			else if(str.compare("listBTree") == 0){
				t->listPrint();
			}
			else if(str.compare("printAll") == 0)
			{
				a.printAll();
			}
			else if(str.compare("printsingle") == 0)
			{
				string name;
				cin >> name;
				a.printSingle(name);
				cout << endl;
			}
			else if(str.compare("RangeQuery") == 0)
			{
				string start, end;
				cin >> start >> end;
				t->RangeQuery(start,end);
			}
			else if(str.compare("ListFriendsInfo") == 0)
			{
				string name;
				cin >> name;
				a.ListFriendsInfo(name);
				cout << "done" << endl;
			}
			else if(str.compare("graph") == 0){
				system("Rscript graph.R");
    			system("open mygraphic.png");
			}
			else
			{
				cin.clear();
				cout << "Inputed string format was incorrect" << endl;
			}
			
		}
	}
	catch(exception& ex)
	{
		cerr << ex.what() << endl;
	}
		//a.printAll();
		//a.print();


		return 0;
	} 