#include "Document.h"
#include "NotepadForm.h"

Document::Document(NotepadForm* notepadForm) 
	: pathName("제목 없음"), encodingType("ANSI") {
	this->notepadForm = notepadForm;
	this->isDirty = false;
}

Document::Document(const Document& source) 
	: pathName(source.pathName), encodingType(source.encodingType) {
	this->notepadForm = source.notepadForm;
	this->isDirty = source.isDirty;
}

Document::~Document() {

}

Document& Document::operator=(const Document& source) {
	this->notepadForm = source.notepadForm;
	this->isDirty = source.isDirty;
	this->pathName = source.pathName;
	this->encodingType = source.encodingType;

	return *this;
}

void Document::SetIsDirty(bool isDirty) {
	this->isDirty = isDirty;
}

void Document::SetPathName(string pathName) {
	this->pathName = pathName;
}

void Document::SetEncodingType(string encodingType) {
	this->encodingType = encodingType;
}