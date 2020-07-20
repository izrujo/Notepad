#ifndef _NOTEPADFORM_H
#define _NOTEPADFORM_H

#include <afxwin.h>
#include "Subject.h"

class ScrollController;
class Font;
class CaretController;
class CharacterMetrics;
class Glyph;
class Document;
class Editor;

class NotepadForm : public CFrameWnd, public Subject {
public:
	NotepadForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	Glyph *note;
	Glyph *current;
	CaretController *caretController;
	Font *font;
	CharacterMetrics *characterMetrics;
	ScrollController *scrollController;
	Document *document;
	Glyph* highlight;
	Editor* editor;
protected:
	afx_msg void OnClose();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cs, int cy);
	afx_msg void OnSetFocus(CWnd *pNewWnd);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlag, CPoint point);
	afx_msg void OnMouseMove(UINT nFlag, CPoint point);
	afx_msg void OnCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//afx_msg void OnUpdateCommandUIRange(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
public:
	BOOL GetIsComposing() const;
private:
	BOOL isComposing;
	CMenu menu;
};

inline BOOL NotepadForm::GetIsComposing() const {
	return this->isComposing;
}

#endif //_NOTEPADFORM_H