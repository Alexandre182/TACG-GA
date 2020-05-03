#include "FileReader.h"


void FileReader::ReadFile()
{
	file.open("SceneConfig.txt");


	if (file.is_open())
	{
		while (!file.eof())
		{
			file >> fv.varName >> fv.values[0] >> fv.values[1] >> fv.values[2];
			fileValues.push_front(fv);
		}
	}

	file.close();
}

FileValues FileReader::GetFileValues(int index)
{
	list<FileValues>::iterator it = fileValues.begin();
	//for (list<FileValues>::iterator it = fileValues.begin(); it != ; it++)
	//{
		advance(it, index);
		return *it;
	//}
}

