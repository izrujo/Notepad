#include "DummyLine.h"
#include "Line.h"
#include "Glyph.h"
#include "Characters.h"

DummyLine::DummyLine(Long capacity)
	: Line(capacity) {
}

DummyLine::DummyLine(const DummyLine& source)
	: Line(source) {
}

DummyLine::~DummyLine() {
}

DummyLine& DummyLine::operator=(const DummyLine& source) {
	Line::operator=(source);

	return *this;
}

Glyph* DummyLine::Clone() {
	return new DummyLine(*this);
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