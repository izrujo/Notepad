#include "Glyph.h"

Glyph::Glyph() {
}

Glyph::Glyph(const Glyph& source) {
}

Glyph::~Glyph() {
}

Glyph& Glyph::operator=(const Glyph& source) {
	return *this;
}

Long Glyph::Add(Glyph* glyph) {
	return -1;
}

Long Glyph::Add(Long index, Glyph* glyph) {
	return -1;
}

Long Glyph::Remove(Long index) {
	return -1;
}

Glyph* Glyph::GetAt(Long index) {
	return 0;
}


Glyph* Glyph::Divide(Long index) {
	return 0;
}

Glyph* Glyph::Combine(Glyph* index) {
	return 0;
}

Long Glyph::First() {
	return -1;
}

Long Glyph::Last() {
	return -1;
}

Long Glyph::Previous() {
	return -1;
}

Long Glyph::Next() {
	return -1;
}

Long Glyph::Move(Long index) {
	return -1;
}

Long Glyph::MovePreviousWord() {
	return -1;
}

Long Glyph::MoveNextWord() {
	return -1;
}

bool Glyph::IsFirst() {
	return false;
}

bool Glyph::IsLast() {
	return false;
}

void Glyph::UnselectAll() {

}

bool Glyph::IsSelecting() {
	return false;
}

string Glyph::GetSelectedContent(Long start, Long end) {
	return "";
}

void Glyph::Select(bool isSelected) {

}

Long Glyph::Select(Long start, Long end) {
	return 0;
}

Long Glyph::Select(Long start, Long startColumn, Long end, Long endColumn) {
	return 0;
}

Long Glyph::GetSelectedStartColumn(Long start) {
	return -1;
}

Long Glyph::GetSelectedEndColumn(Long end) {
	return -1;
}

string Glyph::GetContent(Long startRow, Long startColumn, Long endRow, Long endColumn) {
	return "";
}

Long Glyph::GetCapacity() const {
	return -1;
}
Long Glyph::GetLength() const {
	return -1;
}
Long Glyph::GetCurrent() const {
	return -1;
}

bool Glyph::GetIsSelected() const {
	return false;
}

#if 0

#include "Composite.h"
#include "Note.h"
#include "Line.h"
#include "GlyphFactory.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	GlyphFactory glyphFactory;

	Glyph* note = glyphFactory.Make("");

	Glyph* line = glyphFactory.Make("\r\n");
	note->Add(line);

	Glyph* character = glyphFactory.Make("a");
	line->Add(character);

	character = glyphFactory.Make("��");
	line->Add(character);

	character = glyphFactory.Make(" ");
	line->Add(character);

	character = glyphFactory.Make("��");
	line->Add(character);

	line = glyphFactory.Make("\r\n");
	note->Add(line);

	character = glyphFactory.Make("1");
	line->Add(character);

	character = glyphFactory.Make("b");
	line->Add(character);

	Long j;
	Long i = 0;
	while (i < note->GetLength()) {
		line = note->GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			cout << character->GetContent();
			j++;
		}
		cout << endl;
		i++;
	}

	Long index = note->First();
	line = note->GetAt(index);
	string content = line->GetContent();
	cout << content << endl;

	index = line->First();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = line->Previous();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = note->Previous();
	line = note->GetAt(index);
	content = line->GetContent();
	cout << content << endl;

	index = line->MoveNextWord();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = line->Next();
	cout << index << endl;

	index = line->Next();
	cout << index << endl;

	index = note->Next();
	line = note->GetAt(index);
	content = line->GetContent();
	cout << content << endl;

	index = line->First();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	index = line->Next();
	character = line->GetAt(index);
	content = character->GetContent();
	cout << content << endl;

	Long current = line->GetCurrent();
	index = note->Previous();
	line = note->GetAt(index);

	index = line->Move(current);
	character = line->GetAt(index);
	cout << character->GetContent() << endl;
	//19
	index = line->Last();
	cout << index << endl;
	cout << "���� ����" << line->GetLength() << endl;
	//20
	index = note->MoveNextWord();
	line = note->GetAt(index);
	cout << line->GetCurrent() << endl;
	//21
	index = line->MoveNextWord();
	cout << line->GetCurrent() << endl;
	//22
	index = line->First();
	character = line->GetAt(index);
	cout << character->GetContent() << endl;
	//23
	index = note->MovePreviousWord();
	line = note->GetAt(index);
	index = line->GetCurrent();
	cout << index << endl;
	//24
	index = line->MovePreviousWord();
	character = line->GetAt(index);
	cout << character->GetContent() << endl;
	//25
	index = note->Move(1);
	line = note->GetAt(index);
	cout << line->GetContent() << endl;
	//26
	index = line->Move(0);
	character = line->GetAt(index);
	cout << character->GetContent() << endl;

	// 20190509
	// 1. ù���� �ٿ��� 2��° ���� ���� ������. -Line::Divide
	index = note->First();
	line = note->GetAt(index);
	line = line->Divide(1);
	cout << line->GetContent() << endl;

	cout << "=================================================" << endl;
	// 2. ��Ʈ�� 2��°�� ���� ���� �߰��Ѵ�. -Note::Add
	index = note->Add(1, line);
	cout << note->GetContent() << endl;
	cout << "=================================================" << endl;

	// 3. 1��° �ٰ� 2��° ���� ��ģ��. -Line::Combine
	line = note->GetAt(0);
	Glyph* otherLine = note->GetAt(1);
	line->Combine(otherLine);
	cout << line->GetContent() << endl;
	cout << "=================================================" << endl;
	// 4. 2��° ���� �����. - Note:Remove
	note->Remove(1);
	cout << note->GetContent() << endl;
	cout << "=================================================" << endl;
	// 5. ��Ʈ�� ���� ���� �߰��Ѵ�. - Note:Add
	line = glyphFactory.Make("\r\n");
	index = note->Add(line);
	line = note->GetAt(index);
	// 6. �ٿ� '��'�� �߰��Ѵ�. - Line::Add
	character = glyphFactory.Make("��");
	line->Add(character);
	cout << note->GetContent() << endl;
	cout << "=================================================" << endl;
	// 7. �ٿ� 1��°�� '��'�� �߰��Ѵ� - Line::Add
	character = glyphFactory.Make("��");
	line->Add(0, character);
	cout << note->GetContent() << endl;
	// 6. ��Ʈ���� ���� ��ġ�� ���� �����. - Note::Remove(current)
	note->Remove(note->GetCurrent());
	cout << "=================================================" << endl;
	cout << note->GetContent() << endl;

	if (note != 0) {
		delete note;
	}

	return 0;

}
#endif