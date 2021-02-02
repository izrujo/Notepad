#include "CNTKeyActions.h"
#include "CNTKeyActionFactory.h"
#include "TextEditingForm.h"
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

#include "DummyManager.h" //CtrlF1
#include "HistoryBook.h" //CtrlF1

#include "resource.h"

#include <string>

//CNTKeyAction
CNTKeyAction::CNTKeyAction(TextEditingForm* textEditingForm) {
	this->textEditingForm = textEditingForm;
}

CNTKeyAction::CNTKeyAction(const CNTKeyAction& source) {
	this->textEditingForm = source.textEditingForm;
}

CNTKeyAction::~CNTKeyAction() {

}

CNTKeyAction& CNTKeyAction::operator=(const CNTKeyAction& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

//CNTLeftKeyAction
CNTLeftKeyAction::CNTLeftKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {

}

CNTLeftKeyAction::CNTLeftKeyAction(const CNTLeftKeyAction& source)
	: CNTKeyAction(source) {

}

CNTLeftKeyAction::~CNTLeftKeyAction() {

}

CNTLeftKeyAction& CNTLeftKeyAction::operator=(const CNTLeftKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

void CNTLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_LEFT, 0));
}

//CNTRightKeyAction
CNTRightKeyAction::CNTRightKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTRightKeyAction::CNTRightKeyAction(const CNTRightKeyAction& source)
	: CNTKeyAction(source) {
}

CNTRightKeyAction::~CNTRightKeyAction() {

}

CNTRightKeyAction& CNTRightKeyAction::operator=(const CNTRightKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

void CNTRightKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_RIGHT, 0));
}

//CNTUpKeyAction
CNTUpKeyAction::CNTUpKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {

}

CNTUpKeyAction::CNTUpKeyAction(const CNTUpKeyAction& source)
	: CNTKeyAction(source) {

}

CNTUpKeyAction::~CNTUpKeyAction() {

}

CNTUpKeyAction& CNTUpKeyAction::operator=(const CNTUpKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

void CNTUpKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_UP, 0));
}

//CNTDownKeyAction
CNTDownKeyAction::CNTDownKeyAction(TextEditingForm* textEditingForm)
	:CNTKeyAction(textEditingForm) {

}

CNTDownKeyAction::CNTDownKeyAction(const CNTDownKeyAction& source)
	: CNTKeyAction(source) {

}

CNTDownKeyAction::~CNTDownKeyAction() {

}

CNTDownKeyAction& CNTDownKeyAction::operator=(const CNTDownKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTDownKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_DOWN, 0));
}

//CNTHomeKeyAction
CNTHomeKeyAction::CNTHomeKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTHomeKeyAction::CNTHomeKeyAction(const CNTHomeKeyAction& source)
	: CNTKeyAction(source) {
}

CNTHomeKeyAction::~CNTHomeKeyAction() {

}

CNTHomeKeyAction& CNTHomeKeyAction::operator=(const CNTHomeKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_HOME, 0));
}

//CNTEndKeyAction
CNTEndKeyAction::CNTEndKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTEndKeyAction::CNTEndKeyAction(const CNTEndKeyAction& source)
	: CNTKeyAction(source) {
}

CNTEndKeyAction::~CNTEndKeyAction() {

}

CNTEndKeyAction& CNTEndKeyAction::operator=(const CNTEndKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_END, 0));
}

//CNTCtrlLeftKeyAction
CNTCtrlLeftKeyAction::CNTCtrlLeftKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlLeftKeyAction::CNTCtrlLeftKeyAction(const CNTCtrlLeftKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlLeftKeyAction::~CNTCtrlLeftKeyAction() {

}

CNTCtrlLeftKeyAction& CNTCtrlLeftKeyAction::operator=(const CNTCtrlLeftKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTCtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLLEFT, 0));
}

//CNTCtrlRightKeyAction
CNTCtrlRightKeyAction::CNTCtrlRightKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlRightKeyAction::CNTCtrlRightKeyAction(const CNTCtrlRightKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlRightKeyAction::~CNTCtrlRightKeyAction() {

}

CNTCtrlRightKeyAction& CNTCtrlRightKeyAction::operator=(const CNTCtrlRightKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTCtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLRIGHT, 0));
}

//CNTCtrlHomeKeyAction
CNTCtrlHomeKeyAction::CNTCtrlHomeKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlHomeKeyAction::CNTCtrlHomeKeyAction(const CNTCtrlHomeKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlHomeKeyAction::~CNTCtrlHomeKeyAction() {

}

CNTCtrlHomeKeyAction& CNTCtrlHomeKeyAction::operator=(const CNTCtrlHomeKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTCtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLHOME, 0));
}

