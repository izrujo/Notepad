#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <iostream>

using namespace std;

class NotepadForm;

class FileManager {
public:
	FileManager(NotepadForm* notepadForm = 0);
	FileManager(const FileManager& source);
	~FileManager();
	FileManager& operator=(const FileManager& source);

	void Save();
	void Load();
	void New();
private:
	NotepadForm* notepadForm;
};

#endif //_FILEMANAGER_H