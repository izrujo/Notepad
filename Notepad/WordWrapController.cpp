#include "WordWrapController.h"
#include "NotepadForm.h"
#include "Subject.h"
#include "resource.h"
#include "Note.h"
#include "DummyLine.h"
#include "CharacterMetrics.h"
#include "GlyphFactory.h"
//#include "WordWrapUnCheckedCommand.h"
//#include "WordWrapCheckedCommand.h"
//#include "Command.h"

WordWrapController::WordWrapController(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
	this->notepadForm->AttachObserver(this);
}

WordWrapController::WordWrapController(const WordWrapController& source) {
	this->notepadForm = source.notepadForm;
}

WordWrapController::~WordWrapController() {
	this->notepadForm->DetachObserver(this);
}

void WordWrapController::Update() {
	/*
	Command *command;
	command = new WordWrapUnCheckedCommand(this->notepadForm);
	command->Execute();
	if (command != 0) {
		delete command;
	}
	command = new WordWrapCheckedCommand(this->notepadForm);
	command->Execute();
	if (command != 0) {
		delete command;
	}
	*/
	//자동 개행 해제
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
	//자동 개행 재설정
	RECT rect;
	this->notepadForm->GetClientRect(&rect);
	Long clientWidth = rect.right - rect.left;
	Long lineWidth = 0;
	//Glyph *line;
	Long index;
	Long noteLength = this->notepadForm->note->GetLength();
	i = 0;
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
}

WordWrapController& WordWrapController::operator =(const WordWrapController& source) {
	this->notepadForm = source.notepadForm;
	
	return *this;
}