//CNTCtrlEndKeyAction
CNTCtrlEndKeyAction::CNTCtrlEndKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlEndKeyAction::CNTCtrlEndKeyAction(const CNTCtrlEndKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlEndKeyAction::~CNTCtrlEndKeyAction() {

}

CNTCtrlEndKeyAction& CNTCtrlEndKeyAction::operator=(const CNTCtrlEndKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTCtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLEND, 0));
}

//CNTDeleteKeyAction
CNTDeleteKeyAction::CNTDeleteKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTDeleteKeyAction::CNTDeleteKeyAction(const CNTDeleteKeyAction& source)
	: CNTKeyAction(source) {
}

CNTDeleteKeyAction::~CNTDeleteKeyAction() {

}

CNTDeleteKeyAction& CNTDeleteKeyAction::operator=(const CNTDeleteKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTDeleteKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->textEditingForm->selection == NULL) {
		this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETE, 0));
	}
	else {
		this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
	}
}

//CNTBackSpaceKeyAction
CNTBackSpaceKeyAction::CNTBackSpaceKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTBackSpaceKeyAction::CNTBackSpaceKeyAction(const CNTBackSpaceKeyAction& source)
	: CNTKeyAction(source) {
}

CNTBackSpaceKeyAction::~CNTBackSpaceKeyAction() {

}

CNTBackSpaceKeyAction& CNTBackSpaceKeyAction::operator=(const CNTBackSpaceKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTBackSpaceKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->textEditingForm->selection == NULL) {
		if (this->textEditingForm->note->GetCurrent() > 0 || this->textEditingForm->current->GetCurrent() > 0) {
			this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_LEFT, 0));
			this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETE, 0));
		}
	}
	else {
		this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
	}
}

//CNTPageUpKeyAction
CNTPageUpKeyAction::CNTPageUpKeyAction(TextEditingForm* textEditingForm)
	:CNTKeyAction(textEditingForm) {

}

CNTPageUpKeyAction::CNTPageUpKeyAction(const CNTPageUpKeyAction& source)
	: CNTKeyAction(source) {

}

CNTPageUpKeyAction::~CNTPageUpKeyAction() {

}

CNTPageUpKeyAction& CNTPageUpKeyAction::operator=(const CNTPageUpKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

void CNTPageUpKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_PAGEUP, 0));
}

//CNTPageDownKeyAction
CNTPageDownKeyAction::CNTPageDownKeyAction(TextEditingForm* textEditingForm)
	:CNTKeyAction(textEditingForm) {
}

CNTPageDownKeyAction::CNTPageDownKeyAction(const CNTPageDownKeyAction& source)
	: CNTKeyAction(source) {

}

CNTPageDownKeyAction::~CNTPageDownKeyAction() {

}

CNTPageDownKeyAction& CNTPageDownKeyAction::operator=(const CNTPageDownKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

void CNTPageDownKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_PAGEDOWN, 0));
}

//CNTShiftLeftKeyAction
CNTShiftLeftKeyAction::CNTShiftLeftKeyAction(TextEditingForm* textEditingForm)
	:CNTKeyAction(textEditingForm) {

}

CNTShiftLeftKeyAction::CNTShiftLeftKeyAction(const CNTShiftLeftKeyAction& source)
	: CNTKeyAction(textEditingForm) {

}

CNTShiftLeftKeyAction::~CNTShiftLeftKeyAction() {

}

void CNTShiftLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_LEFT, 0));
}

CNTShiftLeftKeyAction& CNTShiftLeftKeyAction::operator = (const CNTShiftLeftKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

//CNTShiftRightKeyAction
CNTShiftRightKeyAction::CNTShiftRightKeyAction(TextEditingForm* textEditingForm)
	:CNTKeyAction(textEditingForm) {
}

CNTShiftRightKeyAction::CNTShiftRightKeyAction(const CNTShiftRightKeyAction& source)
	: CNTKeyAction(textEditingForm) {
}

CNTShiftRightKeyAction::~CNTShiftRightKeyAction() {
}

void CNTShiftRightKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_RIGHT, 0));
}

CNTShiftRightKeyAction& CNTShiftRightKeyAction::operator=(const CNTShiftRightKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

//CNTShiftUpKeyAction
CNTShiftUpKeyAction::CNTShiftUpKeyAction(TextEditingForm* textEditingForm)
	:CNTKeyAction(textEditingForm) {

}

CNTShiftUpKeyAction::CNTShiftUpKeyAction(const CNTShiftUpKeyAction& source)
	: CNTKeyAction(source) {

}

CNTShiftUpKeyAction::~CNTShiftUpKeyAction() {

}

void CNTShiftUpKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_UP, 0));
}

