#include "NotepadForm.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "Note.h"
#include "Line.h"
#include "CaretController.h"
#include "CharacterMetrics.h"
#include "Font.h"
#include "CommandFactory.h"
#include "Commands.h"
#include "KeyActionFactory.h"
#include "KeyActions.h"
#include <WinUser.h>
#include <imm.h>
#include <afxdlgs.h>
#include "resource.h"
#include "ScrollController.h"
#include "VScrollActionFactory.h"
#include "VScrollActions.h"
#include "HScrollActionFactory.h"
#include "HScrollActions.h"
#include "Scroll.h"
#include "Document.h"

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_CHAR()
	ON_MESSAGE(WM_IME_COMPOSITION, OnImeComposition)
	ON_MESSAGE(WM_IME_CHAR, OnImeChar)
	ON_MESSAGE(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND_RANGE(IDM_FORMAT_FONT, IDM_FORMAT_WORDWRAPCANCEL, OnCommandRange)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MENUSELECT()
	//ON_UPDATE_COMMAND_UI_RANGE(IDM_FORMAT_WORDWRAP, IDM_FORMAT_WORDWRAP, OnUpdateCommandUIRange)
END_MESSAGE_MAP()

NotepadForm::NotepadForm() {
	this->note = NULL;
	this->current = NULL;
	this->caretController = NULL;
	this->font = NULL;
	this->characterMetrics = NULL;
	this->isComposing = FALSE;
	this->scrollController = NULL;
	this->document = NULL;
}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	GlyphFactory glyphFactory;
	this->note = glyphFactory.Make("");
	this->current = glyphFactory.Make("\r\n");
	this->note->Add(this->current);

	this->font = new Font(this);

	this->characterMetrics = new CharacterMetrics(this);

	this->menu.LoadMenuA(IDR_MENU1);
	this->SetMenu(&menu);
	this->menu.CheckMenuItem(IDM_FORMAT_WORDWRAP, MF_UNCHECKED | MF_BYCOMMAND);

	this->document = new Document(this);

	Long index = this->note->Move(0);
	this->current = this->note->GetAt(index);
	this->current->First();
	this->Notify();

	return 0;
}

void NotepadForm::OnClose() {
	if (this->characterMetrics != NULL) {
		delete this->characterMetrics;
	}
	if (this->font != NULL) {
		delete this->font;
	}
	if (this->note != NULL) {
		delete this->note;
		this->note = NULL;
	}
	if (this->document != NULL) {
		delete this->document;
	}

	CFrameWnd::OnClose();
}

void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	GlyphFactory glyphFactory;
	TCHAR content[2];
	Long index;

	if (nChar >= 32 || nChar == VK_TAB) {
		content[0] = nChar;
		Glyph* character = glyphFactory.Make(content);

		index = this->current->GetCurrent();

		if (index >= this->current->GetLength()) {
			this->current->Add(character);
		}
		else {
			this->current->Add(index, character);
		}
	}
	else if (nChar == VK_RETURN) {
		index = this->current->GetCurrent();

		if (index < this->current->GetLength()) {
			this->current = this->current->Divide(index);
			index = this->note->GetCurrent();
			this->note->Add(index + 1, this->current);
			this->current->First();
		}
		else {
			this->current = glyphFactory.Make("\r\n");
			index = this->note->GetCurrent();
			this->note->Add(index + 1, this->current);
		}
	}
	this->Notify();
	this->Invalidate();
	
	if (this->document->GetIsDirty() == false) {
		CString title;
		this->GetWindowText(title);
		title.Insert(0, '*');
		this->SetWindowTextA(title);
		this->document->SetIsDirty(true);
	}

	//스마트스크롤 부분
	Long x = this->characterMetrics->GetX(this->current) + 1;
	Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->scrollController->SmartScrollToPoint(x, y);
}

LRESULT NotepadForm::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	TCHAR buffer[2];
	Long index;

	if (lParam & GCS_COMPSTR) {
		buffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		buffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));

		if (this->isComposing == TRUE) {
			index = this->current->GetCurrent();
			this->current->Remove(index - 1);
		}

		if (buffer[0] != '\0') {
			this->isComposing = TRUE;
			GlyphFactory glyphFactory;
			Glyph* doubleByteCharacter = glyphFactory.Make(buffer);
			index = this->current->GetCurrent();

			if (index >= this->current->GetLength()) {
				this->current->Add(doubleByteCharacter);
			}
			else {
				this->current->Add(index, doubleByteCharacter);
			}
		}
		else {
			this->isComposing = FALSE;
		}
		this->Notify();
		this->Invalidate();

		if (this->document->GetIsDirty() == false) {
			CString title;
			this->GetWindowText(title);
			title.Insert(0, '*');
			this->SetWindowTextA(title);
			this->document->SetIsDirty(true);
		}

		//스마트스크롤 부분
		Long x = this->characterMetrics->GetX(this->current) + 1; // 
		Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
		this->scrollController->SmartScrollToPoint(x, y);

	}

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam) {
	TCHAR buffer[2];
	Long index = this->current->GetCurrent();
	this->current->Remove(index - 1);

	if (IsDBCSLeadByte((BYTE)(wParam >> 8)) == TRUE) {
		buffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		buffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));
	}
	else {
		buffer[0] = (TCHAR)wParam;
	}

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Make(buffer);

	if (this->current->GetCurrent() >= this->current->GetLength()) {
		this->current->Add(glyph);
	}
	else {
		this->current->Add(this->current->GetCurrent(), glyph);
	}

	this->isComposing = FALSE;
	this->Notify();
	this->Invalidate();

	if (this->document->GetIsDirty() == false) {
		CString title;
		this->GetWindowText(title);
		title.Insert(0, '*');
		this->SetWindowTextA(title);
		this->document->SetIsDirty(true);
	}
	
	//스마트스크롤 부분
	Long x = this->characterMetrics->GetX(this->current) + 1; // 
	Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->scrollController->SmartScrollToPoint(x, y);

	return 0;
}

