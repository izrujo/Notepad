#include "Printer.h"
#include "NotepadForm.h"
#include "PrintInformation.h"
#include "Document.h"
#include "CharacterMetrics.h"
#include "Font.h"
#include <afxwin.h>
#include "PrintingVisitor.h"
#include "Glyph.h"

Printer::Printer(NotepadForm* notepadForm, PrintInformation *printInformation) {
	this->notepadForm = notepadForm;
	this->printInformation = printInformation;
}

Printer::Printer(const Printer& source) {
	this->notepadForm = source.notepadForm;
	this->printInformation = source.printInformation;
}

Printer::~Printer() {

}

Printer& Printer::operator=(const Printer& source) {
	this->notepadForm = source.notepadForm;
	this->printInformation = source.printInformation;

	return *this;
}

void Printer::Print() {
	Long deviceWidth = this->printInformation->printerDC.GetDeviceCaps(PHYSICALWIDTH);
	Long deviceHeight = this->printInformation->printerDC.GetDeviceCaps(PHYSICALHEIGHT);
	Long dpi = this->printInformation->printerDC.GetDeviceCaps(LOGPIXELSX);

	CRect deviceMargin = this->notepadForm->document->GetMargins();
	float milimeterPerInch = 25.4F;
	deviceMargin.left = deviceMargin.left * (dpi / milimeterPerInch);
	deviceMargin.top = deviceMargin.top * (dpi / milimeterPerInch);
	deviceMargin.right = deviceMargin.right * (dpi / milimeterPerInch);
	deviceMargin.bottom = deviceMargin.bottom * (dpi / milimeterPerInch);
	Long top = deviceMargin.top;
	Long bottom = deviceMargin.bottom;

	string header = this->notepadForm->document->GetHeader();
	if (header != "") {
		top += this->printInformation->characterMetrics->GetHeight();
	}
	string footer = this->notepadForm->document->GetFooter();
	if (footer != "") {
		bottom += this->printInformation->characterMetrics->GetHeight();
	}

	CDC memDC;
	memDC.CreateCompatibleDC(&this->printInformation->printerDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&this->printInformation->printerDC, deviceWidth, deviceHeight);
	CBitmap* oldBitmap = memDC.SelectObject(&bitmap);
	RECT deviceRect = { 0, 0, deviceWidth, deviceHeight }; // 그리고자 하는 영역의 크기

	CFont* oldCFont;
	COLORREF oldColor;
	CFont cFont;
	this->printInformation->font->Create(cFont);
	oldCFont = memDC.SelectObject(&cFont);
	oldColor = memDC.SetTextColor(this->printInformation->font->GetColor());

	Long i = 0;
	int canStart = this->printInformation->printerDC.StartPage();
	while (canStart > 0) {
		memDC.FillSolidRect(&deviceRect, RGB(255, 255, 255)); // 배경을 칠하다

		//==========머리말을 그리다.
		memDC.DrawText(header.c_str(), CRect(0, deviceMargin.top, deviceWidth, top), DT_CENTER);
		//=========================

		//==========본문 내용을 그리다.
		Visitor* printingVisitor = new PrintingVisitor(&memDC, this->printInformation->characterMetrics, deviceMargin.left, top);

		this->printInformation->book->GetAt(i++)->Accept(printingVisitor);
		//=========================

		//==========꼬리말을 그리다. //deviceMargin.bottom 에다가 그리기
		memDC.DrawText(footer.c_str(), CRect(0, deviceHeight - bottom, deviceWidth, deviceHeight - deviceMargin.bottom), DT_CENTER);
		//=========================

		this->printInformation->printerDC.BitBlt(0, 0, deviceWidth, deviceHeight, &memDC, 0, 0, SRCCOPY);
		DWORD error = GetLastError();

		if (printingVisitor != NULL) {
			delete printingVisitor;
		}
		this->printInformation->printerDC.EndPage();
		if (i >= this->printInformation->book->GetLength()) {
			this->printInformation->printerDC.EndDoc();
		}
		canStart = this->printInformation->printerDC.StartPage();
	}

	memDC.SelectObject(oldCFont);
	memDC.SetTextColor(oldColor);

	memDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}