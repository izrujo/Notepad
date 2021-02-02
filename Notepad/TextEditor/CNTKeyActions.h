#ifndef _CNTKEYACTIONS_H
#define _CNTKEYACTIONS_H

#include <afxwin.h>

class TextEditingForm;

class CNTKeyAction {
public:
	CNTKeyAction(TextEditingForm *textEditingForm = 0);
	CNTKeyAction(const CNTKeyAction& source);
	virtual ~CNTKeyAction() = 0;
	CNTKeyAction& operator=(const CNTKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
protected:
	TextEditingForm *textEditingForm;
};

class CNTLeftKeyAction : public CNTKeyAction {
public:
	CNTLeftKeyAction(TextEditingForm *textEditingForm = 0);
	CNTLeftKeyAction(const CNTLeftKeyAction& source);
	virtual ~CNTLeftKeyAction();
	CNTLeftKeyAction& operator=(const CNTLeftKeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTRightKeyAction : public CNTKeyAction {
public:
	CNTRightKeyAction(TextEditingForm *textEditingForm = 0);
	CNTRightKeyAction(const CNTRightKeyAction& source);
	virtual ~CNTRightKeyAction();
	CNTRightKeyAction& operator=(const CNTRightKeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTUpKeyAction : public CNTKeyAction {
public:
	CNTUpKeyAction(TextEditingForm *textEditingForm = 0);
	CNTUpKeyAction(const CNTUpKeyAction& source);
	virtual ~CNTUpKeyAction();
	CNTUpKeyAction& operator=(const CNTUpKeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
};

class CNTDownKeyAction : public CNTKeyAction {
public:
	CNTDownKeyAction(TextEditingForm *textEditingForm = 0);
	CNTDownKeyAction(const CNTDownKeyAction& source);
	virtual ~CNTDownKeyAction();
	CNTDownKeyAction& operator=(const CNTDownKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
};

class CNTHomeKeyAction : public CNTKeyAction {
public:
	CNTHomeKeyAction(TextEditingForm *textEditingForm = 0);
	CNTHomeKeyAction(const CNTHomeKeyAction& source);
	virtual ~CNTHomeKeyAction();
	CNTHomeKeyAction& operator=(const CNTHomeKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTEndKeyAction : public CNTKeyAction {
public:
	CNTEndKeyAction(TextEditingForm *textEditingForm = 0);
	CNTEndKeyAction(const CNTEndKeyAction& source);
	virtual ~CNTEndKeyAction();
	CNTEndKeyAction& operator=(const CNTEndKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTCtrlLeftKeyAction : public CNTKeyAction {
public:
	CNTCtrlLeftKeyAction(TextEditingForm *textEditingForm = 0);
	CNTCtrlLeftKeyAction(const CNTCtrlLeftKeyAction& source);
	virtual ~CNTCtrlLeftKeyAction();
	CNTCtrlLeftKeyAction& operator=(const CNTCtrlLeftKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTCtrlRightKeyAction : public CNTKeyAction {
public:
	CNTCtrlRightKeyAction(TextEditingForm *textEditingForm = 0);
	CNTCtrlRightKeyAction(const CNTCtrlRightKeyAction& source);
	virtual ~CNTCtrlRightKeyAction();
	CNTCtrlRightKeyAction& operator=(const CNTCtrlRightKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTCtrlHomeKeyAction : public CNTKeyAction {
public:
	CNTCtrlHomeKeyAction(TextEditingForm *textEditingForm = 0);
	CNTCtrlHomeKeyAction(const CNTCtrlHomeKeyAction& source);
	virtual ~CNTCtrlHomeKeyAction();
	CNTCtrlHomeKeyAction& operator=(const CNTCtrlHomeKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTCtrlEndKeyAction : public CNTKeyAction {
public:
	CNTCtrlEndKeyAction(TextEditingForm *textEditingForm = 0);
	CNTCtrlEndKeyAction(const CNTCtrlEndKeyAction& source);
	virtual ~CNTCtrlEndKeyAction();
	CNTCtrlEndKeyAction& operator=(const CNTCtrlEndKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTDeleteKeyAction : public CNTKeyAction {
public:
	CNTDeleteKeyAction(TextEditingForm *textEditingForm = 0);
	CNTDeleteKeyAction(const CNTDeleteKeyAction& source);
	virtual ~CNTDeleteKeyAction();
	CNTDeleteKeyAction& operator=(const CNTDeleteKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTBackSpaceKeyAction : public CNTKeyAction {
public:
	CNTBackSpaceKeyAction(TextEditingForm *textEditingForm = 0);
	CNTBackSpaceKeyAction(const CNTBackSpaceKeyAction& source);
	virtual ~CNTBackSpaceKeyAction();
	CNTBackSpaceKeyAction& operator=(const CNTBackSpaceKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTPageUpKeyAction : public CNTKeyAction {
public:
	CNTPageUpKeyAction(TextEditingForm *textEditingForm = 0);
	CNTPageUpKeyAction(const CNTPageUpKeyAction& source);
	virtual ~CNTPageUpKeyAction();
	CNTPageUpKeyAction& operator=(const CNTPageUpKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTPageDownKeyAction : public CNTKeyAction {
public:
	CNTPageDownKeyAction(TextEditingForm *textEditingForm = 0);
	CNTPageDownKeyAction(const CNTPageDownKeyAction& source);
	virtual ~CNTPageDownKeyAction();
	CNTPageDownKeyAction& operator=(const CNTPageDownKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CNTShiftLeftKeyAction : public CNTKeyAction {
public:
	CNTShiftLeftKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftLeftKeyAction(const CNTShiftLeftKeyAction& source);
	virtual ~CNTShiftLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftLeftKeyAction& operator =(const CNTShiftLeftKeyAction& source);
};

class CNTShiftRightKeyAction : public CNTKeyAction {
public:
	CNTShiftRightKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftRightKeyAction(const CNTShiftRightKeyAction& source);
	virtual ~CNTShiftRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftRightKeyAction& operator =(const CNTShiftRightKeyAction& source);
};

class CNTShiftUpKeyAction : public CNTKeyAction {
public:
	CNTShiftUpKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftUpKeyAction(const CNTShiftUpKeyAction& source);
	virtual ~CNTShiftUpKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftUpKeyAction& operator =(const CNTShiftUpKeyAction& source);
};

class CNTShiftDownKeyAction : public CNTKeyAction {
public:
	CNTShiftDownKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftDownKeyAction(const CNTShiftDownKeyAction& source);
	virtual ~CNTShiftDownKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftDownKeyAction& operator =(const CNTShiftDownKeyAction& source);
};

class CNTShiftHomeKeyAction : public CNTKeyAction {
public:
	CNTShiftHomeKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftHomeKeyAction(const CNTShiftHomeKeyAction& source);
	virtual ~CNTShiftHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftHomeKeyAction& operator =(const CNTShiftHomeKeyAction& source);
};

class CNTShiftEndKeyAction : public CNTKeyAction {
public:
	CNTShiftEndKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftEndKeyAction(const CNTShiftEndKeyAction& source);
	virtual ~CNTShiftEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftEndKeyAction& operator =(const CNTShiftEndKeyAction& source);
};

class CNTShiftCtrlLeftKeyAction : public CNTKeyAction {
public:
	CNTShiftCtrlLeftKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlLeftKeyAction(const CNTShiftCtrlLeftKeyAction& source);
	virtual ~CNTShiftCtrlLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftCtrlLeftKeyAction& operator =(const CNTShiftCtrlLeftKeyAction& source);
};

class CNTShiftCtrlRightKeyAction : public CNTKeyAction {
public:
	CNTShiftCtrlRightKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlRightKeyAction(const CNTShiftCtrlRightKeyAction& source);
	virtual ~CNTShiftCtrlRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftCtrlRightKeyAction& operator =(const CNTShiftCtrlRightKeyAction& source);
};

class CNTShiftCtrlHomeKeyAction : public CNTKeyAction {
public:
	CNTShiftCtrlHomeKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlHomeKeyAction(const CNTShiftCtrlHomeKeyAction& source);
	virtual ~CNTShiftCtrlHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftCtrlHomeKeyAction& operator =(const CNTShiftCtrlHomeKeyAction& source);
};

class CNTShiftCtrlEndKeyAction : public CNTKeyAction {
public:
	CNTShiftCtrlEndKeyAction(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlEndKeyAction(const CNTShiftCtrlEndKeyAction& source);
	virtual ~CNTShiftCtrlEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTShiftCtrlEndKeyAction& operator =(const CNTShiftCtrlEndKeyAction& source);
};

class CNTCtrlAKeyAction : public CNTKeyAction {
public:
	CNTCtrlAKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlAKeyAction(const CNTCtrlAKeyAction& source);
	virtual ~CNTCtrlAKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlAKeyAction& operator =(const CNTCtrlAKeyAction& source);
};

class CNTCtrlCKeyAction : public CNTKeyAction {
public:
	CNTCtrlCKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlCKeyAction(const CNTCtrlCKeyAction& source);
	virtual ~CNTCtrlCKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlCKeyAction& operator =(const CNTCtrlCKeyAction& source);
};

class CNTCtrlVKeyAction : public CNTKeyAction {
public:
	CNTCtrlVKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlVKeyAction(const CNTCtrlVKeyAction& source);
	virtual ~CNTCtrlVKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlVKeyAction& operator =(const CNTCtrlVKeyAction& source);
};

class CNTCtrlXKeyAction : public CNTKeyAction {
public:
	CNTCtrlXKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlXKeyAction(const CNTCtrlXKeyAction& source);
	virtual ~CNTCtrlXKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlXKeyAction& operator =(const CNTCtrlXKeyAction& source);
};

class CNTCtrlZKeyAction : public CNTKeyAction {
public:
	CNTCtrlZKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlZKeyAction(const CNTCtrlZKeyAction& source);
	virtual ~CNTCtrlZKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlZKeyAction& operator =(const CNTCtrlZKeyAction& source);
};

class CNTCtrlYKeyAction : public CNTKeyAction {
public:
	CNTCtrlYKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlYKeyAction(const CNTCtrlYKeyAction& source);
	virtual ~CNTCtrlYKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlYKeyAction& operator =(const CNTCtrlYKeyAction& source);
};

class CNTCtrlFKeyAction : public CNTKeyAction {
public:
	CNTCtrlFKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlFKeyAction(const CNTCtrlFKeyAction& source);
	virtual ~CNTCtrlFKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlFKeyAction& operator =(const CNTCtrlFKeyAction& source);
};

class CNTCtrlHKeyAction : public CNTKeyAction {
public:
	CNTCtrlHKeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlHKeyAction(const CNTCtrlHKeyAction& source);
	virtual ~CNTCtrlHKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlHKeyAction& operator =(const CNTCtrlHKeyAction& source);
};

//임시 - 자동개행 LockHScroll
class CNTCtrlF1KeyAction : public CNTKeyAction {
public:
	CNTCtrlF1KeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlF1KeyAction(const CNTCtrlF1KeyAction& source);
	virtual ~CNTCtrlF1KeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlF1KeyAction& operator =(const CNTCtrlF1KeyAction& source);
};

//임시 - 실행취소 UnlockHistoryBook
class CNTCtrlF2KeyAction : public CNTKeyAction {
public:
	CNTCtrlF2KeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlF2KeyAction(const CNTCtrlF2KeyAction& source);
	virtual ~CNTCtrlF2KeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlF2KeyAction& operator =(const CNTCtrlF2KeyAction& source);
};

//임시 - 찾기바꾸기 UnlockFindReplaceDialog
class CNTCtrlF3KeyAction : public CNTKeyAction {
public:
	CNTCtrlF3KeyAction(TextEditingForm* textEditingForm = 0);
	CNTCtrlF3KeyAction(const CNTCtrlF3KeyAction& source);
	virtual ~CNTCtrlF3KeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CNTCtrlF3KeyAction& operator =(const CNTCtrlF3KeyAction& source);
};

#endif //_CNTKEYACTIONS_H