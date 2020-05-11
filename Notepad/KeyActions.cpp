#include "KeyActions.h"
#include "KeyActionFactory.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "Character.h"
#include "Note.h"
#include "Line.h"
#include "Composite.h"
#include "ScrollController.h"
#include "CaretController.h"
#include "Scroll.h"
#include <string>

//KeyAction
KeyAction::KeyAction(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

KeyAction::KeyAction(const KeyAction& source) {
	this->notepadForm = source.notepadForm;
}

KeyAction::~KeyAction() {
}

KeyAction& KeyAction::operator =(const KeyAction& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

//LeftKeyAction
LeftKeyAction::LeftKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {

}

LeftKeyAction::LeftKeyAction(const LeftKeyAction& source)
	: KeyAction(notepadForm) {

}

LeftKeyAction::~LeftKeyAction() {

}

void LeftKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->current->GetCurrent() > 0) {
		this->notepadForm->current->Previous();
	}
	else if (this->notepadForm->note->GetCurrent() > 0) {
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->Last();
	}

}

LeftKeyAction& LeftKeyAction::operator = (const LeftKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//RightKeyAction
RightKeyAction::RightKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

RightKeyAction::RightKeyAction(const RightKeyAction& source)
	: KeyAction(notepadForm) {
}

RightKeyAction::~RightKeyAction() {
}

void RightKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->current->GetCurrent() < this->notepadForm
		->current->GetLength()) {
		this->notepadForm->current->Next();
	}
	else if (this->notepadForm->note->GetCurrent() < this->notepadForm
		->note->GetLength() - 1) {
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->First();
	}
}

RightKeyAction& RightKeyAction::operator=(const RightKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//UpKeyAction
UpKeyAction::UpKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {

}

UpKeyAction::UpKeyAction(const UpKeyAction& source)
	: KeyAction(source) {

}

UpKeyAction::~UpKeyAction() {

}

void UpKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() > 0) {
		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);
	}
}

UpKeyAction& UpKeyAction::operator = (const UpKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//DownKeyAction
DownKeyAction::DownKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {

}

DownKeyAction::DownKeyAction(const DownKeyAction& source)
	: KeyAction(notepadForm) {

}

DownKeyAction::~DownKeyAction() {

}

void DownKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {
		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);

	}
}

DownKeyAction& DownKeyAction::operator=(const DownKeyAction& source) {
	KeyAction::operator=(source);
	return *this;

}

//HomeKeyAction
HomeKeyAction::HomeKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

HomeKeyAction::HomeKeyAction(const HomeKeyAction& source)
	: KeyAction(source) {
}

HomeKeyAction::~HomeKeyAction() {
}

void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->current->First();
}

HomeKeyAction& HomeKeyAction::operator =(const HomeKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//EndKeyAction
EndKeyAction::EndKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

EndKeyAction::EndKeyAction(const EndKeyAction& source)
	: KeyAction(source) {
}

EndKeyAction::~EndKeyAction() {
}

void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->current->Last();
}

