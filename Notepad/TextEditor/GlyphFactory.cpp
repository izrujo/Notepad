#include "GlyphFactory.h"
#include "Glyph.h"
#include "Note.h"
#include "Line.h"
#include "Characters.h"
#include "CharacterMetrics.h"

GlyphFactory::GlyphFactory() {
	
}

GlyphFactory::GlyphFactory(const GlyphFactory& source) {
	
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

	return glyph;
}

/*
#include <iostream>
#include "Characters.h"
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