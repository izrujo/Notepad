#include "Document.h"
#include "NotepadForm.h"

Document::Document(NotepadForm* notepadForm) 
	: pathName("제목 없음"), encodingType("ANSI"), 
	paperSize(21000, 29700), margins(2000, 2500, 2000, 2500), header(""), footer("") {
	this->notepadForm = notepadForm;
	this->isDirty = false;
	this->isVertical = true;
}

Document::Document(const Document& source) 
	: pathName(source.pathName), encodingType(source.encodingType), 
	paperSize(source.paperSize), margins(source.margins), header(source.header), footer(source.footer) {
	this->notepadForm = source.notepadForm;
	this->isDirty = source.isDirty;
	this->isVertical = source.isVertical;
}

Document::~Document() {

}

Document& Document::operator=(const Document& source) {
	this->notepadForm = source.notepadForm;
	this->isDirty = source.isDirty;
	this->pathName = source.pathName;
	this->encodingType = source.encodingType;
	this->paperSize = source.paperSize;
	this->isVertical = source.isVertical;
	this->margins = source.margins;
	this->header = source.header;
	this->footer = source.footer;

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

void Document::SetPaperSize(CSize paperSize) {
	this->paperSize = paperSize;
}

void Document::SetIsVertical(bool isVertical) {
	this->isVertical = isVertical;
}

void Document::SetMargins(CRect margins) {
	this->margins = margins;
}

void Document::SetHeader(string header) {
	this->header = header;
}

void Document::SetFooter(string footer) {
	this->footer = footer;
}