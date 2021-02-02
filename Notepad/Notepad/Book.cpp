#include "Book.h"
#include "../TextEditor/Visitor.h"

Book::Book(Long capacity)
	: Composite(capacity) {

}

Book::Book(const Book& source)
	: Composite(source) {

}

Book::~Book() {

}

Book& Book::operator=(const Book& source) {
	Composite::operator=(source);

	return *this;
}

Long Book::Add(Glyph* glyph) {
	Long index = Composite::Add(glyph);
	this->current = index;

	return index;
}

Long Book::Add(Long index, Glyph* glyph) {
	Composite::Add(index, glyph);
	this->current = index;

	return this->current;
}

Long Book::Remove(Long index) {
	Composite::Remove(index);
	this->current = index - 1;

	return -1;
}

Long Book::Next() {
	this->current = Composite::Next();
	if (this->current >= this->length) {
		this->current = this->length - 1;
	}

	return this->current;
}

Glyph* Book::Clone() {
	return new Book(*this);
}

string Book::GetContent() {
	string content = "";

	return content;
}

void Book::Accept(Visitor* visitor) {
	visitor->Visit(this);
}