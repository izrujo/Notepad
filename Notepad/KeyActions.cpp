#include "KeyActions.h"
#include "KeyActionFactory.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "Characters.h"
#include "Note.h"
#include "Line.h"
#include "Composite.h"
#include "ScrollController.h"
#include "CaretController.h"
#include "Scroll.h"
#include "Highlight.h"
#include "Editor.h"
#include "Selector.h"

#include "resource.h"

#include <string>

//KeyAction
KeyAction::KeyAction(NotepadForm* notepadForm) {
	this->notepadForm = notepadForm;
}

KeyAction::KeyAction(const KeyAction& source) {
	this->notepadForm = source.notepadForm;
}

KeyAction::~KeyAction() {

}

KeyAction& KeyAction::operator=(const KeyAction& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

//LeftKeyAction
LeftKeyAction::LeftKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {

}

LeftKeyAction::LeftKeyAction(const LeftKeyAction& source)
	: KeyAction(source) {

}

LeftKeyAction::~LeftKeyAction() {

}

LeftKeyAction& LeftKeyAction::operator=(const LeftKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

void LeftKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;

		Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
		Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
		Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
		Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
		//시작지점이 끝지점보다 작으면 시작지점으로 이동한다.
		if (noteStartPosition < noteEndPosition ||
			(noteStartPosition == noteEndPosition && lineStartPosition < lineEndPosition)) {
			this->notepadForm->note->Move(noteStartPosition);
			this->notepadForm->current = this->notepadForm->note->GetAt(noteStartPosition);
			this->notepadForm->current->Move(lineStartPosition);
		}
		//시작지점이 끝지점보다 크면 이동하지 않는다.
	}
	else {
		if (this->notepadForm->current->GetCurrent() > 0) {
			this->notepadForm->current->Previous();
		}
		else if (this->notepadForm->note->GetCurrent() > 0) {
			Long index = this->notepadForm->note->Previous();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->Last();
		}
	}
}

//RightKeyAction
RightKeyAction::RightKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

RightKeyAction::RightKeyAction(const RightKeyAction& source)
	: KeyAction(source) {
}

RightKeyAction::~RightKeyAction() {

}

RightKeyAction& RightKeyAction::operator=(const RightKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

void RightKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;

		Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
		Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
		Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
		Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
		//시작지점이 끝지점보다 크면 시작지점으로 이동한다.
		if (noteStartPosition > noteEndPosition ||
			(noteStartPosition == noteEndPosition && lineStartPosition > lineEndPosition)) {
			this->notepadForm->note->Move(noteStartPosition);
			this->notepadForm->current = this->notepadForm->note->GetAt(noteStartPosition);
			this->notepadForm->current->Move(lineStartPosition);
		}
		//시작지점이 끝지점보다 작으면 이동하지 않는다.
	}
	else {
		if (this->notepadForm->current->GetCurrent() < this->notepadForm->current->GetLength()) {
			this->notepadForm->current->Next();
		}
		else if (this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {
			Long index = this->notepadForm->note->Next();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->First();
		}
	}
}

//UpKeyAction
UpKeyAction::UpKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {

}

UpKeyAction::UpKeyAction(const UpKeyAction& source)
	: KeyAction(source) {

}

UpKeyAction::~UpKeyAction() {

}

UpKeyAction& UpKeyAction::operator=(const UpKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

void UpKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() > 0) {
		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);
	}

	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}
}

//DownKeyAction
DownKeyAction::DownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm) {

}

DownKeyAction::DownKeyAction(const DownKeyAction& source)
	: KeyAction(source) {

}

DownKeyAction::~DownKeyAction() {

}

DownKeyAction& DownKeyAction::operator=(const DownKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void DownKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	if (this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {
		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);
	}

	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}
}

//HomeKeyAction
HomeKeyAction::HomeKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

HomeKeyAction::HomeKeyAction(const HomeKeyAction& source)
	: KeyAction(source) {
}

HomeKeyAction::~HomeKeyAction() {

}

HomeKeyAction& HomeKeyAction::operator=(const HomeKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void HomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

	this->notepadForm->current->First();
}

//EndKeyAction
EndKeyAction::EndKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

EndKeyAction::EndKeyAction(const EndKeyAction& source)
	: KeyAction(source) {
}

EndKeyAction::~EndKeyAction() {

}

EndKeyAction& EndKeyAction::operator=(const EndKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void EndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

	this->notepadForm->current->Last();
}

//CtrlLeftKeyAction
CtrlLeftKeyAction::CtrlLeftKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlLeftKeyAction::CtrlLeftKeyAction(const CtrlLeftKeyAction& source)
	: KeyAction(source) {
}

