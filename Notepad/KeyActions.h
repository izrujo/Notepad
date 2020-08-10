#ifndef _KEYACTIONS_H
#define _KEYACTIONS_H

#include <afxwin.h>

class NotepadForm;

class KeyAction {
public:
	KeyAction(NotepadForm *notepadForm = 0);
	KeyAction(const KeyAction& source);
	virtual ~KeyAction() = 0;
	KeyAction& operator=(const KeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
protected:
	NotepadForm *notepadForm;
};

class LeftKeyAction : public KeyAction {
public:
	LeftKeyAction(NotepadForm *notepadForm = 0);
	LeftKeyAction(const LeftKeyAction& source);
	virtual ~LeftKeyAction();
	LeftKeyAction& operator=(const LeftKeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class RightKeyAction : public KeyAction {
public:
	RightKeyAction(NotepadForm *notepadForm = 0);
	RightKeyAction(const RightKeyAction& source);
	virtual ~RightKeyAction();
	RightKeyAction& operator=(const RightKeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class UpKeyAction : public KeyAction {
public:
	UpKeyAction(NotepadForm *notepadForm = 0);
	UpKeyAction(const UpKeyAction& source);
	virtual ~UpKeyAction();
	UpKeyAction& operator=(const UpKeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
};

class DownKeyAction : public KeyAction {
public:
	DownKeyAction(NotepadForm *notepadForm = 0);
	DownKeyAction(const DownKeyAction& source);
	virtual ~DownKeyAction();
	DownKeyAction& operator=(const DownKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
};

class HomeKeyAction : public KeyAction {
public:
	HomeKeyAction(NotepadForm *notepadForm = 0);
	HomeKeyAction(const HomeKeyAction& source);
	virtual ~HomeKeyAction();
	HomeKeyAction& operator=(const HomeKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class EndKeyAction : public KeyAction {
public:
	EndKeyAction(NotepadForm *notepadForm = 0);
	EndKeyAction(const EndKeyAction& source);
	virtual ~EndKeyAction();
	EndKeyAction& operator=(const EndKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CtrlLeftKeyAction : public KeyAction {
public:
	CtrlLeftKeyAction(NotepadForm *notepadForm = 0);
	CtrlLeftKeyAction(const CtrlLeftKeyAction& source);
	virtual ~CtrlLeftKeyAction();
	CtrlLeftKeyAction& operator=(const CtrlLeftKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CtrlRightKeyAction : public KeyAction {
public:
	CtrlRightKeyAction(NotepadForm *notepadForm = 0);
	CtrlRightKeyAction(const CtrlRightKeyAction& source);
	virtual ~CtrlRightKeyAction();
	CtrlRightKeyAction& operator=(const CtrlRightKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CtrlHomeKeyAction : public KeyAction {
public:
	CtrlHomeKeyAction(NotepadForm *notepadForm = 0);
	CtrlHomeKeyAction(const CtrlHomeKeyAction& source);
	virtual ~CtrlHomeKeyAction();
	CtrlHomeKeyAction& operator=(const CtrlHomeKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CtrlEndKeyAction : public KeyAction {
public:
	CtrlEndKeyAction(NotepadForm *notepadForm = 0);
	CtrlEndKeyAction(const CtrlEndKeyAction& source);
	virtual ~CtrlEndKeyAction();
	CtrlEndKeyAction& operator=(const CtrlEndKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class DeleteKeyAction : public KeyAction {
public:
	DeleteKeyAction(NotepadForm *notepadForm = 0);
	DeleteKeyAction(const DeleteKeyAction& source);
	virtual ~DeleteKeyAction();
	DeleteKeyAction& operator=(const DeleteKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class BackSpaceKeyAction : public KeyAction {
public:
	BackSpaceKeyAction(NotepadForm *notepadForm = 0);
	BackSpaceKeyAction(const BackSpaceKeyAction& source);
	virtual ~BackSpaceKeyAction();
	BackSpaceKeyAction& operator=(const BackSpaceKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class PageUpKeyAction : public KeyAction {
public:
	PageUpKeyAction(NotepadForm *notepadForm = 0);
	PageUpKeyAction(const PageUpKeyAction& source);
	virtual ~PageUpKeyAction();
	PageUpKeyAction& operator=(const PageUpKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class PageDownKeyAction : public KeyAction {
public:
	PageDownKeyAction(NotepadForm *notepadForm = 0);
	PageDownKeyAction(const PageDownKeyAction& source);
	virtual ~PageDownKeyAction();
	PageDownKeyAction& operator=(const PageDownKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class ShiftLeftKeyAction : public KeyAction {
public:
	ShiftLeftKeyAction(NotepadForm* notepadForm = 0);
	ShiftLeftKeyAction(const ShiftLeftKeyAction& source);
	virtual ~ShiftLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftLeftKeyAction& operator =(const ShiftLeftKeyAction& source);
};

class ShiftRightKeyAction : public KeyAction {
public:
	ShiftRightKeyAction(NotepadForm* notepadForm = 0);
	ShiftRightKeyAction(const ShiftRightKeyAction& source);
	virtual ~ShiftRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftRightKeyAction& operator =(const ShiftRightKeyAction& source);
};

class ShiftUpKeyAction : public KeyAction {
public:
	ShiftUpKeyAction(NotepadForm* notepadForm = 0);
	ShiftUpKeyAction(const ShiftUpKeyAction& source);
	virtual ~ShiftUpKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftUpKeyAction& operator =(const ShiftUpKeyAction& source);
};

class ShiftDownKeyAction : public KeyAction {
public:
	ShiftDownKeyAction(NotepadForm* notepadForm = 0);
	ShiftDownKeyAction(const ShiftDownKeyAction& source);
	virtual ~ShiftDownKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftDownKeyAction& operator =(const ShiftDownKeyAction& source);
};

class ShiftHomeKeyAction : public KeyAction {
public:
	ShiftHomeKeyAction(NotepadForm* notepadForm = 0);
	ShiftHomeKeyAction(const ShiftHomeKeyAction& source);
	virtual ~ShiftHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftHomeKeyAction& operator =(const ShiftHomeKeyAction& source);
};

class ShiftEndKeyAction : public KeyAction {
public:
	ShiftEndKeyAction(NotepadForm* notepadForm = 0);
	ShiftEndKeyAction(const ShiftEndKeyAction& source);
	virtual ~ShiftEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftEndKeyAction& operator =(const ShiftEndKeyAction& source);
};

class ShiftCtrlLeftKeyAction : public KeyAction {
public:
	ShiftCtrlLeftKeyAction(NotepadForm* notepadForm = 0);
	ShiftCtrlLeftKeyAction(const ShiftCtrlLeftKeyAction& source);
	virtual ~ShiftCtrlLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftCtrlLeftKeyAction& operator =(const ShiftCtrlLeftKeyAction& source);
};

class ShiftCtrlRightKeyAction : public KeyAction {
public:
	ShiftCtrlRightKeyAction(NotepadForm* notepadForm = 0);
	ShiftCtrlRightKeyAction(const ShiftCtrlRightKeyAction& source);
	virtual ~ShiftCtrlRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftCtrlRightKeyAction& operator =(const ShiftCtrlRightKeyAction& source);
};

class ShiftCtrlHomeKeyAction : public KeyAction {
public:
	ShiftCtrlHomeKeyAction(NotepadForm* notepadForm = 0);
	ShiftCtrlHomeKeyAction(const ShiftCtrlHomeKeyAction& source);
	virtual ~ShiftCtrlHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftCtrlHomeKeyAction& operator =(const ShiftCtrlHomeKeyAction& source);
};

class ShiftCtrlEndKeyAction : public KeyAction {
public:
	ShiftCtrlEndKeyAction(NotepadForm* notepadForm = 0);
	ShiftCtrlEndKeyAction(const ShiftCtrlEndKeyAction& source);
	virtual ~ShiftCtrlEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	ShiftCtrlEndKeyAction& operator =(const ShiftCtrlEndKeyAction& source);
};

class CtrlAKeyAction : public KeyAction {
public:
	CtrlAKeyAction(NotepadForm* notepadForm = 0);
	CtrlAKeyAction(const CtrlAKeyAction& source);
	virtual ~CtrlAKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlAKeyAction& operator =(const CtrlAKeyAction& source);
};

class CtrlCKeyAction : public KeyAction {
public:
	CtrlCKeyAction(NotepadForm* notepadForm = 0);
	CtrlCKeyAction(const CtrlCKeyAction& source);
	virtual ~CtrlCKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlCKeyAction& operator =(const CtrlCKeyAction& source);
};

class CtrlVKeyAction : public KeyAction {
public:
	CtrlVKeyAction(NotepadForm* notepadForm = 0);
	CtrlVKeyAction(const CtrlVKeyAction& source);
	virtual ~CtrlVKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlVKeyAction& operator =(const CtrlVKeyAction& source);
};

class CtrlXKeyAction : public KeyAction {
public:
	CtrlXKeyAction(NotepadForm* notepadForm = 0);
	CtrlXKeyAction(const CtrlXKeyAction& source);
	virtual ~CtrlXKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlXKeyAction& operator =(const CtrlXKeyAction& source);
};

class CtrlZKeyAction : public KeyAction {
public:
	CtrlZKeyAction(NotepadForm* notepadForm = 0);
	CtrlZKeyAction(const CtrlZKeyAction& source);
	virtual ~CtrlZKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlZKeyAction& operator =(const CtrlZKeyAction& source);
};

class CtrlYKeyAction : public KeyAction {
public:
	CtrlYKeyAction(NotepadForm* notepadForm = 0);
	CtrlYKeyAction(const CtrlYKeyAction& source);
	virtual ~CtrlYKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlYKeyAction& operator =(const CtrlYKeyAction& source);
};

#endif //_KEYACTIONS_H