#include "KeyActionFactory.h"
#include "KeyActions.h"
#include "NotepadForm.h"
#include <WinUser.h>

KeyActionFactory::KeyActionFactory(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

KeyActionFactory::KeyActionFactory(const KeyActionFactory& source) {
	this->notepadForm = source.notepadForm;
}

KeyActionFactory::~KeyActionFactory() {

}

KeyActionFactory& KeyActionFactory::operator=(const KeyActionFactory& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

KeyAction* KeyActionFactory::Make(UINT nChar) {
	KeyAction* keyAction = 0;

	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	SHORT isShift = GetKeyState(VK_SHIFT) & 0X8000;
	
	if (isShift && isCtrl && nChar == 0x53) {
		keyAction = new CtrlShiftSKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == 0x4E) {
		keyAction = new CtrlNKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == 0x4F) {
		keyAction = new CtrlOKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == 0x53) {
		keyAction = new CtrlSKeyAction(this->notepadForm);
	}

	return keyAction;
}