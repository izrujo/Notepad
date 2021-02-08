#include "TextEditingForm.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "CharacterMetrics.h"
#include "Font.h"
#include "DrawingVisitor.h"
#include "CaretController.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "Selection.h"
#include "CNTCommandFactory.h"
#include "CNTCommands.h"
#include "VScrollActionFactory.h"
#include "VScrollActions.h"
#include "HScrollActionFactory.h"
#include "HScrollActions.h"
#include "CNTKeyActionFactory.h"
#include "CNTKeyActions.h"
#include "HistoryBook.h"
#include "DummyManager.h"
#include "FindReplaceDialog.h"

#include "resource.h"

BEGIN_MESSAGE_MAP(TextEditingForm, CWnd)
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
	ON_COMMAND_RANGE(IDCNT_EDIT_WRITE, IDCNT_EDIT_REPLACE, OnEditCommandRange)
	ON_COMMAND_RANGE(IDCNT_BASIC_WRITE, IDCNT_BASIC_DELETESELECTION, OnBasicCommandRange)
	ON_COMMAND_RANGE(IDCNT_MOVE_LEFT, IDCNT_SELECTMOVE_CTRLEND, OnMoveCommandRange)
	ON_COMMAND_RANGE(IDCNT_FLAG_LOCKHSCROLL, /*IDCNT_FLAG_UNLOCKFINDREPLACEDIALOG*/IDCNT_ETC_SIZE, OnFlagCommandRange)
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_REGISTERED_MESSAGE(WM_FINDREPLACE, OnFindReplace)
END_MESSAGE_MAP()

TextEditingForm::TextEditingForm() {
	this->note = NULL;
	this->current = NULL;
	this->font = NULL;
	this->characterMetrics = NULL;
	this->caretController = NULL;
	this->scrollController = NULL;
	this->selection = NULL;
	this->undoHistoryBook = NULL;
	this->redoHistoryBook = NULL;
	this->findReplaceDialog = NULL;

	this->isComposing = FALSE;
	this->currentCharacter = '\0';
	this->currentBuffer[0] = '\0';
	this->currentBuffer[1] = '\0';
	this->sizedWidth = 0;
	this->isSized = TRUE;
	this->previousWidth = 0;

	this->isLockedHScroll = FALSE;
	this->isUnlockedHistoryBook = FALSE;
	this->isUnlockedFindReplaceDialog = FALSE;

	this->wasUndo = FALSE;
	this->wasMove = FALSE;
	this->isAllReplacing = FALSE;
	this->isDeleteSelectionByInput = FALSE;
}

int TextEditingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CWnd::OnCreate(lpCreateStruct);

	GlyphFactory glyphFactory;
	this->note = glyphFactory.Make("");
	this->current = glyphFactory.Make("\r\n");
	this->note->Add(this->current);

	this->font = new Font(this);

	this->characterMetrics = new CharacterMetrics(this, this->font);

	this->undoHistoryBook = new HistoryBook(10);
	this->redoHistoryBook = new HistoryBook(10);

	Long index = this->note->Move(0);
	this->current = this->note->GetAt(index);
	this->current->First();
	this->Notify();

	return 0;
}

void TextEditingForm::OnClose() {
	if (this->note != NULL) {
		delete this->note;
		this->note = NULL; // OnClose ���� OnSize ȣ���.
	}
	if (this->characterMetrics != NULL) {
		delete this->characterMetrics;
	}
	if (this->selection != NULL) {
		delete this->selection;
	}
	if (this->undoHistoryBook != NULL) {
		delete this->undoHistoryBook;
	}
	if (this->redoHistoryBook != NULL) {
		delete this->redoHistoryBook;
	}
	if (this->findReplaceDialog != NULL) {
		delete this->findReplaceDialog;
	}

	CWnd::OnClose();
}

void TextEditingForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar >= 32 || nChar == VK_TAB || nChar == VK_RETURN) {
		this->currentCharacter = nChar;
		if (this->selection != NULL) {
			this->isDeleteSelectionByInput = TRUE;
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
			this->isDeleteSelectionByInput = FALSE;
		}
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_WRITE, 0));
	}
}

