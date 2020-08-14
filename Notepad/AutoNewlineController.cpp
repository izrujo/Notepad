#include "AutoNewlineController.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "DummyLine.h"
#include "CharacterMetrics.h"

AutoNewlineController::AutoNewlineController(NotepadForm* notepadForm)
	: Observer() {
	this->notepadForm = notepadForm;
	this->notepadForm->AttachObserver(this);
}

AutoNewlineController::AutoNewlineController(const AutoNewlineController& source)
	: Observer(source) {
	this->notepadForm = source.notepadForm;
	this->notepadForm->AttachObserver(this);
}

AutoNewlineController::~AutoNewlineController() {
	this->notepadForm->DetachObserver(this);
}

AutoNewlineController& AutoNewlineController::operator=(const AutoNewlineController& source) {
	Observer::operator=(source);
	this->notepadForm = source.notepadForm;

	return *this;
}

void AutoNewlineController::Update() {
	Long row = this->notepadForm->note->GetCurrent();
	Long column = this->notepadForm->current->GetCurrent();

	Glyph* line;
	Glyph* previous;
	Long i = this->notepadForm->note->GetLength() - 1;
	while (i > 0) {
		previous = this->notepadForm->note->GetAt(i - 1);
		line = this->notepadForm->note->GetAt(i);
		if (dynamic_cast<DummyLine*>(line)) {
			if (row >= i) {
				if (row == i) {
					column += previous->GetLength();
				}
				row--;
			}
			previous->Combine(line);
			this->notepadForm->note->Remove(i);
		}
		i--;
	}

	Long characterwidthAverage = this->notepadForm->characterMetrics->GetWidthAverage();
	RECT clientRect;
	this->notepadForm->GetClientRect(&clientRect);
	Long clientWidth = clientRect.right - clientRect.left - characterwidthAverage;

	Glyph* next;
	Glyph* dummy;
	Long cutColumn;
	Long width;
	Long j;
	i = 0;
	while (i < this->notepadForm->note->GetLength()) {
		line = this->notepadForm->note->GetAt(i);
		width = this->notepadForm->characterMetrics->GetX(line, line->GetLength());
		if (width >= clientWidth) {
			dummy = new DummyLine;
			cutColumn = this->notepadForm->characterMetrics->GetColumn(line, clientWidth);
			next = line->Divide(cutColumn);
			j = 0;
			while (j < next->GetLength()) {
				dummy->Add(next->GetAt(j)->Clone());
				j++;
			}
			this->notepadForm->note->Add(i + 1, dummy);
			
			if (row > i) {
				row++;
			}
			else if (row == i && column > cutColumn) {
				row++;
				column -= cutColumn;
			}
			
			if (next != 0) {
				delete next;
			}
		}
		i++;
	}


	this->notepadForm->note->Move(row);
	this->notepadForm->current = this->notepadForm->note->GetAt(row);
	this->notepadForm->current->Move(column);

	this->notepadForm->Invalidate();
}

void AutoNewlineController::Release() {
	Long row = this->notepadForm->note->GetCurrent();
	Long column = this->notepadForm->current->GetCurrent();

	Glyph* line;
	Glyph* previous;
	Long i = this->notepadForm->note->GetLength() - 1;
	while (i > 0) {
		previous = this->notepadForm->note->GetAt(i - 1);
		line = this->notepadForm->note->GetAt(i);
		if (dynamic_cast<DummyLine*>(line)) {
			if (row >= i) {
				if (row == i) {
					column += previous->GetLength();
				}
				row--;
			}
			previous->Combine(line);
			this->notepadForm->note->Remove(i);
		}
		i--;
	}

	this->notepadForm->note->Move(row);
	this->notepadForm->current = this->notepadForm->note->GetAt(row);
	this->notepadForm->current->Move(column);
}