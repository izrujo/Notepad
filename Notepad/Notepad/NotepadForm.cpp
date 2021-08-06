#include "NotepadForm.h"
#include "CommandFactory.h"
#include "Commands.h"
#include "Document.h"
#include "PrintJobManager.h"
#include "PrintStateDialog.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/ScrollController.h"
#include "../TextEditor/CNTKeyActionFactory.h"
#include "../TextEditor/CNTKeyActions.h"

#include "resource.h"
#include "../TextEditor/resource.h"

#include <WinUser.h>
#include <imm.h>
#include <afxdlgs.h>

BEGIN_MESSAGE_MAP(NotepadForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND_RANGE(IDM_FILE_NEW, IDM_FORMAT_AUTONEWLINE, OnCommandRange)
	ON_COMMAND_RANGE(IDM_FILE_CLOSE, IDC_REPORT_DIRTY, OnSimpleCommandRange)
	ON_WM_KEYDOWN()
	ON_MESSAGE(WM_THREADNOTIFY, OnThreadNotify)
	//ON_UPDATE_COMMAND_UI_RANGE(IDM_FORMAT_WORDWRAP, IDM_FORMAT_WORDWRAP, OnUpdateCommandUIRange)
END_MESSAGE_MAP()

NotepadForm::NotepadForm() {
	this->textEditingForm = NULL;
	this->document = NULL;
	this->printJobManager = NULL;
	this->printStateDialog = NULL;
}

int NotepadForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	CRect rect;
	this->GetClientRect(rect);

	this->textEditingForm = new TextEditingForm;
	this->textEditingForm->Create(NULL, "CNTextEditor", WS_CHILD,
		rect, this, NULL, NULL);
	this->textEditingForm->ShowWindow(SW_SHOW);
	this->textEditingForm->UpdateWindow();
	this->textEditingForm->SetFocus();
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_UNLOCKHISTORYBOOK, 0));
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_UNLOCKFINDREPLACEDIALOG, 0));

	this->menu.LoadMenuA(IDR_MENU1);
	this->SetMenu(&menu);

	this->document = new Document(this);

	this->printJobManager = new PrintJobManager(this);

	return 0;
}

void NotepadForm::OnClose() {
	if (this->textEditingForm != NULL) {
		this->textEditingForm->SendMessage(WM_CLOSE);
		this->textEditingForm = NULL;
	}
	if (this->document != NULL) {
		delete this->document;
	}
	if (this->printJobManager != NULL) {
		delete this->printJobManager;
	}
	if (this->printStateDialog != NULL) {
		delete this->printStateDialog;
	}

	CFrameWnd::OnClose();
}

void NotepadForm::OnSize(UINT nType, int cx, int cy) {
	CRect rect;
	this->GetClientRect(rect);

	if (this->textEditingForm != NULL) {
		this->textEditingForm->MoveWindow(rect);
	}

	CFrameWnd::OnSize(nType, cx, cy);
}

void NotepadForm::OnSetFocus(CWnd* pOldWnd) {
	CFrameWnd::OnSetFocus(pOldWnd);
	this->textEditingForm->SetFocus();
}

void NotepadForm::OnKillFocus(CWnd* pNewWnd) {
	CFrameWnd::OnKillFocus(pNewWnd);
}

void NotepadForm::OnCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		delete command;
	}

	if (this->textEditingForm->scrollController != NULL) {
		delete this->textEditingForm->scrollController;
	}
	this->textEditingForm->scrollController = new ScrollController(this->textEditingForm);

	this->textEditingForm->Notify();
	this->textEditingForm->Invalidate();

	Long x = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current) + 1; // 
	Long y = this->textEditingForm->characterMetrics->GetY(this->textEditingForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->textEditingForm->scrollController->SmartScrollToPoint(x, y);
}

void NotepadForm::OnSimpleCommandRange(UINT uID) {
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		delete command;
	}
}

void NotepadForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CNTKeyActionFactory keyActionFactory(this->textEditingForm);
	CNTKeyAction* keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);

		delete keyAction;
	}
	this->textEditingForm->Notify();
	this->textEditingForm->Invalidate();
}

LRESULT NotepadForm::OnThreadNotify(WPARAM wParam, LPARAM lParam) {
	if (this->printStateDialog != NULL) {
		this->printStateDialog->DestroyWindow();
		this->printStateDialog = NULL;
	}

	return 0;
}