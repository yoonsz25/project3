/*
 * Main.cpp
 *
 *  Created on: Feb 11, 2016
 *      Author: omid
 */
#include "AdList.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>

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

int main()
{
	AdList* pointerA = new AdList();
	AdList a = *(pointerA);
	ifstream f;
	f.open("Generated1.txt", ios::in);
	if(!f) cerr << "File not found" << endl;
	else
	{
		string line;
		const char *profileName = "ProfileData.txt";
		FILE *pFile;
		pFile = fopen(profileName, "w");
		while(std::getline(f, line))
		{
			vector<string> words = split(line, ',');
			// ... TO DO ...
			a.insertData(words, pFile);
			
		}
		 fclose(pFile);

		a.print();
	}
	return 0;
}