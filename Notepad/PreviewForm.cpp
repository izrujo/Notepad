#include "PreviewForm.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "Document.h"
#include "DrawingVisitor.h"
#include "resource.h"
#include "Font.h"
#include "CharacterMetrics.h"
#include "PrintingVisitor.h"
#include "LineDivider.h"
#include "Book.h"

#define SCREENDPI 45

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(IDT_BUTTON_PRINT, IDT_BUTTON_NEXT, OnCommandRange)
END_MESSAGE_MAP()

PreviewForm::PreviewForm(CWnd* parent, Glyph* note) {
	this->parent = parent;
	this->note = note;
	this->book = NULL;
}

int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CHWindowForm::OnCreate(lpCreateStruct);

	this->toolBar.CreateEx(this);
	this->toolBar.LoadToolBar(IDR_TOOLBAR1);

	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);

	DEVMODE* pDevMode = (DEVMODE*)GlobalLock(notepadForm->document->deviceMode);
	if (pDevMode) {
		this->printerDC.CreateDC("WINSPOOL", (LPCTSTR)pDevMode->dmDeviceName, NULL, pDevMode);
	}
	else {
		CPrintDialog printDialog(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION | PD_RETURNDEFAULT, this);
		printDialog.GetDefaults();
		HDC hdc = printDialog.GetPrinterDC();
		this->printerDC.Attach(hdc);
	}
	DWORD error = GetLastError();
	GlobalUnlock(notepadForm->document->deviceMode);

	Long dpi = this->printerDC.GetDeviceCaps(LOGPIXELSX);

	LOGFONT logFont = notepadForm->font->GetFont();
	logFont.lfHeight = logFont.lfHeight * dpi / 144;
	logFont.lfWidth = logFont.lfWidth * dpi / 144;
	this->font = new Font(logFont, notepadForm->font->GetColor(), this);

	this->characterMetrics = new CharacterMetrics(this, this->font);

	LineDivider lineDivider(this->characterMetrics);

	Long deviceWidth = this->printerDC.GetDeviceCaps(PHYSICALWIDTH);
	Long deviceHeight = this->printerDC.GetDeviceCaps(PHYSICALHEIGHT);
	CRect deviceMargin = notepadForm->document->GetMargins();
	float milimeterPerInch = 25.4F;
	deviceMargin.left = deviceMargin.left * (dpi / milimeterPerInch);
	deviceMargin.top = deviceMargin.top * (dpi / milimeterPerInch);
	deviceMargin.right = deviceMargin.right * (dpi / milimeterPerInch);
	deviceMargin.bottom = deviceMargin.bottom * (dpi / milimeterPerInch);
	Long top = deviceMargin.top;
	Long bottom = deviceMargin.bottom;

	string header = notepadForm->document->GetHeader();
	if (header != "") {
		top += this->characterMetrics->GetHeight();
	}
	string footer = notepadForm->document->GetFooter();
	if (footer != "") {
		bottom += this->characterMetrics->GetHeight();
	}

	this->book = new Book;
	Glyph* currentNote = this->note->Clone();
	Glyph* nextNote = lineDivider.Divide(deviceWidth - (deviceMargin.left + deviceMargin.right),
		deviceHeight - (top + bottom), currentNote, 0, 0); //좌표아니고 index임
	while (nextNote != NULL) {
		this->book->Add(currentNote);
		currentNote = nextNote;
		nextNote = lineDivider.Divide(deviceWidth - (deviceMargin.left + deviceMargin.right),
			deviceHeight - (top + bottom), currentNote, 0, 0); //좌표아니고 index임
	}
	this->book->Add(currentNote);
	this->book->Move(0);

	return 0;
}

void PreviewForm::OnClose() {
	if (this->note != NULL) {
		delete this->note;
	}

	if (this->printerDC != NULL) {
		this->printerDC.DeleteDC();
	}

	if (this->book != NULL) {
		delete this->book;
	}

	CFrameWnd::OnClose();
}

