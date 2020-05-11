#include "GlyphFactory.h"
#include "Glyph.h"
#include "Note.h"
#include "Line.h"
#include "Character.h"
#include "NotepadForm.h"
#include "CharacterMetrics.h"
#include "DummyLine.h"

GlyphFactory::GlyphFactory(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}
GlyphFactory::GlyphFactory(const GlyphFactory& source) {
	this->notepadForm = source.notepadForm;
}
GlyphFactory::~GlyphFactory() {
}
Glyph* GlyphFactory::Make(const char(*content)) {
	Glyph *glyph = 0;
	if (content[0] == '\0') {
		glyph = new Note;
	}
	else if (content[0] == '\r' && content[1] == '\n') {
		glyph = new Line;
	}
	else if (!(content[0] & 0x80)) {
		glyph = new SingleByteCharacter(content[0]);
	}
	else if (content[0] & 0x80) {
		glyph = new DoubleByteCharacter(content);
	}
	else if (content[0] == '\r') {
		glyph = new DummyLine;
	}

	return glyph;
}

Glyph* GlyphFactory::Make(Glyph *line, Long clientWidth) {
	Long lineWidth = this->notepadForm->characterMetrics->GetX(line, line->GetLength());
	Glyph *character = 0;
	string content;
	Long characterWidth;
	Long i = line->GetLength() - 1;
	while (i >= 0 && lineWidth > clientWidth) {
		character = line->GetAt(i);
		if (dynamic_cast<SingleByteCharacter*>(character)) {
			content = character->GetContent();
			if (content.length() == 1) {
				characterWidth = this->notepadForm->characterMetrics->GetWidth(content[0]);
			}
			else {
				characterWidth = this->notepadForm->characterMetrics->GetWidth(9);
			}
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			characterWidth = this->notepadForm->characterMetrics->GetDoubleByteWidth();
		}
		lineWidth -= characterWidth;
		i--;
	}
	Glyph *blank;
	Long j = i + 1; //ȭ�� ���� ��� ���ں��� Ȯ��
	while (j >= 0 && content != " ") {
		blank = line->GetAt(j);
		content = blank->GetContent();
		j--;
	}
	bool isBlankExist = FALSE;
	if (j >= 0) {
		isBlankExist = TRUE;
		j += 1; //���鹮�� ���� ��ġ�� ����
	}
	Long l;
	Glyph *dummyCharacter;
	Glyph *temp = 0;
	Glyph *dummyLine = new DummyLine;
	if (isBlankExist == TRUE) {
		temp = line->Divide(j);
		l = 0;
		while (l < temp->GetLength()) {
			dummyCharacter = temp->GetAt(l)->Clone();
			dummyLine->Add(dummyCharacter);
			l++;
		}
	}
	else if (isBlankExist == FALSE || dynamic_cast<DoubleByteCharacter*>(character)) {
		temp = line->Divide(i + 1);
		l = 0;
		while (l < temp->GetLength()) {
			dummyCharacter = temp->GetAt(l)->Clone();
			dummyLine->Add(dummyCharacter);
			l++;
		}
	}
	if (temp != 0) {
		delete temp;
	}

	return dummyLine;
}
/*
#include <iostream>
#include "Character.h"
#include "Composite.h"

int main(int argc, char *argv[]) {
	GlyphFactory glyphFactory;
	Glyph *note = glyphFactory.Make("");
	Glyph *glyph = glyphFactory.Make("\r\n");
	Glyph *line;

	Long index = note->Add(glyph); //�� ���� �����.
	line = note->GetAt(index); //���� ���� �ּҸ� �����´�.

	glyph = glyphFactory.Make("a"); //a�� �Է¹޴´�.
	index = line->Add(glyph); //a�� ���� �ٿ� �߰��Ѵ�.
	glyph = line->GetAt(index); //�߰��� ������ �ּҸ� �����´�.
	cout << glyph->GetContent() << endl; //�߰��� ���ڸ� ����Ѵ�.

	glyph = glyphFactory.Make("��"); //���� �Է¹޴´�.
	index = line->Add(glyph); //���� ���� �ٿ� �߰��Ѵ�.
	cout << line->GetAt(index)->GetContent() << endl; //�߰��� ������ �ּҸ� ������ ����Ѵ�.

	glyph = glyphFactory.Make("\r\n"); //����Ű�� �Է¹޴´�.
	index = note->Add(glyph); //�� ���� �����.
	line = note->GetAt(index); //���� ���� �ּҸ� �����´�.
	glyph = glyphFactory.Make("3"); //3�� �Է¹޴´�.
	index = line->Add(glyph); //3�� ���� �ٿ� �߰��Ѵ�.
	cout << line->GetAt(index)->GetContent() << endl; //�߰��� ������ �ּҸ� ������ ����Ѵ�.

	Glyph *otherNote(note); //����
	cout << otherNote->GetContent() << endl; //������ ��Ʈ�� ��� ���ڵ��� ����Ѵ�.

	glyph = glyphFactory.Make("��"); //���� �Է¹޴´�.
	line = otherNote->GetAt(0); //������ ��Ʈ�� ù ��° ���� �ּҸ� �����´�.
	index = line->Add(glyph); //���� ���� �ٿ� �߰��Ѵ�.
	glyph = line->GetAt(index); //�߰��� ������ �ּҸ� �����´�.
	cout << glyph->GetContent() << endl; //�߰��� ���ڸ� ����Ѵ�.

	Glyph *another(otherNote); //ġȯ
	cout << another->GetContent() << endl; //ġȯ�� ��Ʈ�� ��� ���ڵ��� ����Ѵ�.

	if (note != 0) {
		delete note;
	}

	return 0;
}*/