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
#include "HistoryBook.h"
#include "DrawingVisitor.h"
#include "Selection.h"
#include "AutoNewlineController.h"
#include "FindReplaceDialog.h"
#include "PrintJobManager.h"
#include "PrintStateDialog.h"

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
	ON_COMMAND_RANGE(IDM_FILE_NEW, IDM_FORMAT_FONT, OnCommandRange)
	ON_COMMAND_RANGE(IDC_WRITE_CHAR, IDM_EDIT_REPLACE, OnEditCommandRange)
	ON_COMMAND_RANGE(IDC_MOVE_LEFT, IDC_SELECTMOVE_RIGHT, OnMoveCommandRange)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_MENUSELECT()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
	ON_MESSAGE(WM_THREADNOTIFY, OnThreadNotify)
	//ON_UPDATE_COMMAND_UI_RANGE(IDM_FORMAT_WORDWRAP, IDM_FORMAT_WORDWRAP, OnUpdateCommandUIRange)
END_MESSAGE_MAP()

NotepadForm::NotepadForm() {
	this->note = NULL;
	this->current = NULL;
	this->font = NULL;
	this->characterMetrics = NULL;
	this->caretController = NULL;
	this->scrollController = NULL;
	this->document = NULL;
	this->undoHistoryBook = NULL;
	this->redoHistoryBook = NULL;
	this->selection = NULL;
	this->autoNewlineController = NULL;
	this->findReplaceDialog = NULL;
	this->printJobManager = NULL;
	this->printStateDialog = NULL;

	this->isComposing = FALSE;
	this->currentCharacter = '\0';
	this->currentBuffer[0] = '\0';
	this->currentBuffer[1] = '\0';
	this->wasUndo = FALSE;
	this->wasMove = FALSE;
	this->isAllReplacing = FALSE;
}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	GlyphFactory glyphFactory;
	this->note = glyphFactory.Make("");
	this->current = glyphFactory.Make("\r\n");
	this->note->Add(this->current);

	this->font = new Font(this);

	this->characterMetrics = new CharacterMetrics(this, this->font);

	this->menu.LoadMenuA(IDR_MENU1);
	this->SetMenu(&menu);

	this->document = new Document(this);

	this->undoHistoryBook = new HistoryBook(10);
	this->redoHistoryBook = new HistoryBook(10);

	this->printJobManager = new PrintJobManager(this);

	Long index = this->note->Move(0);
	this->current = this->note->GetAt(index);
	this->current->First();
	this->Notify();

	return 0;
}

void NotepadForm::OnClose() {
	if (this->note != NULL) {
		delete this->note;
		this->note = NULL;
	}
	if (this->document != NULL) {
		delete this->document;
	}
	if (this->undoHistoryBook != NULL) {
		delete this->undoHistoryBook;
	}
	if (this->redoHistoryBook != NULL) {
		delete this->redoHistoryBook;
	}
	if (this->selection != NULL) {
		delete this->selection;
	}
	if (this->findReplaceDialog != NULL) {
		delete this->findReplaceDialog;
	}
	if (this->printJobManager != NULL) {
		delete this->printJobManager;
	}
	if (this->printStateDialog != NULL) {
		delete this->printStateDialog;
	}

	CFrameWnd::OnClose();
}

void NotepadForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar >= 32 || nChar == VK_TAB || nChar == VK_RETURN) {
		this->currentCharacter = nChar;
		if (this->selection != NULL) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
		}
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_WRITE_CHAR, 0));
	}
}

LRESULT NotepadForm::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	if (lParam & GCS_COMPSTR) {
		this->currentBuffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		this->currentBuffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));
		if (this->selection != NULL) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
		}
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

	this->isComposing = FALSE;

	return 0;
}

LRESULT NotepadForm::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {
	return 0;
}

