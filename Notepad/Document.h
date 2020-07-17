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
	void SetEncodingType(string encodingType);

	bool GetIsDirty() const;
	string& GetPathName() const;
	string& GetEncodingType() const;

private:
	NotepadForm* notepadForm;
	bool isDirty;
	string pathName;
	string encodingType;
};

inline bool Document::GetIsDirty() const {
	return this->isDirty;
}

inline string& Document::GetPathName() const {
	return const_cast<string&>(this->pathName);
}

inline string& Document::GetEncodingType() const {
	return const_cast<string&>(this->encodingType);
}

#endif //_DOCUMENT_H