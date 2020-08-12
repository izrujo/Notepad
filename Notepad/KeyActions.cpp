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
#include "Selection.h"

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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_LEFT, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_RIGHT, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_UP, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_DOWN, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_HOME, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_END, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_CTRLLEFT, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_CTRLRIGHT, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_CTRLHOME, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_CTRLEND, 0));
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
	if (this->notepadForm->selection == NULL) {
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
	if (this->notepadForm->selection == NULL) {
		if (this->notepadForm->note->GetCurrent() > 0 || this->notepadForm->current->GetCurrent() > 0) {
			this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_LEFT, 0));
			this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_DELETE_CHAR, 0));
		}
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_PAGEUP, 0));
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_PAGEDOWN, 0));
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
	Glyph* character;
	Long column;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (lineCurrent > 0) {
		column = this->notepadForm->current->Previous();
		character = this->notepadForm->current->GetAt(column);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
	}
	else if (noteCurrent > 0) {
		row = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(row);
		this->notepadForm->current->Last();
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
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
	Glyph* character;
	Long column;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (lineCurrent < this->notepadForm->current->GetLength()) {
		column = this->notepadForm->current->Next();
		character = this->notepadForm->current->GetAt(column - 1);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
	}
	else if (noteCurrent < this->notepadForm->note->GetLength() - 1) {
		row = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(row);
		this->notepadForm->current->First();
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
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
	Glyph* character;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	if (noteCurrent > 0) {
		Long i = lineCurrent;
		while (i > 0) {
			character = this->notepadForm->current->GetAt(i - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i--;
		}

		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		row = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(row);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);

		i = this->notepadForm->current->GetLength();
		while (i > column) {
			character = this->notepadForm->current->GetAt(i - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i--;
		}
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
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
	Glyph* character;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	if (noteCurrent < this->notepadForm->note->GetLength() - 1) {
		Long i = lineCurrent;
		while (i < this->notepadForm->current->GetLength()) {
			character = this->notepadForm->current->GetAt(i);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i++;
		}

		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		row = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(row);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);

		i = 0;
		while (i < column) {
			character = this->notepadForm->current->GetAt(i);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i++;
		}
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
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
	Glyph* character;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Long index = this->notepadForm->current->First();
	Long i = lineCurrent;
	while (i > index) {
		character = this->notepadForm->current->GetAt(i - 1);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		i--;
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
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
	Glyph* character;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Long index = this->notepadForm->current->Last();
	Long i = lineCurrent;
	while (i < index) {
		character = this->notepadForm->current->GetAt(i);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		i++;
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
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
	Glyph* character;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;

	Long lineCurrent = this->notepadForm->current->GetCurrent();

	row = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(row);

	Long lineNext = this->notepadForm->current->GetCurrent();
	Long i = lineCurrent;
	while (i > lineNext) {
		character = this->notepadForm->current->GetAt(i - 1);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		i--;
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
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
	Glyph* character;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;

	Long lineCurrent = this->notepadForm->current->GetCurrent();

	row = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(row);

	Long lineNext = this->notepadForm->current->GetCurrent();
	Long i = lineCurrent;
	while (i < lineNext) {
		character = this->notepadForm->current->GetAt(i);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		i++;
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
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
	Glyph* character;
	Glyph* line;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();

	Long row = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(row);
	this->notepadForm->current->First();

	Long j;
	Long i = noteCurrent;
	while (i >= row) {
		line = this->notepadForm->note->GetAt(i);
		j = line->GetLength();
		if (i == noteCurrent) {
			j = lineCurrent;
		}
		while (j > 0) {
			character = line->GetAt(j - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			j--;
		}
		i--;
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
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
	Glyph* character;
	Glyph* line;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long lineCurrent = this->notepadForm->current->GetCurrent();

	Long row = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(row);
	this->notepadForm->current->Last();

	Long j;
	Long i = noteCurrent;
	while (i <= row) {
		line = this->notepadForm->note->GetAt(i);
		j = 0;
		if (i == noteCurrent) {
			j = lineCurrent;
		}
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			j++;
		}
		i++;
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
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
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_CUT, 0));
	}
}

CtrlXKeyAction& CtrlXKeyAction::operator =(const CtrlXKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlZKeyAction
CtrlZKeyAction::CtrlZKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlZKeyAction::CtrlZKeyAction(const CtrlZKeyAction& source)
	: KeyAction(source) {
}

CtrlZKeyAction::~CtrlZKeyAction() {
}

void CtrlZKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_UNDO, 0));
}

CtrlZKeyAction& CtrlZKeyAction::operator =(const CtrlZKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlYKeyAction
CtrlYKeyAction::CtrlYKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlYKeyAction::CtrlYKeyAction(const CtrlYKeyAction& source)
	: KeyAction(source) {
}

CtrlYKeyAction::~CtrlYKeyAction() {
}

void CtrlYKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_REDO, 0));
}

CtrlYKeyAction& CtrlYKeyAction::operator =(const CtrlYKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}