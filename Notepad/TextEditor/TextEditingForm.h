#ifndef _TEXTEDITINGFORM_H
#define _TEXTEDITINGFORM_H

#include <afxwin.h>
#include "Subject.h"

class Glyph;
class CharacterMetrics;
class Font;
class CaretController;
class ScrollController;
class Selection;
class HistoryBook;
class FindReplaceDialog;

static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);

class TextEditingForm : public CWnd, public Subject {
public:
	TextEditingForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	Glyph* note;
	Glyph* current;
	CharacterMetrics* characterMetrics;
	Font* font;
	CaretController* caretController;
	ScrollController* scrollController;
	Selection* selection;
	HistoryBook* undoHistoryBook;
	HistoryBook* redoHistoryBook;
	FindReplaceDialog* findReplaceDialog;
protected:
	afx_msg void OnClose();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnImeComposition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeChar(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImeStartComposition(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlag, CPoint point);
	afx_msg void OnMouseMove(UINT nFlag, CPoint point);
	afx_msg void OnBasicCommandRange(UINT uID);
	afx_msg void OnEditCommandRange(UINT uID);
	afx_msg void OnMoveCommandRange(UINT uID);
	afx_msg void OnFlagCommandRange(UINT uID);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	BOOL GetIsComposing() const;
	void SetIsComposing(BOOL isComposing);
	char GetCurrentCharacter() const;
	void SetCurrentCharacter(char currentCharacter);
	TCHAR* GetCurrentBuffer() const;
	void SetCurrentBuffer(TCHAR(*currentBuffer));
	BOOL GetIsSized() const;
	void SetIsSized(BOOL isSized);
	Long GetSizedWidth() const;
	void SetSizedWidth(Long sizedWidth);
	Long GetPreviousWidth() const;
	void SetPreviousWidth(Long previousWidth);

	BOOL GetIsLockedHScroll() const;
	void SetIsLockedHScroll(BOOL isLockedHScroll);
	BOOL GetIsUnlockedHistoryBook() const;
	void SetIsUnlockedHistoryBook(BOOL isUnlockedHistoryBook);
	BOOL GetIsUnlockedFindReplaceDialog() const;
	void SetIsUnlockedFindReplaceDialog(BOOL isUnlockedFindReplaceDialog);

	BOOL GetIsDeleteSelectionByInput() const;
	void SetIsDeleteSelectionByInput(BOOL isDeleteSelectionByInput);
private:
	BOOL isComposing;
	char currentCharacter;
	TCHAR currentBuffer[2];
	BOOL isSized;
	Long sizedWidth; //OnSize 자동개행
	Long previousWidth; //SizeCommand

	BOOL isLockedHScroll; //자동개행여부
	BOOL isUnlockedHistoryBook; //실행취소가능여부
	BOOL isUnlockedFindReplaceDialog; //찾기바꾸기가능여부

	BOOL wasUndo; //실행취소관련
	BOOL wasMove; //실행취소관련
	BOOL isAllReplacing; //실행취소관련
	BOOL isDeleteSelectionByInput; //실행취소관련
};

inline BOOL TextEditingForm::GetIsComposing() const {
	return this->isComposing;
}

inline void TextEditingForm::SetIsComposing(BOOL isComposing) {
	this->isComposing = isComposing;
}

inline char TextEditingForm::GetCurrentCharacter() const {
	return this->currentCharacter;
}

inline void TextEditingForm::SetCurrentCharacter(char currentCharacter) {
	this->currentCharacter = currentCharacter;
}

inline TCHAR* TextEditingForm::GetCurrentBuffer() const {
	return const_cast<TCHAR*>(this->currentBuffer);
}

inline void TextEditingForm::SetCurrentBuffer(TCHAR(*currentBuffer)) {
	this->currentBuffer[0] = currentBuffer[0];
	this->currentBuffer[1] = currentBuffer[1];
}

inline BOOL TextEditingForm::GetIsSized() const {
	return this->isSized;
}

inline void TextEditingForm::SetIsSized(BOOL isSized) {
	this->isSized = isSized;
}

inline Long TextEditingForm::GetSizedWidth() const {
	return this->sizedWidth;
}

inline void TextEditingForm::SetSizedWidth(Long sizedWidth) {
	this->sizedWidth = sizedWidth;
}

inline Long TextEditingForm::GetPreviousWidth() const {
	return this->previousWidth;
}

inline void TextEditingForm::SetPreviousWidth(Long previousWidth) {
	this->previousWidth = previousWidth;
}

inline BOOL TextEditingForm::GetIsLockedHScroll() const {
	return this->isLockedHScroll;
}

inline void TextEditingForm::SetIsLockedHScroll(BOOL isLockedHScroll) {
	this->isLockedHScroll = isLockedHScroll;
}

inline BOOL TextEditingForm::GetIsUnlockedHistoryBook() const {
	return this->isUnlockedHistoryBook;
}

inline void TextEditingForm::SetIsUnlockedHistoryBook(BOOL isUnlockedHistoryBook) {
	this->isUnlockedHistoryBook = isUnlockedHistoryBook;
}

inline BOOL TextEditingForm::GetIsUnlockedFindReplaceDialog() const {
	return this->isUnlockedFindReplaceDialog;
}

inline void TextEditingForm::SetIsUnlockedFindReplaceDialog(BOOL isUnlockedFindReplaceDialog) {
	this->isUnlockedFindReplaceDialog = isUnlockedFindReplaceDialog;
}

inline BOOL TextEditingForm::GetIsDeleteSelectionByInput() const {
	return this->isDeleteSelectionByInput;
}

inline void TextEditingForm::SetIsDeleteSelectionByInput(BOOL isDeleteSelectionByInput) {
	this->isDeleteSelectionByInput = isDeleteSelectionByInput;
}

#endif //_TEXTEDITINGFORM_H