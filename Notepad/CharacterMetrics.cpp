#include "CharacterMetrics.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "Characters.h"
#include "Font.h"

CharacterMetrics::CharacterMetrics(NotepadForm* notepadForm) {
	this->notepadForm = notepadForm;
	CDC* dc = this->notepadForm->GetDC();
	CString buffer;
	CSize size;
	CFont* oldFont;
	CFont font;
	this->notepadForm->font->Create(font);
	oldFont = dc->SelectObject(&font);

	Long i = 1;
	while (i < 128) {
		buffer.Format("%c", i);
		size = dc->GetTextExtent(buffer);
		this->widths[i] = size.cx;
		i++;
	}
	this->widths[9] = this->widths[32] * 8; //Tab ó��
	this->widths[0] = this->widths[32];
	size = dc->GetTextExtent("��");
	this->widths[i] = size.cx;

	TEXTMETRICA textmetrica;
	memset(&textmetrica, 0, sizeof(textmetrica));
	dc->GetTextMetrics(&textmetrica);
	this->height = textmetrica.tmHeight;
	dc->SelectObject(oldFont);
}

CharacterMetrics::CharacterMetrics(const CharacterMetrics& source) {
	this->notepadForm = source.notepadForm;

	Long i = 0;
	while (i < 129) {
		this->widths[i] = source.widths[i];
		i++;
	}

	this->height = source.height;
}

CharacterMetrics::~CharacterMetrics() {

}

Long CharacterMetrics::GetX(Glyph* line) {
	Long x = 0;

	Long i = 0;
	while (i < line->GetCurrent()) {
		Glyph* character = line->GetAt(i);

		if (dynamic_cast<SingleByteCharacter*>(character)) {
			string content = character->GetContent();
			if (content.length() == 1) {
				x += this->widths[content[0]];
			}
			else {
				x += this->widths[9]; //Tab
			}
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			x += this->widths[128];
		}

		i++;
	}

	return x;
}

Long CharacterMetrics::GetX(Glyph* line, Long index) {
	Long x = 0;

	Long i = 0;
	while (i < index) {
		Glyph* character = line->GetAt(i);

		if (dynamic_cast<SingleByteCharacter*>(character)) {
			string content = character->GetContent();
			if (content.length() == 1) {
				x += this->widths[content[0]];
			}
			else {
				x += this->widths[9]; //Tab
			}
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			x += this->widths[128];
		}

		i++;
	}

	return x;
}

Long CharacterMetrics::GetX(const string& buffer) {
	Long x = 0;

	Long i = 0;
	while (i < buffer.length()) {
		if (!(buffer[i] & 0x80)) {
			if (buffer.length() == 1) {
				x += this->widths[buffer[0]];
			}
			else {
				x += this->widths[9]; //Tab
			}
			i++;
		}
		else if (buffer[i] & 0x80) {
			x += this->widths[128];
			i += 2;
		}
	}

	return x;
}

Long CharacterMetrics::GetY(Long index) {
	return this->height * (index);
}

CharacterMetrics& CharacterMetrics::operator=(const CharacterMetrics& source) {
	this->notepadForm = source.notepadForm;

	Long i = 0;
	while (i < 129) {
		this->widths[i] = source.widths[i];
		i++;
	}

	this->height = source.height;

	return *this;
}

Long CharacterMetrics::GetColumn(Glyph* line, Long x) {
	Long index = 0;
	Long currentX = 0;
	Long previousX = -1;
	Glyph* character;
	string content;
	Long asciiIndex;
	Long width;

	while (index < line->GetLength() && currentX < x) {
		character = line->GetAt(index);
		content = character->GetContent();

		if (dynamic_cast<SingleByteCharacter*>(character)) {
			if (content[1] == '\0') { // ���� �ƴ� ��
				asciiIndex = content[0];
			}
			else {
				asciiIndex = 9;
			}
			width = this->widths[asciiIndex];
		}
		else if (dynamic_cast<DoubleByteCharacter*>(character)) {
			width = this->widths[128];
		}
		previousX = currentX;
		currentX += width;
		index++;
	}

	if ((x - previousX) < (currentX - x)) {
		index--;
	}

	return index;
}

Long CharacterMetrics::GetRow(Long y) {
	return y / this->height;
}

Long CharacterMetrics::GetNoteWidth(Glyph* note) {
	Long i = 0;
	Long j;
	Glyph* line;
	Glyph* character;
	Long lineWidth;
	Long noteWidth = 0;
	string content;

	while (i < note->GetLength()) {
		line = note->GetAt(i);
		lineWidth = 0;
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			content = character->GetContent();

			if (dynamic_cast<SingleByteCharacter*> (character)) {
				if (content.length() == 1) {
					lineWidth += this->widths[content[0]];
				}
				else {
					lineWidth += this->widths[9];
				}
			}
			else if (dynamic_cast<DoubleByteCharacter*>(character)) {
				lineWidth += this->widths[128];
			}
			j++;
		}
		if (noteWidth < lineWidth) {
			noteWidth = lineWidth;
		}
		i++;
	}

	return noteWidth;
}

Long CharacterMetrics::GetWidthAverage() {
	CDC* dc = this->notepadForm->GetDC();
	TEXTMETRICA textmetrica;
	memset(&textmetrica, 0, sizeof(textmetrica));
	dc->GetTextMetrics(&textmetrica);

	return textmetrica.tmAveCharWidth;
}