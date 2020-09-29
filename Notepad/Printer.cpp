#include "Printer.h"
#include "NotepadForm.h"
#include "PrintInformation.h"
#include "Document.h"
#include "CharacterMetrics.h"
#include "Font.h"
#include <afxwin.h>
#include "PrintingVisitor.h"
#include "Glyph.h"
#include "PrintStateDialog.h"

Printer::Printer(NotepadForm* notepadForm) {
	this->notepadForm = notepadForm;
	this->thread = NULL;
	this->printInformation = NULL;
	this->printStateDialog = NULL;
	this->isPrinting = FALSE;
	this->isPausing = FALSE;
}

Printer::Printer(const Printer& source) {
	this->notepadForm = source.notepadForm;
	this->thread = source.thread;
	this->printInformation = source.printInformation;
	this->printStateDialog = source.printStateDialog;
	this->isPrinting = source.isPrinting;
	this->isPausing = source.isPausing;
}

Printer::~Printer() {
	this->isPrinting = FALSE;
	if (this->thread != NULL) {
		//DWORD ret;
		//::GetExitCodeThread(this->thread->m_hThread, &ret);
		if (this->isPausing == TRUE) {
			this->thread->ResumeThread();
		}
		::WaitForSingleObject(this->thread->m_hThread, INFINITE);
	}
}

Printer& Printer::operator=(const Printer& source) {
	this->notepadForm = source.notepadForm;
	this->thread = source.thread;
	this->printInformation = source.printInformation;
	this->printStateDialog = source.printStateDialog;
	this->isPrinting = source.isPrinting;
	this->isPausing = source.isPausing;

	return *this;
}

void Printer::Print(PrintInformation *printInformation, PrintStateDialog* printStateDialog) {
	this->isPrinting = TRUE;
	this->printInformation = printInformation;
	this->printStateDialog = printStateDialog;

	this->thread = AfxBeginThread(PrintThread, (LPVOID)this);
}

void Printer::Pause() {
	this->isPrinting = FALSE;
	this->isPausing = TRUE;
	this->thread->SuspendThread();
}

void Printer::Resume() {
	this->isPrinting = TRUE;
	this->isPausing = FALSE;
	this->thread->ResumeThread();
}

void Printer::End() {
	this->isPrinting = FALSE;
	if (this->thread != NULL) {
		if (this->isPausing == TRUE) {
			this->thread->ResumeThread();
		}
		::WaitForSingleObject(this->thread->m_hThread, INFINITE);
	
	}
	this->isPausing = FALSE;
}

UINT Printer::PrintThread(LPVOID pParam) {
	Printer* printer = (Printer*)pParam;

	Long deviceWidth = printer->printInformation->printerDC.GetDeviceCaps(PHYSICALWIDTH);
	Long deviceHeight = printer->printInformation->printerDC.GetDeviceCaps(PHYSICALHEIGHT);
	Long dpi = printer->printInformation->printerDC.GetDeviceCaps(LOGPIXELSX);

	CRect deviceMargin = printer->notepadForm->document->GetMargins();
	float milimeterPerInch = 25.4F;
	deviceMargin.left = deviceMargin.left * (dpi / milimeterPerInch);
	deviceMargin.top = deviceMargin.top * (dpi / milimeterPerInch);
	deviceMargin.right = deviceMargin.right * (dpi / milimeterPerInch);
	deviceMargin.bottom = deviceMargin.bottom * (dpi / milimeterPerInch);
	Long top = deviceMargin.top;
	Long bottom = deviceMargin.bottom;

	string header = printer->notepadForm->document->GetHeader();
	if (header != "") {
		top += printer->printInformation->characterMetrics->GetHeight();
	}
	string footer = printer->notepadForm->document->GetFooter();
	if (footer != "") {
		bottom += printer->printInformation->characterMetrics->GetHeight();
	}

	CDC memDC;
	memDC.CreateCompatibleDC(&printer->printInformation->printerDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&printer->printInformation->printerDC, deviceWidth, deviceHeight);
	CBitmap* oldBitmap = memDC.SelectObject(&bitmap);
	RECT deviceRect = { 0, 0, deviceWidth, deviceHeight }; // 그리고자 하는 영역의 크기

	CFont* oldCFont;
	COLORREF oldColor;
	CFont cFont;
	printer->printInformation->font->Create(cFont);
	oldCFont = memDC.SelectObject(&cFont);
	oldColor = memDC.SetTextColor(printer->printInformation->font->GetColor());

	Long i = 0;
	int canStart = printer->printInformation->printerDC.StartPage();
	while (canStart > 0 && printer->isPrinting == TRUE) {
		memDC.FillSolidRect(&deviceRect, RGB(255, 255, 255)); // 배경을 칠하다

		//==========머리말을 그리다.
		memDC.DrawText(header.c_str(), CRect(0, deviceMargin.top, deviceWidth, top), DT_CENTER);
		//=========================

		//==========본문 내용을 그리다.
		Visitor* printingVisitor = new PrintingVisitor(&memDC, printer->printInformation->characterMetrics, deviceMargin.left, top);

		printer->printInformation->book->GetAt(i++)->Accept(printingVisitor);
		//=========================

		//==========꼬리말을 그리다. //deviceMargin.bottom 에다가 그리기
		memDC.DrawText(footer.c_str(), CRect(0, deviceHeight - bottom, deviceWidth, deviceHeight - deviceMargin.bottom), DT_CENTER);
		//=========================

		printer->printInformation->printerDC.BitBlt(0, 0, deviceWidth, deviceHeight, &memDC, 0, 0, SRCCOPY);
		DWORD error = GetLastError();

		if (printingVisitor != NULL) {
			delete printingVisitor;
		}
		printer->printInformation->printerDC.EndPage();
		if (i >= printer->printInformation->book->GetLength()) {
			printer->printInformation->printerDC.EndDoc();
		}
		canStart = printer->printInformation->printerDC.StartPage();
	}

	memDC.SelectObject(oldCFont);
	memDC.SetTextColor(oldColor);

	memDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();


	printer->isPrinting = FALSE;

	if (printer->printInformation != NULL) {
		delete printer->printInformation;
		printer->printInformation = NULL;
	}
	//인쇄를 정상적으로 완료했을 때 인쇄 중 대화 상자 꺼야 하는데 아래 코드 넣으면 오류 발생함.
	/*if (printer->printStateDialog != NULL) {
		printer->printStateDialog->DestroyWindow();
		printer->printStateDialog = NULL;
	}*/

	return 0;
}