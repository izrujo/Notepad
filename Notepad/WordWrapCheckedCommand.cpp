#include "WordWrapCheckedCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "CharacterMetrics.h"
#include "GlyphFactory.h"
#include "ScrollController.h"
#include "Scroll.h"
#include <afxwin.h>

WordWrapCheckedCommand::WordWrapCheckedCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

WordWrapCheckedCommand::WordWrapCheckedCommand(const WordWrapCheckedCommand& source)
	: Command(source) {
}

WordWrapCheckedCommand::~WordWrapCheckedCommand() {
}

void WordWrapCheckedCommand::Execute() {
	RECT rect;
	this->notepadForm->GetClientRect(&rect);
	Long clientWidth = rect.right - rect.left;
	Long lineWidth = 0;
	Glyph *line;
	Long noteLength = this->notepadForm->note->GetLength();
	Long i = 0;
	while (i < noteLength) {
		line = this->notepadForm->note->GetAt(i);
		lineWidth = this->notepadForm->characterMetrics->GetX(line, line->GetLength());
		if (lineWidth > clientWidth) {
			GlyphFactory glyphFactory(this->notepadForm);
			Glyph *dummyLine = glyphFactory.Make(line, clientWidth);
			if (i < noteLength) {
				this->notepadForm->note->Add(i + 1, dummyLine);
			}
			else {
				this->notepadForm->note->Add(dummyLine);
			}
		}
		noteLength = this->notepadForm->note->GetLength();
		i++;
	}
	this->notepadForm->note->First();
	this->notepadForm->current->First();
}

WordWrapCheckedCommand& WordWrapCheckedCommand::operator =(const WordWrapCheckedCommand& source) {
	Command::operator =(source);

	return *this;
}