void NotepadForm::OnPaint() {
	CPaintDC dc(this);
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;

	CRect rect;
	this->GetClientRect(&rect);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* oldBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(&rect, RGB(255, 255, 255));

	CFont* oldFont;
	COLORREF oldColor;
	CFont font;
	this->font->Create(font);
	oldFont = memDC.SelectObject(&font);
	oldColor = memDC.SetTextColor(this->font->GetColor());

	Visitor* drawingVisitor = new DrawingVisitor(&memDC, this->characterMetrics, this->scrollController, this->selection);

	this->note->Accept(drawingVisitor);

	if (drawingVisitor != NULL) {
		delete drawingVisitor;
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(oldFont);
	memDC.SetTextColor(oldColor);

	memDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
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

	this->wasMove = TRUE;

	//선택하다 추가
	if (nFlag != 5) {
		if (this->selection != NULL) {
			this->note->UnselectAll();
			delete this->selection;
			this->selection = NULL;
		}
	}

	if (nFlag == 5) {
		Long startRow = noteCurrent;
		Long endRow = index;
		Long startColumn = lineCurrent;
		Long endColumn = lineIndex;
		if (noteCurrent > index) {
			startRow = index;
			endRow = noteCurrent;
			startColumn = lineIndex;
			endColumn = lineCurrent;
		}
		else if (noteCurrent == index) {
			if (lineCurrent > lineIndex) {
				startColumn = lineIndex;
				endColumn = lineCurrent;
			}
		}
		Glyph* line;
		Glyph* character;
		Long length;
		Long j;
		Long i = startRow;
		while (i <= endRow) {
			line = this->note->GetAt(i);

			length = line->GetLength();
			if (i == endRow) {
				length = endColumn;
			}

			j = 0;
			if (i == startRow) {
				j = startColumn;
			}

			while (j < length) {
				character = line->GetAt(j);
				if (!character->GetIsSelected()) {
					character->Select(true);
				}
				else {
					character->Select(false);
				}
				j++;
			}
			i++;
		}

		Long start = startRow;
		Long end = endRow;
		if (this->selection != NULL) {
			Long originStart = this->selection->GetStart();
			Long originEnd = this->selection->GetEnd();
			if (originEnd == noteCurrent) { //선택할 때
				start = originStart;
				end = row;
			}
			else if (originStart == noteCurrent) { //선택 해제할 때
				end = originEnd;
				start = row;
			}
			delete this->selection;
			this->selection = NULL;
		}
		this->selection = new Selection(start, end);

		if (start == end && this->note->IsSelecting() == false) {
			delete this->selection;
			this->selection = NULL;
		}
	}

	this->Notify();
	this->Invalidate(); //선택하다 추가
}

void NotepadForm::OnMouseMove(UINT nFlags, CPoint point) {
	if (nFlags == MK_LBUTTON) {
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

		Long startRow = noteCurrent;
		Long endRow = index;
		Long startColumn = lineCurrent;
		Long endColumn = lineIndex;
		if (noteCurrent > index) {
			startRow = index;
			endRow = noteCurrent;
			startColumn = lineIndex;
			endColumn = lineCurrent;
		}
		else if (noteCurrent == index) {
			if (lineCurrent > lineIndex) {
				startColumn = lineIndex;
				endColumn = lineCurrent;
			}
		}
		Glyph* line;
		Glyph* character;
		Long length;
		Long j;
		Long i = startRow;
		while (i <= endRow) {
			line = this->note->GetAt(i);

			length = line->GetLength();
			if (i == endRow) {
				length = endColumn;
			}

			j = 0;
			if (i == startRow) {
				j = startColumn;
			}

			while (j < length) {
				character = line->GetAt(j);
				if (!character->GetIsSelected()) {
					character->Select(true);
				}
				else {
					character->Select(false);
				}
				j++;
			}
			i++;
		}

		Long start = startRow;
		Long end = endRow;
		if (this->selection != NULL) {
			Long originStart = this->selection->GetStart();
			Long originEnd = this->selection->GetEnd();
			if (originEnd == noteCurrent) { //선택할 때
				start = originStart;
				end = row;
			}
			else if (originStart == noteCurrent) { //선택 해제할 때
				end = originEnd;
				start = row;
			}
			delete this->selection;
			this->selection = NULL;
		}
		this->selection = new Selection(start, end);

		if (start == end && this->note->IsSelecting() == false) {
			delete this->selection;
			this->selection = NULL;
		}

		this->Notify();
		this->Invalidate();
	}
}

void NotepadForm::OnCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		delete command;
	}

	if (this->scrollController != NULL) {
		delete this->scrollController;
	}
	this->scrollController = new ScrollController(this);

	this->Notify();
	this->Invalidate();

	Long x = this->characterMetrics->GetX(this->current) + 1; // 
	Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->scrollController->SmartScrollToPoint(x, y);
}

