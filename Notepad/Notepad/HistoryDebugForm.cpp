#include "HistoryDebugForm.h"
#include "NotepadForm.h"
#include "../TextEditor/HistoryBook.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/CNTCommands.h"


BEGIN_MESSAGE_MAP(HistoryDebugForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

HistoryDebugForm::HistoryDebugForm(CWnd* parent) {
	this->parent = parent;
	this->undo = NULL;
	this->redo = NULL;
	this->undos = new Array<CString>;
	this->undosCapacity = this->undos->GetCapacity();
	this->undosLength = this->undos->GetLength();
	this->redos = new Array<CString>;
	this->redosCapacity = this->redos->GetCapacity();
	this->redosLength = this->redos->GetLength();
}

int HistoryDebugForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	return 0;
}

void HistoryDebugForm::OnClose() {
	if (this->undo != NULL) {
		delete this->undo;
	}
	if (this->redo != NULL) {
		delete this->redo;
	}
	if (this->undos != NULL) {
		delete this->undos;
	}
	if (this->redos != NULL) {
		delete this->redos;
	}

	CFrameWnd::OnClose();
}

void HistoryDebugForm::OnPaint() {
	CPaintDC dc(this);
	CRect rect;
	this->GetClientRect(rect);
	dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));


	CString number;
	Long x=0;
	Long y=0;
	if (this->undo != NULL && this->redo != NULL) {
		x = rect.left + 100;
		y = rect.bottom - 100;
		Long j = 0;
		Long i = this->undos->GetLength();
		while (i > 0) {
			number.Format("%d", ++j);
			dc.TextOutA(x - 20, y, number);
			dc.TextOutA(x, y, this->undos->GetAt(i - 1));
			y -= 20;
			i--;
		}

		x = x + 200;
		y = rect.bottom - 100;
		j = 0;
		i = this->redos->GetLength();
		while (i > 0) {
			number.Format("%d", ++j);
			dc.TextOutA(x - 20, y, number);
			dc.TextOutA(x, y, this->redos->GetAt(i-1));
			y -= 20;
			i--;
		}
	}
	else {
		dc.TextOutA(x, y, "Press F5 to refresh");
	}
}

void HistoryDebugForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	//f5 새로고침
	if (nChar == VK_F5) {
		NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);

		TextEditingForm* textEditingForm = notepadForm->textEditingForm;

		if (this->undo != NULL) {
			delete this->undo;
		}
		this->undo = new HistoryBook(*textEditingForm->undoHistoryBook);
		if (this->undos->GetLength() > 0) {
			this->undos->Clear();
			this->undos->Ready();
		}
		
		Long i = 0;
		CNTCommand* command;
		while (this->undo->GetLength() > 0) {
			command = this->undo->OpenAt();
			this->undos->Store(i++, command->GetType().c_str());
			this->undosLength++;
			this->undo->Erase();
		}
		
		if (this->redo != NULL) {
			delete this->redo;
		}
		this->redo = new HistoryBook(*textEditingForm->redoHistoryBook);
		if (this->redos->GetLength() > 0) {
			this->redos->Clear();
			this->redos->Ready();
		}

		i = 0;
		while (this->redo->GetLength() > 0) {
			command = this->redo->OpenAt();
			this->redos->Store(i++, command->GetType().c_str());
			this->redosLength++;
			this->redo->Erase();
		}
		this->Invalidate();
	}
}