#ifndef _FILEFACTORY_H
#define _FILEFACTORY_H

#include <iostream>

using namespace std;

class File;

class FileFactory {
public:
	FileFactory();
	FileFactory(const FileFactory& source);
	~FileFactory();
	FileFactory& operator=(const FileFactory& source);

	File* MakeSaveFile(string filePathName, string encodingType);
	File* MakeOpenFile(string filePathName);
};

#endif //_FILEFACTORY_H