void PreviewForm::OnPaint() {
	CPaintDC dc(this);
	CRect rect;
	this->GetClientRect(rect);
	dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);

	Long deviceWidth = this->printerDC.GetDeviceCaps(PHYSICALWIDTH);
	Long deviceHeight = this->printerDC.GetDeviceCaps(PHYSICALHEIGHT);
	Long dpi = this->printerDC.GetDeviceCaps(LOGPIXELSX);

	//convert to dpi 45
	Long width = deviceWidth * SCREENDPI / dpi;
	Long height = deviceHeight * SCREENDPI / dpi;
	Long x = rect.Width() / 2 - width / 2;
	Long y = rect.Height() / 2 - height / 2;

	CRect deviceMargin = notepadForm->document->GetMargins();
	float milimeterPerInch = 25.4F;
	deviceMargin.left = deviceMargin.left * (SCREENDPI / milimeterPerInch);
	deviceMargin.top = deviceMargin.top * (SCREENDPI / milimeterPerInch);
	deviceMargin.right = deviceMargin.right * (SCREENDPI / milimeterPerInch);
	deviceMargin.bottom = deviceMargin.bottom * (SCREENDPI / milimeterPerInch);
	Long top = deviceMargin.top;
	Long bottom = deviceMargin.bottom;

	COLORREF color = this->font->GetColor();
	LOGFONT logFont = this->font->GetFont();
	logFont.lfHeight = logFont.lfHeight * SCREENDPI / dpi;
	logFont.lfWidth = logFont.lfWidth * SCREENDPI / dpi;
	Font font(logFont, color, this);

	CharacterMetrics characterMetrics(this, &font);

	string header = notepadForm->document->GetHeader();
	if (header != "") {
		top += characterMetrics.GetHeight();
	}
	string footer = notepadForm->document->GetFooter();
	if (footer != "") {
		bottom += characterMetrics.GetHeight();
	}

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, width, height);
	CBitmap* oldBitmap = memDC.SelectObject(&bitmap);
	RECT screenRect = { 0, 0, width, height }; // 그리고자 하는 영역의 크기
	memDC.FillSolidRect(&screenRect, RGB(255, 255, 255)); // 배경을 칠하다

	CFont* oldFont;
	COLORREF oldColor;
	CFont cFont;
	font.Create(cFont);
	oldFont = memDC.SelectObject(&cFont);
	oldColor = memDC.SetTextColor(font.GetColor());

	//==========머리말을 그리다.
	memDC.DrawText(header.c_str(), CRect(0, deviceMargin.top, width, top), DT_CENTER);
	//=========================

	//==========본문 내용을 그리다.
	Visitor* printingVisitor = new PrintingVisitor(&memDC, &characterMetrics, deviceMargin.left, top);

	this->book->GetAt(this->book->GetCurrent())->Accept(printingVisitor);
	//=========================

	//==========꼬리말을 그리다. //deviceMargin.bottom 에다가 그리기
	memDC.DrawText(footer.c_str(), CRect(0, height - bottom, width, height - deviceMargin.bottom), DT_CENTER);
	//=========================

	dc.BitBlt(x, y, width, height, &memDC, 0, 0, SRCCOPY);

	if (printingVisitor != NULL) {
		delete printingVisitor;
	}

	memDC.SelectObject(oldFont);
	memDC.SetTextColor(oldColor);

	memDC.SelectObject(oldBitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void PreviewForm::OnCommandRange(UINT uID) {
	if (uID == IDT_BUTTON_PRINT) {
		NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);
		// start a page
		if (this->printerDC.StartDocA(notepadForm->document->GetPathName().c_str()) < 0) {
			AfxMessageBox(_T("Printer wouldn't initialize"));
		}
		else {
			Long deviceWidth = this->printerDC.GetDeviceCaps(PHYSICALWIDTH);
			Long deviceHeight = this->printerDC.GetDeviceCaps(PHYSICALHEIGHT);
			Long dpi = this->printerDC.GetDeviceCaps(LOGPIXELSX);

			CRect deviceMargin = notepadForm->document->GetMargins();
			float milimeterPerInch = 25.4F;
			deviceMargin.left = deviceMargin.left * (dpi / milimeterPerInch);
			deviceMargin.top = deviceMargin.top * (dpi / milimeterPerInch);
			deviceMargin.right = deviceMargin.right * (dpi / milimeterPerInch);
			deviceMargin.bottom = deviceMargin.bottom * (dpi / milimeterPerInch);
			Long top = deviceMargin.top;
			Long bottom = deviceMargin.bottom;

			string header = notepadForm->document->GetHeader();
			if (header != "") {
				top += this->characterMetrics->GetHeight();
			}
			string footer = notepadForm->document->GetFooter();
			if (footer != "") {
				bottom += this->characterMetrics->GetHeight();
			}

			CDC memDC;
			memDC.CreateCompatibleDC(&this->printerDC);
			CBitmap bitmap;
			bitmap.CreateCompatibleBitmap(&this->printerDC, deviceWidth, deviceHeight);
			CBitmap* oldBitmap = memDC.SelectObject(&bitmap);
			RECT deviceRect = { 0, 0, deviceWidth, deviceHeight }; // 그리고자 하는 영역의 크기

			CFont* oldCFont;
			COLORREF oldColor;
			CFont cFont;
			this->font->Create(cFont);
			oldCFont = memDC.SelectObject(&cFont);
			oldColor = memDC.SetTextColor(font->GetColor());

			Long i = 0;
			int canStart = this->printerDC.StartPage();
			while (canStart > 0) {
				memDC.FillSolidRect(&deviceRect, RGB(255, 255, 255)); // 배경을 칠하다

				//==========머리말을 그리다.
				memDC.DrawText(header.c_str(), CRect(0, deviceMargin.top, deviceWidth, top), DT_CENTER);
				//=========================

				//==========본문 내용을 그리다.
				Visitor* printingVisitor = new PrintingVisitor(&memDC, this->characterMetrics, deviceMargin.left, top);

				this->book->GetAt(i++)->Accept(printingVisitor);
				//=========================

				//==========꼬리말을 그리다. //deviceMargin.bottom 에다가 그리기
				memDC.DrawText(footer.c_str(), CRect(0, deviceHeight - bottom, deviceWidth, deviceHeight - deviceMargin.bottom), DT_CENTER);
				//=========================

				this->printerDC.BitBlt(0, 0, deviceWidth, deviceHeight, &memDC, 0, 0, SRCCOPY);

				if (printingVisitor != NULL) {
					delete printingVisitor;
				}
				this->printerDC.EndPage();
				if (i >= this->book->GetLength()) {
					this->printerDC.EndDoc();
				}
				canStart = this->printerDC.StartPage();
			}

			memDC.SelectObject(oldCFont);
			memDC.SetTextColor(oldColor);

			memDC.SelectObject(oldBitmap);
			bitmap.DeleteObject();
			memDC.DeleteDC();
		}
		this->OnClose();
	}
	else if (uID == IDT_BUTTON_EXIT) {
		this->OnClose();
	}
	else if (uID == IDT_BUTTON_PREVIOUS) {
		this->book->Previous();
		this->Invalidate();
	}
	else if (uID == IDT_BUTTON_NEXT) {
		this->book->Next();
		this->Invalidate();
	}
}