LRESULT TextEditingForm::OnImeComposition(WPARAM wParam, LPARAM lParam) {
	if (lParam & GCS_COMPSTR) {
		this->currentBuffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		this->currentBuffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));
		if (this->selection != NULL) {
			this->isDeleteSelectionByInput = TRUE;
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
			this->isDeleteSelectionByInput = FALSE;
		}
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_IMECOMPOSITION, 0));
	}

	return ::DefWindowProc(this->m_hWnd, WM_IME_COMPOSITION, wParam, lParam);
}

LRESULT TextEditingForm::OnImeChar(WPARAM wParam, LPARAM lParam) {
	if (IsDBCSLeadByte((BYTE)(wParam >> 8)) == TRUE) {
		this->currentBuffer[0] = (TCHAR)HIBYTE(LOWORD(wParam));
		this->currentBuffer[1] = (TCHAR)LOBYTE(LOWORD(wParam));
	}
	else {
		this->currentBuffer[0] = (TCHAR)wParam;
	}

	this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_IMECHAR, 0));

	this->isComposing = FALSE;

	return 0;
}

LRESULT TextEditingForm::OnImeStartComposition(WPARAM wParam, LPARAM lParam) {
	return 0;
}

void TextEditingForm::OnPaint() {
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

void TextEditingForm::OnSize(UINT nType, int cx, int cy) {
	if (this->isLockedHScroll == TRUE && this->sizedWidth != cx) {
		DummyManager dummyManager(this->note, this->characterMetrics, cx);

		Long start;
		Long startColumn;
		Long end;
		Long endColumn;
		Long startDistance;
		Long endDistance;
		if (this->selection != NULL) {
			start = this->selection->GetStart();
			end = this->selection->GetEnd();
			startColumn = this->note->GetSelectedStartColumn(start);
			endColumn = this->note->GetSelectedEndColumn(end);
			startDistance = dummyManager.CountDistance(start, startColumn);
			endDistance = dummyManager.CountDistance(end, endColumn);
		}

		Long row = this->note->GetCurrent();
		Long column = this->current->GetCurrent();
		Long distance = dummyManager.CountDistance(row, column);
		Long i = 0;
		while (i < this->note->GetLength()) {
			dummyManager.Unfold(i);
			i++;
		}
		i = 0;
		while (i < this->note->GetLength()) {
			i = dummyManager.Fold(i);
			i++;
		}
		dummyManager.CountIndex(distance, &row, &column);
		this->note->Move(row);
		this->current = this->note->GetAt(row);
		this->current->Move(column);

		if (this->selection != NULL) {
			delete this->selection;
			dummyManager.CountIndex(startDistance, &start, &startColumn);
			dummyManager.CountIndex(endDistance, &end, &endColumn);
			this->selection = new Selection(start, end);
		}

		this->isSized = TRUE;
	}
	this->sizedWidth = cx; //���ʿ��� ����ó���� ���� ����(������� ������ ũ�� ������ ����ó�� �ϱ� ����)

	if (this->scrollController == NULL) {
		this->scrollController = new ScrollController(this);
	}
	if (this->note != NULL) {
		this->Notify();
	}
}

void TextEditingForm::OnSetFocus(CWnd* pOldWnd) {
	if (this->caretController == NULL) {
		this->caretController = new CaretController(this);
	}
	this->Notify();
}

void TextEditingForm::OnKillFocus(CWnd* pNewWnd) {
	if (this->caretController != NULL) {
		delete this->caretController;
		this->caretController = NULL;
	}
}

void TextEditingForm::OnLButtonDown(UINT nFlag, CPoint point) {
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

	//�����ϴ� �߰�
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
			if (originEnd == noteCurrent) { //������ ��
				start = originStart;
				end = row;
			}
			else if (originStart == noteCurrent) { //���� ������ ��
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
	this->Invalidate(); //�����ϴ� �߰�
}

void TextEditingForm::OnMouseMove(UINT nFlag, CPoint point) {
	if (nFlag == MK_LBUTTON) {
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
			if (originEnd == noteCurrent) { //������ ��
				start = originStart;
				end = row;
			}
			else if (originStart == noteCurrent) { //���� ������ ��
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

void TextEditingForm::OnEditCommandRange(UINT uID) {
	CNTCommandFactory commandFactory(this);
	CNTCommand* command = commandFactory.Make(uID);
	if (command != NULL) {
		string type = command->GetType();

		//SizeCommand �߰�
		if ((type == "CNTWrite" || type == "CNTImeChar" || type == "CNTDelete"
			|| type == "CNTDeleteSelection" || type == "CNTPaste")
			&& (this->isSized == TRUE || this->undoHistoryBook->IsEmpty())) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_ETC_SIZE, 0));
			this->previousWidth = this->sizedWidth;
			this->isSized = FALSE;
		}

		command->Execute();

		//========== ���� ��� �߰� ==========
		if (this->isUnlockedHistoryBook == TRUE) {
			CNTCommand* sizeCommand = this->undoHistoryBook->OpenAt();
			//1. Undo�ƴϰ� Redo�ƴϰ� Find�ƴϰ� Replace�ƴϰ�
			//1.1. DeleteSelection�̸�
			//1.1.1. ��������̷��������鼭�����ֱ��̷��̸�ũ���̰� ��ݽ�����Ҹ��������鼭����̵������߰ų� ���ݸ�ιٲٱ����̸�
			//1.1.1.1. (��ũ�ο�) ���� Ŀ�ǵ带 �߰��ϴ�.
			//1.1.2. ��������̷��̾��ų� ��ݽ�����Ҹ��߰ų�����̵����߰� ���ݸ�ιٲٱ����̾ƴϸ�
			//1.1.2.1. ��ũ�θ� ���� Ŀ�ǵ带 �߰��ϰ� �� ��ũ�θ� ��������̷����� ����.
			//1.1.2.2. ������Ҿ��ߴٰ� �׸��� ���������ʾҴٰ� ����صд�.
			//1.2. deleteSelection�ƴϸ�
			//1.2.1. (��ũ�ε�ƴϵ�)��������̷����� ����.
			//1.3. �ٽý����̷��� ������ �ٽý����̷��� �� �����.
			//2. Undo�� ��������ߴٰ� ����صд�.
			if (type != "CNTImeComposition"
				&& type != "CNTUndo" && type != "CNTRedo" && type != "CNTCopy" && type != "CNTCut" && type != "CNTSelectAll"
				&& type != "CNTFind" && type != "CNTReplace") {
				//!!����!! DeleteSelection�� �Բ� ������ EnterŰ�� ��� ���� Macro�� ���� �����ϴ� !!����!!
				if ((type == "CNTWrite" && this->currentCharacter != VK_RETURN)
					|| type == "CNTImeChar" || type == "CNTPaste"
					|| (type == "CNTDeleteSelection" && this->isDeleteSelectionByInput == TRUE)) {
					CNTCommand* history;
					if (sizeCommand->GetLength() > 0 && sizeCommand->OpenAt()->GetType() == "CNTMacro"
						&& (this->wasUndo == FALSE && this->wasMove == FALSE) || this->isAllReplacing == TRUE) {
						history = sizeCommand->OpenAt();
						history->Add(command->Clone());
					}
					else {
						history = new CNTMacroCommand(this);
						history->Add(command->Clone());
						sizeCommand->Write(history);
						this->wasUndo = FALSE;
						this->wasMove = FALSE;
					}
				}
				else {
					sizeCommand->Write(command->Clone());
				}
				if (this->redoHistoryBook->GetLength() > 0) {
					this->redoHistoryBook->Empty();
				}
			}
			if (type == "CNTUndo") {
				this->wasUndo = TRUE;
			}
		}
		//========== ���� ��� �߰� ==========

		delete command;
	}

	if (this->scrollController != NULL) {
		delete this->scrollController;
	}
	this->scrollController = new ScrollController(this);

	this->Notify();
	this->Invalidate();

	Long x = this->characterMetrics->GetX(this->current) + 1; // 
	Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
	this->scrollController->SmartScrollToPoint(x, y);
}

void TextEditingForm::OnBasicCommandRange(UINT uID) {
	CNTCommandFactory commandFactory(this);
	CNTCommand* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();

		delete command;
	}
}

void TextEditingForm::OnMoveCommandRange(UINT uID) {
	CNTCommandFactory commandFactory(this);
	CNTCommand* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		this->wasMove = TRUE;

		delete command;
	}

	this->Notify();
	this->Invalidate();

	Long x = this->characterMetrics->GetX(this->current) + 1; // 
	Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
	this->scrollController->SmartScrollToPoint(x, y);
}