LRESULT NotepadForm::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {
	return 0;
}

void NotepadForm::OnPaint() {
	Glyph* line;
	string content;
	CPaintDC dc(this);
	CFont* oldFont;
	COLORREF oldColor;
	CFont font;
	this->font->Create(font);
	oldFont = dc.SelectObject(&font);
	oldColor = dc.SetTextColor(this->font->GetColor());
	Long x = this->scrollController->GetHorizontalScroll()->GetPosition();
	Long y = this->scrollController->GetVerticalScroll()->GetPosition();

	RECT rect;
	this->GetClientRect(&rect);
	Long clientTop = y;
	Long clientBottom = (rect.bottom - rect.top) + y;

	Long begin = this->characterMetrics->GetRow(y);
	Long end = this->characterMetrics->GetRow(clientBottom);

	Long i = begin;
	while (i < this->note->GetLength() && i < end) {
		line = this->note->GetAt(i);
		content = line->GetContent();
		dc.TextOutA(-x, i * (this->characterMetrics->GetHeight()) - y, content.c_str());
		i++;
	}
	dc.SelectObject(oldFont);
	dc.SetTextColor(oldColor);
}

void NotepadForm::OnSize(UINT nType, int cs, int cy) {
	if (this->scrollController == NULL) {
		this->scrollController = new ScrollController(this);
	}
	if (this->note != NULL) {
		this->Notify();
	}

}

void NotepadForm::OnSetFocus(CWnd* pNewWnd) {
	if (this->caretController == NULL) {
		this->caretController = new CaretController(this);
	}
	this->Notify();
}

void NotepadForm::OnKillFocus(CWnd* pNewWnd) {
	if (this->caretController != NULL) {
		delete this->caretController;
		this->caretController = NULL;
	}
}

void NotepadForm::OnLButtonDown(UINT nFlag, CPoint point) {
	Long row = this->characterMetrics->GetRow(this->scrollController->GetVerticalScroll()->GetPosition() + point.y);
	if (row >= this->note->GetLength()) {
		row = this->note->GetLength() - 1;
	}
	Long index = this->note->Move(row);
	this->current = this->note->GetAt(index);
	Long column = this->characterMetrics->GetColumn(this->current, this->scrollController->GetHorizontalScroll()->GetPosition() + point.x);
	this->current->Move(column);

	this->Notify();
}

void NotepadForm::OnCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	command->Execute();
	if (command != NULL) {
		delete command;
	}
	
	if (this->scrollController != NULL) {
		delete this->scrollController;
	}
	this->scrollController = new ScrollController(this);
	this->Notify();

	this->Invalidate();
}

void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyActionFactory keyActionFactory(this);
	KeyAction* keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		//스마트스크롤 부분
		Long x = this->characterMetrics->GetX(this->current) + 1; // 
		Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
		this->scrollController->SmartScrollToPoint(x, y);

		delete keyAction;
	}

	this->Notify();
	this->Invalidate();
}

void NotepadForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	HScrollActionFactory hScrollFactory(this);
	HScrollAction* hScrollAction = hScrollFactory.Make(nSBCode);
	if (hScrollAction != NULL) {
		hScrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		delete hScrollAction;
	}
	this->Invalidate();
}

void NotepadForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	VScrollActionFactory vScrollActionFactory(this);

	VScrollAction* vScrollAction = vScrollActionFactory.Make(nSBCode);
	if (vScrollAction != NULL) {
		vScrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		delete vScrollAction;
	}
	this->Invalidate();

}

BOOL NotepadForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {

	LONG style = this->GetStyle();

	if ((style & WS_VSCROLL) != 0) {
		Long position = this->scrollController->Rotate(zDelta);
		Long previousPosition = this->SetScrollPos(SB_VERT, position, TRUE);
		position = this->GetScrollPos(SB_VERT);
		this->scrollController->MoveVerticalScroll(position);
		this->ScrollWindow(0, previousPosition - position);
		this->Notify();
	}
	return TRUE;
}