CNTShiftUpKeyAction& CNTShiftUpKeyAction::operator = (const CNTShiftUpKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

//CNTShiftDownKeyAction
CNTShiftDownKeyAction::CNTShiftDownKeyAction(TextEditingForm* textEditingForm)
	:CNTKeyAction(textEditingForm) {

}

CNTShiftDownKeyAction::CNTShiftDownKeyAction(const CNTShiftDownKeyAction& source)
	: CNTKeyAction(textEditingForm) {

}

CNTShiftDownKeyAction::~CNTShiftDownKeyAction() {

}

void CNTShiftDownKeyAction::OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_DOWN, 0));
}

CNTShiftDownKeyAction& CNTShiftDownKeyAction::operator=(const CNTShiftDownKeyAction& source) {
	CNTKeyAction::operator=(source);
	return *this;
}

//CNTShiftHomeKeyAction
CNTShiftHomeKeyAction::CNTShiftHomeKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTShiftHomeKeyAction::CNTShiftHomeKeyAction(const CNTShiftHomeKeyAction& source)
	: CNTKeyAction(source) {
}

CNTShiftHomeKeyAction::~CNTShiftHomeKeyAction() {
}

void CNTShiftHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_HOME, 0));
}

CNTShiftHomeKeyAction& CNTShiftHomeKeyAction::operator =(const CNTShiftHomeKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTShiftEndKeyAction
CNTShiftEndKeyAction::CNTShiftEndKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTShiftEndKeyAction::CNTShiftEndKeyAction(const CNTShiftEndKeyAction& source)
	: CNTKeyAction(source) {
}

CNTShiftEndKeyAction::~CNTShiftEndKeyAction() {
}

void CNTShiftEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_END, 0));
}

CNTShiftEndKeyAction& CNTShiftEndKeyAction::operator =(const CNTShiftEndKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTShiftCtrlLeftKeyAction
CNTShiftCtrlLeftKeyAction::CNTShiftCtrlLeftKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTShiftCtrlLeftKeyAction::CNTShiftCtrlLeftKeyAction(const CNTShiftCtrlLeftKeyAction& source)
	: CNTKeyAction(source) {
}

CNTShiftCtrlLeftKeyAction::~CNTShiftCtrlLeftKeyAction() {
}

void CNTShiftCtrlLeftKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_CTRLLEFT, 0));
}

CNTShiftCtrlLeftKeyAction& CNTShiftCtrlLeftKeyAction::operator =(const CNTShiftCtrlLeftKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTShiftCtrlRightKeyAction
CNTShiftCtrlRightKeyAction::CNTShiftCtrlRightKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTShiftCtrlRightKeyAction::CNTShiftCtrlRightKeyAction(const CNTShiftCtrlRightKeyAction& source)
	: CNTKeyAction(source) {
}

CNTShiftCtrlRightKeyAction::~CNTShiftCtrlRightKeyAction() {
}

void CNTShiftCtrlRightKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_CTRLRIGHT, 0));
}

CNTShiftCtrlRightKeyAction& CNTShiftCtrlRightKeyAction::operator =(const CNTShiftCtrlRightKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTShiftCtrlHomeKeyAction
CNTShiftCtrlHomeKeyAction::CNTShiftCtrlHomeKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTShiftCtrlHomeKeyAction::CNTShiftCtrlHomeKeyAction(const CNTShiftCtrlHomeKeyAction& source)
	: CNTKeyAction(source) {
}

CNTShiftCtrlHomeKeyAction::~CNTShiftCtrlHomeKeyAction() {
}

void CNTShiftCtrlHomeKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_CTRLHOME, 0));
}

CNTShiftCtrlHomeKeyAction& CNTShiftCtrlHomeKeyAction::operator =(const CNTShiftCtrlHomeKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTShiftCtrlEndKeyAction
CNTShiftCtrlEndKeyAction::CNTShiftCtrlEndKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTShiftCtrlEndKeyAction::CNTShiftCtrlEndKeyAction(const CNTShiftCtrlEndKeyAction& source)
	: CNTKeyAction(source) {
}

CNTShiftCtrlEndKeyAction::~CNTShiftCtrlEndKeyAction() {
}

void CNTShiftCtrlEndKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_CTRLEND, 0));
}

CNTShiftCtrlEndKeyAction& CNTShiftCtrlEndKeyAction::operator =(const CNTShiftCtrlEndKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlAKeyAction
CNTCtrlAKeyAction::CNTCtrlAKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlAKeyAction::CNTCtrlAKeyAction(const CNTCtrlAKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlAKeyAction::~CNTCtrlAKeyAction() {
}

void CNTCtrlAKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_SELECTALL, 0));
}

