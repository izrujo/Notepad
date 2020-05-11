#include "WordWrapUnCheckedCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "Glyph.h"
#include "DummyLine.h"
#include "ScrollController.h"
#include "Scroll.h"

WordWrapUnCheckedCommand::WordWrapUnCheckedCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

WordWrapUnCheckedCommand::WordWrapUnCheckedCommand(const WordWrapUnCheckedCommand& source)
	: Command(source) {
}

WordWrapUnCheckedCommand::~WordWrapUnCheckedCommand() {
}

void WordWrapUnCheckedCommand::Execute() {
	Glyph *line;
	Glyph *dummyLine;
	Long i = 0;
	while (i < this->notepadForm->note->GetLength() && this->notepadForm->note->GetLength() > 1) {
		line = this->notepadForm->note->GetAt(i);
		Long j = i + 1;
		while (j < this->notepadForm->note->GetLength() && 
			dynamic_cast<DummyLine*>(this->notepadForm->note->GetAt(j))) {
			dummyLine = this->notepadForm->note->GetAt(j);
			line->Combine(dummyLine);
			this->notepadForm->note->Remove(j);
		}
		i++;
	}
	this->notepadForm->note->First();
	this->notepadForm->current->First();
}

WordWrapUnCheckedCommand& WordWrapUnCheckedCommand::operator =(const WordWrapUnCheckedCommand& source) {
	Command::operator =(source);

	return *this;
}