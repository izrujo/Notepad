#include "PreviewForm.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "Document.h"
#include "DrawingVisitor.h"
#include "resource.h"
#include "Font.h"
#include "CharacterMetrics.h"
#include "PrintingVisitor.h"
#include "Book.h"
#include "PrintInformation.h"
#include "DummyManager.h"

#define SCREENDPI 45

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(IDT_BUTTON_PRINT, IDT_BUTTON_NEXT, OnCommandRange)
END_MESSAGE_MAP()

PreviewForm::PreviewForm(CWnd* parent) {
	this->parent = parent;
	this->printInformation = NULL;
}

int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	this->toolBar.CreateEx(this);
	this->toolBar.LoadToolBar(IDR_TOOLBAR1);

	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);

	Glyph* note = notepadForm->note->Clone();
	if (notepadForm->GetIsAutoNewLining() == TRUE) {
		CRect rect;
		notepadForm->GetClientRect(rect);
		DummyManager dummyManager(note, notepadForm->characterMetrics, rect.Width());
		Long i = 0;
		while (i < note->GetLength()) {
			dummyManager.Unfold(i);
			i++;
		}
	}

	this->printInformation = new PrintInformation(notepadForm, note);

	return 0;
}

void PreviewForm::OnClose() {
	if (this->printInformation != NULL) {
		delete this->printInformation;
	}

	CFrameWnd::OnClose();
}

void PreviewForm::OnPaint() {
	CPaintDC dc(this);
	CRect rect;
	this->GetClientRect(rect);
	dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);

	Long deviceWidth = this->printInformation->printerDC.GetDeviceCaps(PHYSICALWIDTH);
	Long deviceHeight = this->printInformation->printerDC.GetDeviceCaps(PHYSICALHEIGHT);
	Long dpi = this->printInformation->printerDC.GetDeviceCaps(LOGPIXELSX);

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

	COLORREF color = this->printInformation->font->GetColor();
	LOGFONT logFont = this->printInformation->font->GetFont();
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

	this->printInformation->book->GetAt(this->printInformation->book->GetCurrent())->Accept(printingVisitor);
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
		notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_FILE_PRINT, 0));
		this->OnClose();
	}
	else if (uID == IDT_BUTTON_EXIT) {
		this->OnClose();
	}
	else if (uID == IDT_BUTTON_PREVIOUS) {
		CRect rect;
		this->GetClientRect(rect);
		CRect toolBarRect;
		this->toolBar.GetClientRect(toolBarRect);
		rect.top += toolBarRect.bottom;
		Long current = this->printInformation->book->GetCurrent();
		Long index = this->printInformation->book->Previous();
		if (index != current) {
			this->InvalidateRect(rect);
		}
	}
	else if (uID == IDT_BUTTON_NEXT) {
		CRect rect;
		this->GetClientRect(rect);
		CRect toolBarRect;
		this->toolBar.GetClientRect(toolBarRect);
		rect.top += toolBarRect.bottom;
		Long current = this->printInformation->book->GetCurrent();
		Long index = this->printInformation->book->Next();
		if (index != current) {
			this->InvalidateRect(rect);
		}
	}
}