EndKeyAction& EndKeyAction::operator =(const EndKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlLeftKeyAction
CtrlLeftKeyAction::CtrlLeftKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlLeftKeyAction::CtrlLeftKeyAction(const CtrlLeftKeyAction& source)
	: KeyAction(source) {
}

CtrlLeftKeyAction::~CtrlLeftKeyAction() {
}

void CtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

CtrlLeftKeyAction& CtrlLeftKeyAction::operator =(const CtrlLeftKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlRightKeyAction
CtrlRightKeyAction::CtrlRightKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlRightKeyAction::CtrlRightKeyAction(const CtrlRightKeyAction& source)
	: KeyAction(source) {
}

CtrlRightKeyAction::~CtrlRightKeyAction() {
}

void CtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

CtrlRightKeyAction& CtrlRightKeyAction::operator =(const CtrlRightKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlHomeKeyAction
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlHomeKeyAction::CtrlHomeKeyAction(const CtrlHomeKeyAction& source)
	: KeyAction(source) {
}

CtrlHomeKeyAction::~CtrlHomeKeyAction() {
}

void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();
}

CtrlHomeKeyAction& CtrlHomeKeyAction::operator =(const CtrlHomeKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlEndKeyAction
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

CtrlEndKeyAction::CtrlEndKeyAction(const CtrlEndKeyAction& source)
	: KeyAction(source) {
}

CtrlEndKeyAction::~CtrlEndKeyAction() {
}

void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long index = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();
}

CtrlEndKeyAction& CtrlEndKeyAction::operator =(const CtrlEndKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//DeleteKeyAction
DeleteKeyAction::DeleteKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

DeleteKeyAction::DeleteKeyAction(const DeleteKeyAction& source)
	: KeyAction(source) {
}

DeleteKeyAction::~DeleteKeyAction() {
}

void DeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long column = this->notepadForm->current->GetCurrent();
	Long lineLength = this->notepadForm->current->GetLength();
	if (column < lineLength) {
		this->notepadForm->current->Remove(column);
	}
	Long row = this->notepadForm->note->GetCurrent();
	Long noteLength = this->notepadForm->note->GetLength();
	if (column >= lineLength && row < noteLength - 1) {
		Glyph* other = this->notepadForm->note->GetAt(row + 1);
		this->notepadForm->current->Combine(other);
		this->notepadForm->note->Remove(row + 1);
	}
}

DeleteKeyAction& DeleteKeyAction::operator =(const DeleteKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//BackSpaceKeyAction
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm *notepadForm)
	: KeyAction(notepadForm) {
}

BackSpaceKeyAction::BackSpaceKeyAction(const BackSpaceKeyAction& source)
	: KeyAction(source) {
}

BackSpaceKeyAction::~BackSpaceKeyAction() {
}

void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->GetIsComposing() == FALSE) {
		Long lineCurrent = this->notepadForm->current->GetCurrent();
		Long noteCurrent = this->notepadForm->note->GetCurrent();
		if (lineCurrent > 0) {
			this->notepadForm->current->Remove(lineCurrent - 1);
		}
		else if (lineCurrent <= 0 && noteCurrent > 0) {
			Glyph *previousLine = this->notepadForm->note->GetAt(noteCurrent - 1);
			Long index = previousLine->GetLength();
			this->notepadForm->current = previousLine->Combine(this->notepadForm->current);
			this->notepadForm->note->Remove(noteCurrent);
			this->notepadForm->current->Move(index);
		}
	}
}

BackSpaceKeyAction& BackSpaceKeyAction::operator =(const BackSpaceKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//PageUpKeyAction
PageUpKeyAction::PageUpKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {

}

PageUpKeyAction::PageUpKeyAction(const PageUpKeyAction& source)
	: KeyAction(source) {

}

PageUpKeyAction::~PageUpKeyAction() {

}

void PageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long position = this->notepadForm->scrollController->PageUp();
	Long previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다. 따라서 현재 x값을 다시 더한다.
	Long x = this->notepadForm->caretController->GetCaretX() +
		this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다.따라서 이전 y값을 다시 더한다.
	Long y = this->notepadForm->caretController->GetCaretY() + previous;

	Long row = this->notepadForm->characterMetrics->GetRow(y - (previous - position));
	if (row < 0) {
		row = 0;
	}
	Long index = this->notepadForm->note->Move(row); // row가 벗어나는 경우는 없나?
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
	this->notepadForm->current->Move(column);

}

PageUpKeyAction& PageUpKeyAction::operator = (const PageUpKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//PageDownKeyAction
PageDownKeyAction::PageDownKeyAction(NotepadForm *notepadForm)
	:KeyAction(notepadForm) {
}

PageDownKeyAction::PageDownKeyAction(const PageDownKeyAction& source)
	: KeyAction(source) {

}

PageDownKeyAction::~PageDownKeyAction() {
}

void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long position = this->notepadForm->scrollController->PageDown();
	Long previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	this->notepadForm->ScrollWindow(0, previous - position);
	Long x = this->notepadForm->caretController->GetCaretX() +
		this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
	Long y = this->notepadForm->caretController->GetCaretY() + previous;
	Long row = this->notepadForm->characterMetrics->GetRow(y - (previous - position));
	if (row > this->notepadForm->note->GetLength() - 1) {
		row = this->notepadForm->note->GetLength() - 1;
	}
	Long index = this->notepadForm->note->Move(row);
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
	this->notepadForm->current->Move(column);
}

PageDownKeyAction& PageDownKeyAction::operator=(const PageDownKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}