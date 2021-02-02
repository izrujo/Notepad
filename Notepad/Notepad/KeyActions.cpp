#include "KeyActions.h"
#include "KeyActionFactory.h"
#include "NotepadForm.h"

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

//CtrlNKeyAction
CtrlNKeyAction::CtrlNKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlNKeyAction::CtrlNKeyAction(const CtrlNKeyAction& source)
	: KeyAction(source) {
}

CtrlNKeyAction::~CtrlNKeyAction() {
}

void CtrlNKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_NEW, 0));
}

CtrlNKeyAction& CtrlNKeyAction::operator =(const CtrlNKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlOKeyAction
CtrlOKeyAction::CtrlOKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlOKeyAction::CtrlOKeyAction(const CtrlOKeyAction& source)
	: KeyAction(source) {
}

CtrlOKeyAction::~CtrlOKeyAction() {
}

void CtrlOKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_OPEN, 0));
}

CtrlOKeyAction& CtrlOKeyAction::operator =(const CtrlOKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlSKeyAction
CtrlSKeyAction::CtrlSKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlSKeyAction::CtrlSKeyAction(const CtrlSKeyAction& source)
	: KeyAction(source) {
}

CtrlSKeyAction::~CtrlSKeyAction() {
}

void CtrlSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVE, 0));
}

CtrlSKeyAction& CtrlSKeyAction::operator =(const CtrlSKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}

//CtrlShiftSKeyAction
CtrlShiftSKeyAction::CtrlShiftSKeyAction(NotepadForm* notepadForm)
	: KeyAction(notepadForm) {
}

CtrlShiftSKeyAction::CtrlShiftSKeyAction(const CtrlShiftSKeyAction& source)
	: KeyAction(source) {
}

CtrlShiftSKeyAction::~CtrlShiftSKeyAction() {
}

void CtrlShiftSKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_SAVEAS, 0));
}

CtrlShiftSKeyAction& CtrlShiftSKeyAction::operator =(const CtrlShiftSKeyAction& source) {
	KeyAction::operator =(source);

	return *this;
}