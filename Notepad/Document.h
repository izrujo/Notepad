#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include <iostream>

using namespace std;

class NotepadForm;

class Document {
public:
	Document(NotepadForm* notepadForm = 0);
	Document(const Document& source);
	~Document();
	Document& operator=(const Document& source);

	void SetIsDirty(bool isDirty);
	void SetPathName(string pathName);

	bool GetIsDirty() const;
	string& GetPathName() const;

private:
	NotepadForm* notepadForm;
	bool isDirty;
	string pathName;
};

inline bool Document::GetIsDirty() const {
	return this->isDirty;
}

inline string& Document::GetPathName() const {
	return const_cast<string&>(this->pathName);
}

#endif //_DOCUMENT_H