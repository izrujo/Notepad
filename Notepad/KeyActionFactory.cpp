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

	if (isCtrl && nChar == VK_LEFT) {
		keyAction = new CtrlLeftKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == VK_RIGHT) {
		keyAction = new CtrlRightKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == VK_HOME) {
		keyAction = new CtrlHomeKeyAction(this->notepadForm);
	}
	else if (isCtrl && nChar == VK_END) {
		keyAction = new CtrlEndKeyAction(this->notepadForm);
	}

	else if (nChar == VK_LEFT) {
		keyAction = new LeftKeyAction(this->notepadForm);
	}
	else if (nChar == VK_RIGHT) {
		keyAction = new RightKeyAction(this->notepadForm);
	}
	else if (nChar == VK_UP) {
		keyAction = new UpKeyAction(this->notepadForm);
	}
	else if (nChar == VK_DOWN) {
		keyAction = new DownKeyAction(this->notepadForm);
	}
	else if (nChar == VK_HOME) {
		keyAction = new HomeKeyAction(this->notepadForm);
	}
	else if (nChar == VK_END) {
		keyAction = new EndKeyAction(this->notepadForm);
	}
	else if (nChar == VK_DELETE) {
		keyAction = new DeleteKeyAction(this->notepadForm);
	}
	else if (nChar == VK_BACK) {
		keyAction = new BackSpaceKeyAction(this->notepadForm);
	}
	else if (nChar == VK_PRIOR) {
		keyAction = new PageUpKeyAction(this->notepadForm);
	}
	else if (nChar == VK_NEXT) {
		keyAction = new PageDownKeyAction(this->notepadForm);
	}

	return keyAction;
}