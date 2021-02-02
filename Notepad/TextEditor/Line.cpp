#include "Line.h"
#include "Glyph.h"
#include "Characters.h"
#include "Visitor.h"

Line::Line(Long capacity)
	: Composite(capacity) {

}

Line::Line(const Line& source)
	: Composite(source) {

}

Line::~Line() {

}

Line& Line::operator=(const Line& source) {
	Composite::operator=(source);

	return *this;
}

Glyph* Line::Divide(Long index) {
	Glyph* line = new Line;
	Glyph* character;
	Long i = index;
	Long length = this->length;
	while (i < length) {
		character = this->glyphs.GetAt(index);
		line->Add(character);
		this->glyphs.Delete(index);
		this->capacity--;
		this->length--;
		i++;
	}

	return line;
}

Glyph* Line::Combine(Glyph* other) {
	Long i = 0;
	while (i < other->GetLength()) {
		Glyph* character = other->GetAt(i);
		Glyph* characterOther = character->Clone();
		if (this->length < this->capacity) {
			this->glyphs.Store(this->length, characterOther);
		}
		else {
			this->glyphs.AppendFromRear(characterOther);
			this->capacity++;
		}
		this->length++;
		i++;
	}
	return this;
}

Long Line::MovePreviousWord() {
	while (this->current > 0 && (this->glyphs[this->current - 1]->GetContent())[0] == ' ') {
		this->current--;
	}
	while (this->current > 0 && (this->glyphs[this->current - 1]->GetContent())[0] != ' ') {
		this->current--;
	}

	return this->current;
}

Long Line::MoveNextWord() {
	while (this->current < this->length && (this->glyphs[this->current]->GetContent())[0] == ' ') {
		this->current++;
	}
	while (this->current < this->length && (this->glyphs[this->current]->GetContent())[0] != ' ') {
		this->current++;
	}
	while (this->current < this->length && (this->glyphs[this->current]->GetContent())[0] == ' ') {
		this->current++;
	}

	return this->current;
}

bool Line::IsFirst() {
	bool isFirst = false;
	if (this->current == 0) {
		isFirst = true;
	}

	return isFirst;
}

bool Line::IsLast() {
	bool isLast = false;
	if (this->current == this->length) {
		isLast = true;
	}

	return isLast;
}

void Line::UnselectAll() {
	Glyph* character;
	Long i = 0;
	while (i < this->length) {
		character = this->glyphs[i];
		character->Select(false);
		i++;
	}
}

bool Line::IsSelecting() {
	bool isSelecting = false;
	Glyph* character;
	Long j = 0;
	while (j < this->length && isSelecting == false) {
		character = this->glyphs[j];
		isSelecting = character->GetIsSelected();
		j++;
	}

	return isSelecting;
}

Long Line::Select(Long start, Long end) {
	Glyph* character;
	Long count = 0;
	Long i = start;
	while (i < end) {
		character = this->glyphs.GetAt(i);
		character->Select(true);
		count++;
		i++;
	}
	return count;
}

Glyph* Line::Clone() {
	return new Line(*this);
}

string Line::GetContent() {
	string content = "";
	Long i = 0;
	while (i < this->length) {
		content += this->glyphs[i]->GetContent();
		i++;
	}
	return content;
}

void Line::Accept(Visitor* visitor) {
	visitor->Visit(this);
}