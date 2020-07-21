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
#include "Editor.h"
#include "Selector.h"
#include "Highlight.h"

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
	ON_WM_MOUSEMOVE()
	ON_COMMAND_RANGE(IDM_FILE_NEW, IDC_BACKSPACE_CHAR, OnCommandRange)
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
	this->scrollController = NULL;
	this->document = NULL;
	this->highlight = NULL;
	this->editor = NULL;

	this->isComposing = FALSE;
	this->currentCharacter = '\0';
	this->currentBuffer[0] = '\0';
	this->currentBuffer[1] = '\0';
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

	this->document = new Document(this);

	this->editor = new Editor(this);

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
	if (this->highlight != NULL) {
		delete this->highlight;
	}
	if (this->editor != NULL) {
		delete this->editor;
	}

	CFrameWnd::OnClose();
}

void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->currentCharacter = nChar;
	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_WRITE_CHAR, 0));
}

LRESULT NotepadForm::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	if (lParam & GCS_COMPSTR) {
		this->currentBuffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		this->currentBuffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_IME_COMPOSITION, 0));
	}

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT NotepadForm::OnImeChar(WPARAM wParam, LPARAM lParam) {
	if (IsDBCSLeadByte((BYTE)(wParam >> 8)) == TRUE) {
		this->currentBuffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		this->currentBuffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));
	}
	else {
		this->currentBuffer[0] = (TCHAR)wParam;
	}

	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_IME_CHAR, 0));

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

	if (this->highlight != NULL) {
		COLORREF oldBkColor = dc.SetBkColor(RGB(0, 0, 235));
		oldColor = dc.SetTextColor(RGB(255, 255, 255));

		Long noteStartPosition = this->editor->selector->GetNoteStartPosition();
		Long noteEndPosition = this->editor->selector->GetNoteEndPosition();
		Long height = this->characterMetrics->GetHeight();

		Long lineStartPosition = this->editor->selector->GetLineStartPosition();
		Glyph* startLine = this->note->GetAt(noteStartPosition);
		Long startLineX = this->characterMetrics->GetX(startLine, lineStartPosition);

		Long lineEndPosition = this->editor->selector->GetLineEndPosition();
		Glyph* endLine = this->note->GetAt(noteEndPosition);
		Long endX = this->characterMetrics->GetX(endLine, lineEndPosition);

		Long x;
		Long y = noteStartPosition * height;
		if (noteStartPosition < noteEndPosition) {
			x = startLineX;
		}
		else if (noteStartPosition > noteEndPosition) {
			y = noteEndPosition * height;
			x = endX;
		}
		else {
			if (lineStartPosition <= lineEndPosition) {
				x = startLineX;
			}
			else {
				x = endX;
			}
		}

		i = 0;
		while (i < this->highlight->GetLength()) {
			Glyph* highlight = this->highlight->GetAt(i);
			string content = highlight->GetContent();
			dc.TextOutA(x, y, content.c_str());
			y += height;
			x = 0;
			i++;
		}

		dc.SetBkColor(oldBkColor);
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
	Long noteCurrent = this->note->GetCurrent();
	Long lineCurrent = this->current->GetCurrent();
	
	Long row = this->characterMetrics->GetRow(this->scrollController->GetVerticalScroll()->GetPosition() + point.y);
	if (row >= this->note->GetLength()) {
		row = this->note->GetLength() - 1;
	}
	Long index = this->note->Move(row);
	this->current = this->note->GetAt(index);
	Long column = this->characterMetrics->GetColumn(this->current, this->scrollController->GetHorizontalScroll()->GetPosition() + point.x);
	Long lineIndex = this->current->Move(column);

	//선택하다 추가
	if (nFlag != 5) {
		if (this->highlight != NULL) {
			delete this->highlight;
			this->highlight = NULL;
		}
	}

	if (nFlag == 5) {
		if (this->highlight == NULL) {
			this->highlight = new Highlight;
			this->editor->selector = new Selector(this, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
		}
		if (noteCurrent == index && lineCurrent > lineIndex) {
			Long endColumn = column;
			Long startColumn = lineCurrent;
			Long noteStartPosition = this->editor->selector->GetNoteStartPosition();
			Long noteEndPosition = this->editor->selector->GetNoteEndPosition();
			Long lineStartPosition = this->editor->selector->GetLineStartPosition();
			Long lineEndPosition = this->editor->selector->GetLineEndPosition();
			if (noteStartPosition < noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition < lineEndPosition)) {
				startColumn = endColumn;
			}
			this->editor->selector->Left(noteCurrent, startColumn, endColumn);
		}
		else if (noteCurrent == index && lineCurrent < lineIndex) {
			Long endColumn = column;
			Long startColumn = lineCurrent;
			Long noteStartPosition = this->editor->selector->GetNoteStartPosition();
			Long noteEndPosition = this->editor->selector->GetNoteEndPosition();
			Long lineStartPosition = this->editor->selector->GetLineStartPosition();
			Long lineEndPosition = this->editor->selector->GetLineEndPosition();
			if (noteStartPosition > noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition > lineEndPosition)) {
				startColumn = endColumn;
			}
			this->editor->selector->Right(noteCurrent, startColumn, endColumn);
		}

		if (index < noteCurrent) {
			this->editor->UpSelect(noteCurrent, lineCurrent, index, lineIndex);
		}
		else if (index > noteCurrent) {
			this->editor->DownSelect(noteCurrent, lineCurrent, index, lineIndex);
		}
	}

	this->Notify();
	this->Invalidate(); //선택하다 추가
}