CNTCtrlAKeyAction& CNTCtrlAKeyAction::operator =(const CNTCtrlAKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlCKeyAction
CNTCtrlCKeyAction::CNTCtrlCKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlCKeyAction::CNTCtrlCKeyAction(const CNTCtrlCKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlCKeyAction::~CNTCtrlCKeyAction() {
}

void CNTCtrlCKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_COPY, 0));
}

CNTCtrlCKeyAction& CNTCtrlCKeyAction::operator =(const CNTCtrlCKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlVKeyAction
CNTCtrlVKeyAction::CNTCtrlVKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlVKeyAction::CNTCtrlVKeyAction(const CNTCtrlVKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlVKeyAction::~CNTCtrlVKeyAction() {
}

void CNTCtrlVKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_PASTE, 0));
}

CNTCtrlVKeyAction& CNTCtrlVKeyAction::operator =(const CNTCtrlVKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlXKeyAction
CNTCtrlXKeyAction::CNTCtrlXKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlXKeyAction::CNTCtrlXKeyAction(const CNTCtrlXKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlXKeyAction::~CNTCtrlXKeyAction() {
}

void CNTCtrlXKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_CUT, 0));
	}
}

CNTCtrlXKeyAction& CNTCtrlXKeyAction::operator =(const CNTCtrlXKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlZKeyAction
CNTCtrlZKeyAction::CNTCtrlZKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlZKeyAction::CNTCtrlZKeyAction(const CNTCtrlZKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlZKeyAction::~CNTCtrlZKeyAction() {
}

void CNTCtrlZKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_UNDO, 0));
}

CNTCtrlZKeyAction& CNTCtrlZKeyAction::operator =(const CNTCtrlZKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlYKeyAction
CNTCtrlYKeyAction::CNTCtrlYKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlYKeyAction::CNTCtrlYKeyAction(const CNTCtrlYKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlYKeyAction::~CNTCtrlYKeyAction() {
}

void CNTCtrlYKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REDO, 0));
}

CNTCtrlYKeyAction& CNTCtrlYKeyAction::operator =(const CNTCtrlYKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlFKeyAction
CNTCtrlFKeyAction::CNTCtrlFKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlFKeyAction::CNTCtrlFKeyAction(const CNTCtrlFKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlFKeyAction::~CNTCtrlFKeyAction() {
}

void CNTCtrlFKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_FIND, 0));
}

CNTCtrlFKeyAction& CNTCtrlFKeyAction::operator =(const CNTCtrlFKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlHKeyAction
CNTCtrlHKeyAction::CNTCtrlHKeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {
}

CNTCtrlHKeyAction::CNTCtrlHKeyAction(const CNTCtrlHKeyAction& source)
	: CNTKeyAction(source) {
}

CNTCtrlHKeyAction::~CNTCtrlHKeyAction() {
}

void CNTCtrlHKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REPLACE, 0));
}

CNTCtrlHKeyAction& CNTCtrlHKeyAction::operator =(const CNTCtrlHKeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

//CNTCtrlF1KeyAction
CNTCtrlF1KeyAction::CNTCtrlF1KeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {

}

CNTCtrlF1KeyAction::CNTCtrlF1KeyAction(const CNTCtrlF1KeyAction& source)
	: CNTKeyAction(source) {

}

CNTCtrlF1KeyAction::~CNTCtrlF1KeyAction() {

}

CNTCtrlF1KeyAction& CNTCtrlF1KeyAction::operator =(const CNTCtrlF1KeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTCtrlF1KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_LOCKHSCROLL, 0));;
}

//CNTCtrlF2KeyAction
CNTCtrlF2KeyAction::CNTCtrlF2KeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {

}

CNTCtrlF2KeyAction::CNTCtrlF2KeyAction(const CNTCtrlF2KeyAction& source)
	: CNTKeyAction(source) {

}

CNTCtrlF2KeyAction::~CNTCtrlF2KeyAction() {

}

CNTCtrlF2KeyAction& CNTCtrlF2KeyAction::operator =(const CNTCtrlF2KeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTCtrlF2KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_UNLOCKHISTORYBOOK, 0));
}

//CNTCtrlF3KeyAction
CNTCtrlF3KeyAction::CNTCtrlF3KeyAction(TextEditingForm* textEditingForm)
	: CNTKeyAction(textEditingForm) {

}

CNTCtrlF3KeyAction::CNTCtrlF3KeyAction(const CNTCtrlF3KeyAction& source)
	: CNTKeyAction(source) {

}

CNTCtrlF3KeyAction::~CNTCtrlF3KeyAction() {

}

CNTCtrlF3KeyAction& CNTCtrlF3KeyAction::operator =(const CNTCtrlF3KeyAction& source) {
	CNTKeyAction::operator=(source);

	return *this;
}

void CNTCtrlF3KeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_UNLOCKFINDREPLACEDIALOG, 0));
}