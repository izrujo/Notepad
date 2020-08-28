//Document.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : Manage current file state.
MVC : ?
Author : Chan Hyung Kim
Written Date : 2020.07.14
Recently Updated : 2020.07.17
**************************************************/

#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include <iostream>
#include <afxwin.h>

using namespace std;
typedef signed long int Long;

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

	void SetPaperSize(CSize paperSize);
	void SetIsVertical(bool isVertical);
	void SetMargins(CRect margins);
	void SetHeader(string header);
	void SetFooter(string footer);

	bool GetIsDirty() const;
	string& GetPathName() const;
	string& GetEncodingType() const;
	
	CSize& GetPaperSize() const;
	bool GetIsVertical() const;
	CRect& GetMargins() const;
	string& GetHeader() const;
	string& GetFooter() const;

private:
	NotepadForm* notepadForm;
	bool isDirty;
	string pathName;
	string encodingType;
	//페이지 설정
	//용지(크기, 공급(x)), 방향,	여백, 머리글, 바닥글
	CSize paperSize;
	bool isVertical;
	CRect margins;
	string header;
	string footer;
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

inline CSize& Document::GetPaperSize() const {
	return const_cast<CSize&>(this->paperSize);
}

inline bool Document::GetIsVertical() const {
	return this->isVertical;
}

inline CRect& Document::GetMargins() const {
	return const_cast<CRect&>(this->margins);
}

inline string& Document::GetHeader() const {
	return const_cast<string&>(this->header);
}

inline string& Document::GetFooter() const {
	return const_cast<string&>(this->footer);
}

#endif //_DOCUMENT_H