CtrlLeftKeyAction::~CtrlLeftKeyAction() {

}

CtrlLeftKeyAction& CtrlLeftKeyAction::operator=(const CtrlLeftKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void CtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

	Long index = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

//CtrlRightKeyAction
CtrlRightKeyAction::CtrlRightKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlRightKeyAction::CtrlRightKeyAction(const CtrlRightKeyAction& source)
	: KeyAction(source) {
}

CtrlRightKeyAction::~CtrlRightKeyAction() {

}

CtrlRightKeyAction& CtrlRightKeyAction::operator=(const CtrlRightKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void CtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

	Long index = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

//CtrlHomeKeyAction
CtrlHomeKeyAction::CtrlHomeKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlHomeKeyAction::CtrlHomeKeyAction(const CtrlHomeKeyAction& source)
	: KeyAction(source) {
}

CtrlHomeKeyAction::~CtrlHomeKeyAction() {

}

CtrlHomeKeyAction& CtrlHomeKeyAction::operator=(const CtrlHomeKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void CtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

	Long index = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();
}

//CtrlEndKeyAction
CtrlEndKeyAction::CtrlEndKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlEndKeyAction::CtrlEndKeyAction(const CtrlEndKeyAction& source)
	: KeyAction(source) {
}

CtrlEndKeyAction::~CtrlEndKeyAction() {

}

CtrlEndKeyAction& CtrlEndKeyAction::operator=(const CtrlEndKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void CtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

	Long index = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();
}

//DeleteKeyAction
DeleteKeyAction::DeleteKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

DeleteKeyAction::DeleteKeyAction(const DeleteKeyAction& source)
	: KeyAction(source) {
}

DeleteKeyAction::~DeleteKeyAction() {

}

DeleteKeyAction& DeleteKeyAction::operator=(const DeleteKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void DeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_DELETE_CHAR, 0));
	}
	else {
		this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
	}
}

//BackSpaceKeyAction
BackSpaceKeyAction::BackSpaceKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

BackSpaceKeyAction::BackSpaceKeyAction(const BackSpaceKeyAction& source)
	: KeyAction(source) {
}

BackSpaceKeyAction::~BackSpaceKeyAction() {

}

BackSpaceKeyAction& BackSpaceKeyAction::operator=(const BackSpaceKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}

void BackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BACKSPACE_CHAR, 0));
	}
	else {
		this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
	}
}

//PageUpKeyAction
PageUpKeyAction::PageUpKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm) {

}

PageUpKeyAction::PageUpKeyAction(const PageUpKeyAction& source)
	: KeyAction(source) {

}

PageUpKeyAction::~PageUpKeyAction() {

}

PageUpKeyAction& PageUpKeyAction::operator=(const PageUpKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

void PageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

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

//PageDownKeyAction
PageDownKeyAction::PageDownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm) {
}

PageDownKeyAction::PageDownKeyAction(const PageDownKeyAction& source)
	: KeyAction(source) {

}

PageDownKeyAction::~PageDownKeyAction() {

}

PageDownKeyAction& PageDownKeyAction::operator=(const PageDownKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

void PageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}

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

//ShiftLeftKeyAction
ShiftLeftKeyAction::ShiftLeftKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm) {

}

ShiftLeftKeyAction::ShiftLeftKeyAction(const ShiftLeftKeyAction& source)
	: KeyAction(notepadForm) {

}

ShiftLeftKeyAction::~ShiftLeftKeyAction() {

}

void ShiftLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}
	if (lineCurrent > 0) {
		Long endColumn = this->notepadForm->current->Previous();
		Long startColumn = lineCurrent;
		Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
		Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
		Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
		Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
		if (noteStartPosition < noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition < lineEndPosition)) {
			startColumn = endColumn;
		}
		this->notepadForm->editor->selector->Left(noteCurrent, startColumn, endColumn);
	}
	else if (noteCurrent > 0) {
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long endColumn = this->notepadForm->current->Last();
		this->notepadForm->editor->selector->Left(index, endColumn, endColumn);
	}
}

ShiftLeftKeyAction& ShiftLeftKeyAction::operator = (const ShiftLeftKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//ShiftRightKeyAction
ShiftRightKeyAction::ShiftRightKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm) {
}

ShiftRightKeyAction::ShiftRightKeyAction(const ShiftRightKeyAction& source)
	: KeyAction(notepadForm) {
}

ShiftRightKeyAction::~ShiftRightKeyAction() {
}

void ShiftRightKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}

	if (lineCurrent < this->notepadForm->current->GetLength()) {
		Long endColumn = this->notepadForm->current->Next();

		Long startColumn = lineCurrent;
		Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
		Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
		Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
		Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
		if (noteStartPosition > noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition > lineEndPosition)) {
			startColumn = endColumn;
		}
		this->notepadForm->editor->selector->Right(noteCurrent, startColumn, endColumn);
	}
	else if (noteCurrent < this->notepadForm->note->GetLength() - 1) {
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long endColumn = this->notepadForm->current->First();

		this->notepadForm->editor->selector->Right(index, endColumn, endColumn);
	}
}

ShiftRightKeyAction& ShiftRightKeyAction::operator=(const ShiftRightKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//ShiftUpKeyAction
ShiftUpKeyAction::ShiftUpKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm) {

}

ShiftUpKeyAction::ShiftUpKeyAction(const ShiftUpKeyAction& source)
	: KeyAction(source) {

}

ShiftUpKeyAction::~ShiftUpKeyAction() {

}

void ShiftUpKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (noteCurrent > 0) {
		if (this->notepadForm->highlight == NULL) {
			this->notepadForm->highlight = new Highlight;
			this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
		}

		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);

		this->notepadForm->editor->UpSelect(noteCurrent, lineCurrent, index, column);
	}
}

ShiftUpKeyAction& ShiftUpKeyAction::operator = (const ShiftUpKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//ShiftDownKeyAction
ShiftDownKeyAction::ShiftDownKeyAction(NotepadForm* notepadForm)
	:KeyAction(notepadForm) {

}

ShiftDownKeyAction::ShiftDownKeyAction(const ShiftDownKeyAction& source)
	: KeyAction(notepadForm) {

}

ShiftDownKeyAction::~ShiftDownKeyAction() {

}

void ShiftDownKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (noteCurrent < this->notepadForm->note->GetLength() - 1) {
		if (this->notepadForm->highlight == NULL) {
			this->notepadForm->highlight = new Highlight;
			this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
		}

		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);

		this->notepadForm->editor->DownSelect(noteCurrent, lineCurrent, index, column);
	}
}

ShiftDownKeyAction& ShiftDownKeyAction::operator=(const ShiftDownKeyAction& source) {
	KeyAction::operator=(source);
	return *this;
}

//ShiftHomeKeyAction
ShiftHomeKeyAction::ShiftHomeKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

ShiftHomeKeyAction::ShiftHomeKeyAction(const ShiftHomeKeyAction& source)
	: KeyAction(source) {
}

ShiftHomeKeyAction::~ShiftHomeKeyAction() {
}

void ShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}

	Long endColumn = this->notepadForm->current->First();

	Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
	Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
	Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
	Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
	Long startColumn = lineEndPosition;
	if (noteStartPosition == noteEndPosition && lineStartPosition < lineEndPosition) {
		startColumn = lineStartPosition;
	}
	else if (noteStartPosition < noteEndPosition) {
		startColumn = 0;
	}
	this->notepadForm->editor->selector->Left(noteCurrent, startColumn, endColumn);
}

ShiftHomeKeyAction& ShiftHomeKeyAction::operator =(const ShiftHomeKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//ShiftEndKeyAction
ShiftEndKeyAction::ShiftEndKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

ShiftEndKeyAction::ShiftEndKeyAction(const ShiftEndKeyAction& source)
	: KeyAction(source) {
}

ShiftEndKeyAction::~ShiftEndKeyAction() {
}

void ShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}

	Long endColumn = this->notepadForm->current->Last();

	Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
	Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
	Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
	Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
	Long startColumn = lineEndPosition;
	if (noteStartPosition == noteEndPosition && lineStartPosition > lineEndPosition) {
		startColumn = lineStartPosition;
	}
	else if (noteStartPosition > noteEndPosition) {
		startColumn = this->notepadForm->current->GetLength();
	}
	this->notepadForm->editor->selector->Right(noteCurrent, startColumn, endColumn);
}

ShiftEndKeyAction& ShiftEndKeyAction::operator =(const ShiftEndKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//ShiftCtrlLeftKeyAction
ShiftCtrlLeftKeyAction::ShiftCtrlLeftKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

ShiftCtrlLeftKeyAction::ShiftCtrlLeftKeyAction(const ShiftCtrlLeftKeyAction& source)
	: KeyAction(source) {
}

ShiftCtrlLeftKeyAction::~ShiftCtrlLeftKeyAction() {
}

void ShiftCtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}

	Long index = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);

	Long endColumn = this->notepadForm->current->GetCurrent();
	Long startColumn = lineCurrent;
	Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
	Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
	Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
	Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
	if (noteCurrent > index || noteStartPosition < noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition < lineEndPosition)) {
		if (noteStartPosition == index && endColumn < lineStartPosition) {
			startColumn = lineStartPosition;
		}
		else {
			startColumn = endColumn;
		}
	}
	this->notepadForm->editor->selector->Left(index, startColumn, endColumn);
}

