#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "FileValues.h"
#include <list>

using namespace std;

class FileReader
{
public :
	
	FileReader() { };
		
	void ReadFile(); //Colocar caminho como parâmetro se preciso

	FileValues GetFileValues(int index);

	ifstream file;

	string txt;


	FileValues fv;
	list<FileValues> fileValues;

};
		



