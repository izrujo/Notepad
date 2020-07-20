#include "Selector.h"
#include "NotepadForm.h"
#include "GlyphFactory.h"
#include "Composite.h"

Selector::Selector(NotepadForm *notepadForm, Long noteStartPosition, Long lineStartPosition, Long noteEndPosition, Long lineEndPosition) {
	this->notepadForm = notepadForm;
	this->noteStartPosition = noteStartPosition;
	this->lineStartPosition = lineStartPosition;
	this->noteEndPosition = noteEndPosition;
	this->lineEndPosition = lineEndPosition;
}

Selector::Selector(const Selector& source) {
	this->notepadForm = source.notepadForm;
	this->noteStartPosition = source.noteStartPosition;
	this->lineStartPosition = source.lineStartPosition;
	this->noteEndPosition = source.noteEndPosition;
	this->lineEndPosition = source.lineEndPosition;
}

Selector::~Selector() {

}

Selector& Selector::operator=(const Selector& source) {
	this->notepadForm = source.notepadForm;
	this->noteStartPosition = source.noteStartPosition;
	this->lineStartPosition = source.lineStartPosition;
	this->noteEndPosition = source.noteEndPosition;
	this->lineEndPosition = source.lineEndPosition;
	return *this;
}

void Selector::Left(Long row, Long startColumn, Long endColumn) {
	Glyph *line;
	//1. ����� ���� �ƴϸ�
	if (this->noteStartPosition >= this->noteEndPosition &&
		(this->noteStartPosition != this->noteEndPosition || this->lineStartPosition >= this->lineEndPosition)) {
		//1.1. ���̶���Ʈ�� ���ų� ���� �ö������
		if (this->notepadForm->highlight->GetLength() < 1 || row < this->noteEndPosition) {
			//1.1.1. ���� �����.
			GlyphFactory glyphFactory;
			line = glyphFactory.Make("\r\n");
			//1.1.2. ���� ���̶���Ʈ�� �߰��ϴ�.
			this->notepadForm->highlight->Add(0, line);
		}
		//1.2. ���̶���Ʈ�� ù ��° ���� ��������.
		line = this->notepadForm->highlight->GetAt(0);
	}
	//2. ����� ���̸�
	else {
		//2.1. ���� �ö������ ���̶���Ʈ�� ������ ���� �����.
		if (row < this->noteEndPosition) {
			this->notepadForm->highlight->Remove(this->notepadForm->highlight->GetLength() - 1);
		}
		//2.2. ���̶���Ʈ�� ������ ���� ��������.
		line = this->notepadForm->highlight->GetAt(this->notepadForm->highlight->GetLength() - 1);
	}
	Glyph *currentLine = this->notepadForm->note->GetAt(row);
	//3. ���� �ö������ ���� ���� ��������, �ȿö������ �ٳ��ڸ����� ���ۿ����� �ݺ��ϴ�.
	Long i = this->lineEndPosition;
	if (row < this->noteEndPosition) {
		i = currentLine->GetLength();
	}
	while (i > startColumn) {
		//3.1. ���� ���� �����.
		line->Remove(line->GetLength() - 1);
		i--;
	}
	//4. ���ۿ����� �������� �ݺ��ϴ�.
	i = startColumn;
	while (i > endColumn) {
		//4.1. ���� �ٿ��� ���� ���� ���ڸ� ��������.
		Glyph *character = currentLine->GetAt(i - 1);
		//4.2. ������ �ٿ� ������ Ŭ���� �߰��ϴ�.
		line->Add(0, character->Clone());
		i--;
	}
	//5. �������ڸ��� ���ϴ�.
	this->noteEndPosition = row;
	//6. �ٳ��ڸ��� ���ϴ�.
	this->lineEndPosition = endColumn;
}

void Selector::Right(Long row, Long startColumn, Long endColumn) {
	Glyph *line;
	//1. ����� ���� �ƴϸ�
	if (this->noteStartPosition <= this->noteEndPosition &&
		(this->noteStartPosition != this->noteEndPosition || this->lineStartPosition <= this->lineEndPosition)) {
		//1.1. ���̶���Ʈ�� ���ų� �Ʒ��� ����������
		if (this->notepadForm->highlight->GetLength() < 1 || row > this->noteEndPosition) {
			//1.1.1. ���� �����.
			GlyphFactory glyphFactory;
			line = glyphFactory.Make("\r\n");
			//1.1.2. ���� ���̶���Ʈ�� �߰��ϴ�.
			this->notepadForm->highlight->Add(line);
		}
		//1.2. ���̶���Ʈ�� ������ ���� ��������.
		line = this->notepadForm->highlight->GetAt(this->notepadForm->highlight->GetLength() - 1);
	}
	//2. ����� ���̸�
	else {
		//2.1. �Ʒ��� ���������� ���̶���Ʈ�� ù ���� �����.
		if (row > this->noteEndPosition) {
			this->notepadForm->highlight->Remove(0);
		}
		//2.2. ���̶���Ʈ�� ù ���� ��������.
		line = this->notepadForm->highlight->GetAt(0);
	}
	Glyph *currentLine = this->notepadForm->note->GetAt(row);
	//3. �Ʒ��� ���������� 0����, �ȿö������ �ٳ��ڸ����� ���ۿ����� �ݺ��ϴ�.
	Long i = this->lineEndPosition;
	if (row > this->noteEndPosition) {
		i = 0;
	}
	while (i < startColumn) {
		//3.1. ���� ó���� �����.
		line->Remove(0);
		i++;
	}
	//4. ���ۿ����� �������� �ݺ��ϴ�.
	i = startColumn;
	while (i < endColumn) {
		//4.1. ���� �ٿ��� ���� ���� ���ڸ� ��������.
		Glyph *character = currentLine->GetAt(i);
		//4.2. ������ �ٿ� ������ Ŭ���� �߰��ϴ�.
		line->Add(character->Clone());
		i++;
	}
	//5. �������ڸ��� ���ϴ�.
	this->noteEndPosition = row;
	//6. �ٳ��ڸ��� ���ϴ�.
	this->lineEndPosition = endColumn;
}