ShiftCtrlLeftKeyAction& ShiftCtrlLeftKeyAction::operator =(const ShiftCtrlLeftKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//ShiftCtrlRightKeyAction
ShiftCtrlRightKeyAction::ShiftCtrlRightKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

ShiftCtrlRightKeyAction::ShiftCtrlRightKeyAction(const ShiftCtrlRightKeyAction& source)
	: KeyAction(source) {
}

ShiftCtrlRightKeyAction::~ShiftCtrlRightKeyAction() {
}

void ShiftCtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}

	Long index = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);

	Long endColumn = this->notepadForm->current->GetCurrent();
	Long startColumn = lineCurrent;
	Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
	Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
	Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
	Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
	if (noteCurrent < index || noteStartPosition > noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition > lineEndPosition)) {
		if (noteStartPosition == index && endColumn > lineStartPosition) {
			startColumn = lineStartPosition;
		}
		else {
			startColumn = endColumn;
		}
	}
	this->notepadForm->editor->selector->Right(index, startColumn, endColumn);
}

ShiftCtrlRightKeyAction& ShiftCtrlRightKeyAction::operator =(const ShiftCtrlRightKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//ShiftCtrlHomeKeyAction
ShiftCtrlHomeKeyAction::ShiftCtrlHomeKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

ShiftCtrlHomeKeyAction::ShiftCtrlHomeKeyAction(const ShiftCtrlHomeKeyAction& source)
	: KeyAction(source) {
}

ShiftCtrlHomeKeyAction::~ShiftCtrlHomeKeyAction() {
}

void ShiftCtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}

	Long index = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->current->First();

	this->notepadForm->editor->UpSelect(noteCurrent, lineCurrent, index, column);
}

ShiftCtrlHomeKeyAction& ShiftCtrlHomeKeyAction::operator =(const ShiftCtrlHomeKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//ShiftCtrlEndKeyAction
ShiftCtrlEndKeyAction::ShiftCtrlEndKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

ShiftCtrlEndKeyAction::ShiftCtrlEndKeyAction(const ShiftCtrlEndKeyAction& source)
	: KeyAction(source) {
}

ShiftCtrlEndKeyAction::~ShiftCtrlEndKeyAction() {
}

void ShiftCtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
	}

	Long index = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->current->Last();

	this->notepadForm->editor->DownSelect(noteCurrent, lineCurrent, index, column);
}

ShiftCtrlEndKeyAction& ShiftCtrlEndKeyAction::operator =(const ShiftCtrlEndKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlAKeyAction
CtrlAKeyAction::CtrlAKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlAKeyAction::CtrlAKeyAction(const CtrlAKeyAction& source)
	: KeyAction(source) {
}

CtrlAKeyAction::~CtrlAKeyAction() {
}

void CtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_SELECTALL, 0));
}

CtrlAKeyAction& CtrlAKeyAction::operator =(const CtrlAKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlCKeyAction
CtrlCKeyAction::CtrlCKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlCKeyAction::CtrlCKeyAction(const CtrlCKeyAction& source)
	: KeyAction(source) {
}

CtrlCKeyAction::~CtrlCKeyAction() {
}

void CtrlCKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_COPY, 0));
}

CtrlCKeyAction& CtrlCKeyAction::operator =(const CtrlCKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlVKeyAction
CtrlVKeyAction::CtrlVKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlVKeyAction::CtrlVKeyAction(const CtrlVKeyAction& source)
	: KeyAction(source) {
}

CtrlVKeyAction::~CtrlVKeyAction() {
}

void CtrlVKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_PASTE, 0));
}

CtrlVKeyAction& CtrlVKeyAction::operator =(const CtrlVKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlXKeyAction
CtrlXKeyAction::CtrlXKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlXKeyAction::CtrlXKeyAction(const CtrlXKeyAction& source)
	: KeyAction(source) {
}

CtrlXKeyAction::~CtrlXKeyAction() {
}

void CtrlXKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_CUT, 0));
}

CtrlXKeyAction& CtrlXKeyAction::operator =(const CtrlXKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}