void NotepadForm::OnMouseMove(UINT nFlags, CPoint point) {
	if (nFlags == MK_LBUTTON) {
		Long noteCurrent = this->note->GetCurrent();
		Long lineCurrent = this->current->GetCurrent();
		if (this->highlight == NULL) {
			this->highlight = new Highlight;
			this->editor->selector = new Selector(this, noteCurrent, lineCurrent, noteCurrent, lineCurrent);
		}

		Long row = this->characterMetrics->GetRow(point.y);
		if (row >= this->note->GetLength()) {
			row = this->note->GetLength() - 1;
		}
		Long index = this->note->Move(row);
		this->current = this->note->GetAt(index);
		Long column = this->characterMetrics->GetColumn(this->current, point.x);
		Long lineIndex = this->current->Move(column);

		if (noteCurrent == index && lineCurrent > lineIndex) {
			Long endColumn = column;
			Long startColumn = lineCurrent;
			Long noteStartPosition = this->editor->selector->GetNoteStartPosition();
			Long noteEndPosition = this->editor->selector->GetNoteEndPosition();
			Long lineStartPosition = this->editor->selector->GetLineStartPosition();
			Long lineEndPosition = this->editor->selector->GetLineEndPosition();
			if (noteStartPosition < noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition < lineEndPosition)) {
				startColumn = endColumn;
			}
			this->editor->selector->Left(noteCurrent, startColumn, endColumn);
		}
		else if (noteCurrent == index && lineCurrent < lineIndex) {
			Long endColumn = column;
			Long startColumn = lineCurrent;
			Long noteStartPosition = this->editor->selector->GetNoteStartPosition();
			Long noteEndPosition = this->editor->selector->GetNoteEndPosition();
			Long lineStartPosition = this->editor->selector->GetLineStartPosition();
			Long lineEndPosition = this->editor->selector->GetLineEndPosition();
			if (noteStartPosition > noteEndPosition || (noteStartPosition == noteEndPosition && lineStartPosition > lineEndPosition)) {
				startColumn = endColumn;
			}
			this->editor->selector->Right(noteCurrent, startColumn, endColumn);
		}

		if (index < noteCurrent) {
			this->editor->UpSelect(noteCurrent, lineCurrent, index, lineIndex);
		}
		else if (index > noteCurrent) {
			this->editor->DownSelect(noteCurrent, lineCurrent, index, lineIndex);
		}

		this->Notify();
		this->Invalidate();
	}
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