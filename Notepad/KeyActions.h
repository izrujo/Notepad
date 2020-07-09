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

#endif //_KEYACTIONS_H