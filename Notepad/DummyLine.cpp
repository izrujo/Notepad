#include "DummyLine.h"
#include "Line.h"
#include "Glyph.h"
#include "Character.h"

DummyLine::DummyLine(Long capacity)
	: Line(capacity) {
}
/*
DummyLine::DummyLine(Glyph *line) {

	Glyph *character;
	Long i = 0;
	while (i < line->GetLength()) {
		character = line->GetAt(i);
		this->glyphs.AppendFromRear(character->Clone());
		i++;
	}
	this->capacity = line->GetCapacity();
	this->length = line->GetLength();
	this->current = 0;
}
*/

DummyLine::DummyLine(const DummyLine& source)
	: Line(source) {
}

DummyLine::~DummyLine() {
}

DummyLine& DummyLine::operator =(const DummyLine& source) {
	Line::operator =(source);

	return *this;
}

string DummyLine::GetContent() {
	string content = "";
	Long i = 0;
	while (i < this->length) {
		content += this->glyphs[i]->GetContent();
		i++;
	}
	return content;
}

Glyph* DummyLine::Clone() {
	return new DummyLine(*this);
}