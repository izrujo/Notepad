#include "Note.h"
#include "Visitor.h"

Note::Note(Long capacity)
	: Composite(capacity) {

}

Note::Note(const Note& source)
	: Composite(source) {

}

Note::~Note() {

}

Note& Note::operator=(const Note& source) {
	Composite::operator=(source);

	return *this;
}

Long Note::Add(Glyph* glyph) {
	Long index = Composite::Add(glyph);
	this->current = index;

	return index;
}

Long Note::Add(Long index, Glyph* glyph) {
	Composite::Add(index, glyph);
	this->current = index;

	return this->current;
}
Long Note::Remove(Long index) {
	Composite::Remove(index);
	this->current = index - 1;

	return -1;
}

Glyph* Note::Divide(Long index) {
	Glyph* note = new Note;
	Glyph* line;
	Long i = index;
	Long length = this->length;
	while (i < length) {
		line = this->glyphs.GetAt(index);
		note->Add(line);
		this->glyphs.Delete(index);
		this->capacity--;
		this->length--;
		i++;
	}

	return note;
}

Long Note::Next() {
	this->current = Composite::Next();
	if (this->current >= this->length) {
		this->current = this->length - 1;
	}

	return this->current;
}

Long Note::Last() {
	this->current = this->length - 1;

	return this->current;
}

Long Note::MovePreviousWord() {
	Glyph* line;
	line = this->glyphs.GetAt(this->current);
	if (line->GetCurrent() >= 1) {
		line->MovePreviousWord();
	}
	else {
		if (this->current > 0) {
			this->current--;
			line = this->glyphs.GetAt(this->current);
			line->Last();
		}
	}

	return this->current;
}

Long Note::MoveNextWord() {
	Glyph* line;
	line = this->glyphs.GetAt(this->current);
	if (line->GetCurrent() < line->GetLength()) {
		line->MoveNextWord();
	}
	else {
		if (this->current < this->length - 1) {
			this->current++;
			line = this->glyphs.GetAt(this->current);
			line->First();
		}
	}

	return this->current;
}

bool Note::IsFirst() {
	bool isFirst = false;
	if (this->current == 0 && this->glyphs.GetAt(this->current)->IsFirst()) {
		isFirst = true;
	}

	return isFirst;
}

bool Note::IsLast() {
	bool isLast = false;
	if (this->current == this->length-1 && this->glyphs.GetAt(this->current)->IsLast()) {
		isLast = true;
	}

	return isLast;
}

void Note::UnselectAll() {
	Glyph* line;
	Long i = 0;
	while (i < this->length) {
		line = this->glyphs[i];
		line->UnselectAll();
		i++;
	}
}

bool Note::IsSelecting() {
	bool isSelecting = false;
	Glyph* line;
	Long i = 0;
	while (i < this->length && isSelecting == false) {
		line = this->glyphs[i];
		isSelecting = line->IsSelecting();
		i++;
	}

	return isSelecting;
}

string Note::GetSelectedContent(Long start, Long end) {
	string content = "";
	string characterString;
	Glyph* line;
	Glyph* character;
	Long j;
	Long i = start;

	while (i <= end) {
		line = this->glyphs.GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			if (character->GetIsSelected() == true) {
				characterString = character->GetContent();
				if (characterString.length() == 8) {
					characterString = '\t';
				}
				content += characterString;
			}
			j++;
		}
		if (i < end) {
			content += "\n";
		}
		i++;
	}

	return content;
}

Long Note::Select(Long start, Long startColumn, Long end, Long endColumn) {
	Glyph* line;
	Long count = 0;
	Long first;
	Long last;
	Long i = start;
	while (i <= end) {
		line = this->glyphs.GetAt(i);
		if (i == start && i == end) {
			first = startColumn;
			last = endColumn;
		}
		else if (i == start) {
			first = startColumn;
			last = line->GetLength();
		}
		else if (i == end) {
			first = 0;
			last = endColumn;
		}
		else {
			first = 0;
			last = line->GetLength();
		}
		line->Select(first, last);
		count++;
		i++;
	}

	return count;
}

Long Note::GetSelectedStartColumn(Long start) {
	Long startColumn;
	Glyph* character;
	Glyph* line;

	bool isSelected = false;
	line = this->glyphs.GetAt(start); //시작 행
	Long i = 0;
	while (i < line->GetLength() && isSelected == false) { //시작 행의 개수만큼 그리고 현재 글자가 선택되어있지 않은 동안 반복하다.
		character = line->GetAt(i); //시작 행에서 글자를 가져오다.
		isSelected = character->GetIsSelected(); //현재 글자의 선택여부를 확인하다.
		i++;
	}
	startColumn = i - 1;
	if (isSelected == false) {
		startColumn++;
	}
	return startColumn;
}

Long Note::GetSelectedEndColumn(Long end) {
	Long endColumn;
	Glyph* character;
	Glyph* line;

	bool isSelected = false;
	line = this->glyphs.GetAt(end);
	Long i = line->GetLength();
	while (i > 0 && isSelected == false) {
		character = line->GetAt(i - 1);
		isSelected = character->GetIsSelected();
		i--;
	}
	endColumn = i + 1;
	if (isSelected == false) {
		endColumn--;
	}

	return endColumn;
}

Glyph* Note::Clone() {
	return new Note(*this);
}

string Note::GetContent() {
	string content = "";
	string characterString;
	Glyph* line;
	Glyph* character;
	Long j;
	Long i = 0;

	while (i < this->length) {
		line = this->glyphs.GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			characterString = character->GetContent();
			if (characterString.length() == 8) {
				characterString = '\t';
			}
			content += characterString;
			j++;
		}
		if (i < this->length - 1) {
			content += "\n";
		}
		i++;
	}

	return content;
}

string Note::GetContent(Long startRow, Long startColumn, Long endRow, Long endColumn) {
	string content = "";
	string characterString;
	Glyph* line;
	Glyph* character;
	Long length;
	Long j;
	Long i = startRow;

	while (i <= endRow) {
		line = this->glyphs.GetAt(i);
		length = line->GetLength();
		if (i == endRow) {
			length = endColumn;
		}
		j = 0;
		if (i == startRow) {
			j = startColumn;
		}
		while (j < length) {
			character = line->GetAt(j);
			characterString = character->GetContent();
			if (characterString.length() == 8) {
				characterString = '\t';
			}
			content += characterString;
			j++;
		}
		if (i < endRow) {
			content += "\n";
		}
		i++;
	}

	return content;
}

void Note::Accept(Visitor* visitor) {
	visitor->Visit(this);
}