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

class CtrlNKeyAction : public KeyAction {
public:
	CtrlNKeyAction(NotepadForm* notepadForm = 0);
	CtrlNKeyAction(const CtrlNKeyAction& source);
	virtual ~CtrlNKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlNKeyAction& operator =(const CtrlNKeyAction& source);
};

class CtrlOKeyAction : public KeyAction {
public:
	CtrlOKeyAction(NotepadForm* notepadForm = 0);
	CtrlOKeyAction(const CtrlOKeyAction& source);
	virtual ~CtrlOKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlOKeyAction& operator =(const CtrlOKeyAction& source);
};

class CtrlSKeyAction : public KeyAction {
public:
	CtrlSKeyAction(NotepadForm* notepadForm = 0);
	CtrlSKeyAction(const CtrlSKeyAction& source);
	virtual ~CtrlSKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlSKeyAction& operator =(const CtrlSKeyAction& source);
};

class CtrlShiftSKeyAction : public KeyAction {
public:
	CtrlShiftSKeyAction(NotepadForm* notepadForm = 0);
	CtrlShiftSKeyAction(const CtrlShiftSKeyAction& source);
	virtual ~CtrlShiftSKeyAction();
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CtrlShiftSKeyAction& operator =(const CtrlShiftSKeyAction& source);
};

#endif //_KEYACTIONS_H