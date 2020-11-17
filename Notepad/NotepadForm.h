#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include <afxwin.h>
#include <iostream>

using namespace std;
#include "Subject.h"

#define WM_THREADNOTIFY (WM_APP + 2000)

class ScrollController;
class Font;
class CaretController;
class CharacterMetrics;
class Glyph;
class Document;
class HistoryBook;
class Selection;
class FindReplaceDialog;
class PrintJobManager;
class PrintStateDialog;

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

class NotepadForm : public CFrameWnd, public Subject {
public:
	NotepadForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	CMenu menu;
	Glyph* note;
	Glyph* current;
	Font* font;
	CharacterMetrics* characterMetrics;
	CaretController* caretController;
	ScrollController* scrollController;
	Document* document;
	HistoryBook* undoHistoryBook;
	HistoryBook* redoHistoryBook;
	Selection* selection;
	FindReplaceDialog* findReplaceDialog;
	PrintJobManager* printJobManager;
	PrintStateDialog* printStateDialog;

protected:
	afx_msg void OnClose();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg void OnSetFocus(CWnd* pNewWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlag, CPoint point);
	afx_msg void OnMouseMove(UINT nFlag, CPoint point);
	afx_msg void OnCommandRange(UINT uID);
	afx_msg void OnEditCommandRange(UINT uID);
	afx_msg void OnMoveCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThreadNotify(WPARAM wParaml, LPARAM lParam);
	//afx_msg void OnUpdateCommandUIRange(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	BOOL GetIsComposing() const;
	void SetIsComposing(BOOL isComposing);
	char GetCurrentCharacter() const;
	void SetCurrentCharacter(char currentCharacter);
	TCHAR* GetCurrentBuffer() const;
	void SetCurrentBuffer(TCHAR(*currentBuffer));

	BOOL GetIsAutoNewLining() const;
	void SetIsAutoNewLining(BOOL isComposing);

private:
	BOOL isComposing;
	char currentCharacter;
	TCHAR currentBuffer[2];
	BOOL wasUndo;
	BOOL wasMove;
	BOOL isAllReplacing;
	BOOL isAutoNewLining;
	Long previousWidth;
};

inline BOOL NotepadForm::GetIsComposing() const {
	return this->isComposing;
}

inline void NotepadForm::SetIsComposing(BOOL isComposing) {
	this->isComposing = isComposing;
}

inline char NotepadForm::GetCurrentCharacter() const {
	return this->currentCharacter;
}

inline void NotepadForm::SetCurrentCharacter(char currentCharacter) {
	this->currentCharacter = currentCharacter;
}

inline TCHAR* NotepadForm::GetCurrentBuffer() const {
	return const_cast<TCHAR*>(this->currentBuffer);
}

inline void NotepadForm::SetCurrentBuffer(TCHAR(*currentBuffer)) {
	this->currentBuffer[0] = currentBuffer[0];
	this->currentBuffer[1] = currentBuffer[1];
}

inline BOOL NotepadForm::GetIsAutoNewLining() const {
	return this->isAutoNewLining;
}

inline void NotepadForm::SetIsAutoNewLining(BOOL isAutoNewLining) {
	this->isAutoNewLining = isAutoNewLining;
}

#endif //_NOTEPADFORM_H