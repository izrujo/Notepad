#ifndef _KEYACTIONS_H
#define _KEYACTIONS_H

#include <afxwin.h>

class NotepadForm;
class KeyAction {
public:
	KeyAction(NotepadForm *notepadForm = 0);
	KeyAction(const KeyAction& source);
	virtual ~KeyAction() = 0;
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
	KeyAction& operator=(const KeyAction& source);
protected:
	NotepadForm *notepadForm;
};

class LeftKeyAction : public KeyAction {
public:
	LeftKeyAction(NotepadForm *notepadForm = 0);
	LeftKeyAction(const LeftKeyAction& source);
	virtual ~LeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	LeftKeyAction& operator =(const LeftKeyAction& source);
};

class RightKeyAction : public KeyAction {
public:
	RightKeyAction(NotepadForm *notepadForm = 0);
	RightKeyAction(const RightKeyAction& source);
	virtual ~RightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	RightKeyAction& operator =(const RightKeyAction& source);
};

class UpKeyAction : public KeyAction {
public:
	UpKeyAction(NotepadForm *notepadForm = 0);
	UpKeyAction(const UpKeyAction& source);
	virtual ~UpKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
	UpKeyAction& operator = (const UpKeyAction& source);
};

class DownKeyAction : public KeyAction {
public:
	DownKeyAction(NotepadForm *notepadForm = 0);
	DownKeyAction(const DownKeyAction& source);
	virtual ~DownKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepeatCnt, UINT nFlags);
	DownKeyAction& operator=(const DownKeyAction& source);
};

class HomeKeyAction : public KeyAction {
public:
	HomeKeyAction(NotepadForm *notepadForm = 0);
	HomeKeyAction(const HomeKeyAction& source);
	virtual ~HomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	HomeKeyAction& operator =(const HomeKeyAction& source);
};

class EndKeyAction : public KeyAction {
public:
	EndKeyAction(NotepadForm *notepadForm = 0);
	EndKeyAction(const EndKeyAction& source);
	virtual ~EndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	EndKeyAction& operator =(const EndKeyAction& source);
};

class CtrlLeftKeyAction : public KeyAction {
public:
	CtrlLeftKeyAction(NotepadForm *notepadForm = 0);
	CtrlLeftKeyAction(const CtrlLeftKeyAction& source);
	virtual ~CtrlLeftKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlLeftKeyAction& operator =(const CtrlLeftKeyAction& source);
};

class CtrlRightKeyAction : public KeyAction {
public:
	CtrlRightKeyAction(NotepadForm *notepadForm = 0);
	CtrlRightKeyAction(const CtrlRightKeyAction& source);
	virtual ~CtrlRightKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlRightKeyAction& operator =(const CtrlRightKeyAction& source);
};

class CtrlHomeKeyAction : public KeyAction {
public:
	CtrlHomeKeyAction(NotepadForm *notepadForm = 0);
	CtrlHomeKeyAction(const CtrlHomeKeyAction& source);
	virtual ~CtrlHomeKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlHomeKeyAction& operator =(const CtrlHomeKeyAction& source);
};

class CtrlEndKeyAction : public KeyAction {
public:
	CtrlEndKeyAction(NotepadForm *notepadForm = 0);
	CtrlEndKeyAction(const CtrlEndKeyAction& source);
	virtual ~CtrlEndKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlEndKeyAction& operator =(const CtrlEndKeyAction& source);
};

class DeleteKeyAction : public KeyAction {
public:
	DeleteKeyAction(NotepadForm *notepadForm = 0);
	DeleteKeyAction(const DeleteKeyAction& source);
	virtual ~DeleteKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DeleteKeyAction& operator =(const DeleteKeyAction& source);
};

class BackSpaceKeyAction : public KeyAction {
public:
	BackSpaceKeyAction(NotepadForm *notepadForm = 0);
	BackSpaceKeyAction(const BackSpaceKeyAction& source);
	virtual ~BackSpaceKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BackSpaceKeyAction& operator =(const BackSpaceKeyAction& source);
};

class PageUpKeyAction : public KeyAction {
public:
	PageUpKeyAction(NotepadForm *notepadForm = 0);
	PageUpKeyAction(const PageUpKeyAction& source);
	virtual ~PageUpKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	PageUpKeyAction& operator=(const PageUpKeyAction& source);
};

class PageDownKeyAction : public KeyAction {
public:
	PageDownKeyAction(NotepadForm *notepadForm = 0);
	PageDownKeyAction(const PageDownKeyAction& source);
	virtual ~PageDownKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	PageDownKeyAction& operator=(const PageDownKeyAction& source);
};

#endif //_KEYACTIONS_H