void TextEditingForm::OnFlagCommandRange(UINT uID) {
	CNTCommandFactory commandFactory(this);
	CNTCommand* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();

		if (command->GetType() == "CNTSize") {
			this->undoHistoryBook->Write(command->Clone());
		}

		delete command;
	}

	if (this->scrollController != NULL) {
		delete this->scrollController;
	}
	this->scrollController = new ScrollController(this);

	this->Notify();
	this->Invalidate();

	Long x = this->characterMetrics->GetX(this->current) + 1; // 
	Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
	this->scrollController->SmartScrollToPoint(x, y);
}

void TextEditingForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CNTKeyActionFactory keyActionFactory(this);
	CNTKeyAction* keyAction = keyActionFactory.Make(nChar);

	if (this->selection == NULL 
		&& (this->note->IsFirst() == true && nChar == VK_BACK)
		|| (this->note->IsLast() == true && nChar == VK_DELETE)) {
		delete keyAction;
		keyAction = 0;
	}

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		Long x = this->characterMetrics->GetX(this->current) + 1; // 
		Long y = this->characterMetrics->GetY(this->note->GetCurrent() + 1); // 0���̽��̹Ƿ� 1����
		this->scrollController->SmartScrollToPoint(x, y);
		delete keyAction;
	}
	this->Notify();
	this->Invalidate();
}

void TextEditingForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	HScrollActionFactory hScrollFactory(this);
	HScrollAction* hScrollAction = hScrollFactory.Make(nSBCode);
	if (hScrollAction != NULL) {
		hScrollAction->OnHScroll(nSBCode, nPos, pScrollBar);
		delete hScrollAction;
	}
	this->Invalidate();
}

void TextEditingForm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	VScrollActionFactory vScrollActionFactory(this);

	VScrollAction* vScrollAction = vScrollActionFactory.Make(nSBCode);
	if (vScrollAction != NULL) {
		vScrollAction->OnVScroll(nSBCode, nPos, pScrollBar);
		delete vScrollAction;
	}
	this->Invalidate();

}

BOOL TextEditingForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {

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

BOOL TextEditingForm::OnEraseBkgnd(CDC* pDC) {
	return TRUE;
}

LRESULT TextEditingForm::OnFindReplace(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);

	if (this->findReplaceDialog != NULL) {
		CString messageText;
		BOOL isFindSuccess;
		if (this->findReplaceDialog->FindNext()) {
			isFindSuccess = this->findReplaceDialog->Find();
			if (isFindSuccess == FALSE) {
				messageText.Format("\"%s\"��(��) ã�� �� �����ϴ�.", this->findReplaceDialog->GetFindString());
				MessageBox((LPCTSTR)messageText, "�޸���", MB_OK);
				this->findReplaceDialog->SetFocus();
			}
		}
		else if (this->findReplaceDialog->ReplaceCurrent()) {
			this->findReplaceDialog->Replace();
			isFindSuccess = this->findReplaceDialog->Find();
			if (isFindSuccess == FALSE) {
				messageText.Format("\"%s\"��(��) ã�� �� �����ϴ�.", this->findReplaceDialog->GetFindString());
				MessageBox((LPCTSTR)messageText, "�޸���", MB_OK);
				this->findReplaceDialog->SetFocus();
			}
		}
		else if (this->findReplaceDialog->ReplaceAll()) {
			this->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLHOME, 0));
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