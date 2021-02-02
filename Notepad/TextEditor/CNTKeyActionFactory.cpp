#include "CNTKeyActionFactory.h"
#include "CNTKeyActions.h"
#include "TextEditingForm.h"
#include <WinUser.h>

CNTKeyActionFactory::CNTKeyActionFactory(TextEditingForm *textEditingForm) {
	this->textEditingForm = textEditingForm;
}

CNTKeyActionFactory::CNTKeyActionFactory(const CNTKeyActionFactory& source) {
	this->textEditingForm = source.textEditingForm;
}

CNTKeyActionFactory::~CNTKeyActionFactory() {

}

CNTKeyActionFactory& CNTKeyActionFactory::operator=(const CNTKeyActionFactory& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

CNTKeyAction* CNTKeyActionFactory::Make(UINT nChar) {
	CNTKeyAction* keyAction = 0;

	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	SHORT isShift = GetKeyState(VK_SHIFT) & 0X8000;

	//Shift + Ctrl + KEY
	if (isShift && isCtrl && nChar == VK_LEFT) {
		keyAction = new CNTShiftCtrlLeftKeyAction(this->textEditingForm);
	}
	else if (isShift && isCtrl && nChar == VK_RIGHT) {
		keyAction = new CNTShiftCtrlRightKeyAction(this->textEditingForm);
	}
	else if (isShift && isCtrl && nChar == VK_HOME) {
		keyAction = new CNTShiftCtrlHomeKeyAction(this->textEditingForm);
	}
	else if (isShift && isCtrl && nChar == VK_END) {
		keyAction = new CNTShiftCtrlEndKeyAction(this->textEditingForm);
	}
	//Shift + KEY
	else if (isShift && nChar == VK_LEFT) {
		keyAction = new CNTShiftLeftKeyAction(this->textEditingForm);
	}
	else if (isShift && nChar == VK_RIGHT) {
		keyAction = new CNTShiftRightKeyAction(this->textEditingForm);
	}
	else if (isShift && nChar == VK_UP) {
		keyAction = new CNTShiftUpKeyAction(this->textEditingForm);
	}
	else if (isShift && nChar == VK_DOWN) {
		keyAction = new CNTShiftDownKeyAction(this->textEditingForm);
	}
	else if (isShift && nChar == VK_HOME) {
		keyAction = new CNTShiftHomeKeyAction(this->textEditingForm);
	}
	else if (isShift && nChar == VK_END) {
		keyAction = new CNTShiftEndKeyAction(this->textEditingForm);
	}
	//Ctrl + KEY
	else if (isCtrl && nChar == VK_LEFT) {
		keyAction = new CNTCtrlLeftKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == VK_RIGHT) {
		keyAction = new CNTCtrlRightKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == VK_HOME) {
		keyAction = new CNTCtrlHomeKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == VK_END) {
		keyAction = new CNTCtrlEndKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x41) {
		keyAction = new CNTCtrlAKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x43) {
		keyAction = new CNTCtrlCKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x56) {
		keyAction = new CNTCtrlVKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x58) {
		keyAction = new CNTCtrlXKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x5A) {
		keyAction = new CNTCtrlZKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x59) {
		keyAction = new CNTCtrlYKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x46) {
		keyAction = new CNTCtrlFKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x48) {
		keyAction = new CNTCtrlHKeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x70) {// 임시
		keyAction = new CNTCtrlF1KeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x71) {// 임시
		keyAction = new CNTCtrlF2KeyAction(this->textEditingForm);
	}
	else if (isCtrl && nChar == 0x72) {// 임시
		keyAction = new CNTCtrlF3KeyAction(this->textEditingForm);
	}
	//KEY
	else if (nChar == VK_LEFT) {
		keyAction = new CNTLeftKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_RIGHT) {
		keyAction = new CNTRightKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_UP) {
		keyAction = new CNTUpKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_DOWN) {
		keyAction = new CNTDownKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_HOME) {
		keyAction = new CNTHomeKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_END) {
		keyAction = new CNTEndKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_DELETE) {
		keyAction = new CNTDeleteKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_BACK) {
		keyAction = new CNTBackSpaceKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_PRIOR) {
		keyAction = new CNTPageUpKeyAction(this->textEditingForm);
	}
	else if (nChar == VK_NEXT) {
		keyAction = new CNTPageDownKeyAction(this->textEditingForm);
	}

	return keyAction;
}