void NotepadForm::OnEditCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();

		//========== 실행 취소 추가 ==========
		string type = command->GetType();
		if (type != "ImeComposition"
			&& type != "Undo" && type != "Redo" && type != "Copy" && type != "SelectAll" && type != "Find" && type != "Replace") {
			if ((type == "Write" && this->currentCharacter != VK_RETURN)
				|| type == "ImeChar"
				|| type == "DeleteSelection") {
				Command* history;
				if (this->undoHistoryBook->GetLength() > 0 && this->undoHistoryBook->OpenAt()->GetType() == "Macro"
					&& (this->wasUndo == FALSE && this->wasMove == FALSE) || this->isAllReplacing == TRUE) {
					history = this->undoHistoryBook->OpenAt();
					history->Add(command->Clone());
				}
				else {
					history = new MacroCommand(this);
					history->Add(command->Clone());
					this->undoHistoryBook->Write(history);
					this->wasUndo = FALSE;
					this->wasMove = FALSE;
				}
			}
			else {
				this->undoHistoryBook->Write(command->Clone());
			}
			if (this->redoHistoryBook->GetLength() > 0) {
				this->redoHistoryBook->Empty();
			}
		}
		if (type == "Undo") {
			this->wasUndo = TRUE;
		}
		//========== 실행 취소 추가 ==========

		delete command;
	}

	if (this->scrollController != NULL) {
		delete this->scrollController;
	}
	this->scrollController = new ScrollController(this);

	this->Notify();
	this->Invalidate();

	Long x = this->characterMetrics->GetX(this->current) + 1; // 
	Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->scrollController->SmartScrollToPoint(x, y);
}

void NotepadForm::OnMoveCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		this->wasMove = TRUE;

		Long x = this->characterMetrics->GetX(this->current) + 1; // 
		Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0베이스이므로 1더함
		this->scrollController->SmartScrollToPoint(x, y);

		delete command;
	}
	this->Notify();
	this->Invalidate();
}

void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyActionFactory keyActionFactory(this);
	KeyAction* keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
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

BOOL NotepadForm::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

LRESULT NotepadForm::OnFindReplace(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);

	if (this->findReplaceDialog != NULL) {
		BOOL isFindSuccess;
		if (this->findReplaceDialog->FindNext()) {
			isFindSuccess = this->findReplaceDialog->Find();
			if (isFindSuccess == FALSE) {
				CString messageText;
				messageText.Format("\"%s\"을(를) 찾을 수 없습니다.", this->findReplaceDialog->GetFindString());
				MessageBox((LPCTSTR)messageText, "메모장", MB_OK);
			}
		}
		else if (this->findReplaceDialog->ReplaceCurrent()) {
			this->findReplaceDialog->Replace();
			isFindSuccess = this->findReplaceDialog->Find();
			if (isFindSuccess == FALSE) {
				CString messageText;
				messageText.Format("\"%s\"을(를) 찾을 수 없습니다.", this->findReplaceDialog->GetFindString());
				MessageBox((LPCTSTR)messageText, "메모장", MB_OK);
			}
		}
		else if (this->findReplaceDialog->ReplaceAll()) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_CTRLHOME, 0));
			isFindSuccess = this->findReplaceDialog->Find();
			while (isFindSuccess == TRUE) {
				this->findReplaceDialog->Replace();
				this->isAllReplacing = TRUE;
				isFindSuccess = this->findReplaceDialog->Find();
			}
			this->isAllReplacing = FALSE;
		}
		else if (this->findReplaceDialog->IsTerminating()) {
			this->findReplaceDialog = NULL; //No memory leak detected although do not call DestroyWindow() or destructor
		}

		this->Notify();
		this->Invalidate();
	}

	return 0;
}

LRESULT NotepadForm::OnThreadNotify(WPARAM wParam, LPARAM lParam) {
	if (this->printStateDialog != NULL) {
		this->printStateDialog->DestroyWindow();
		this->printStateDialog = NULL;
	}
	
	return 0;
}