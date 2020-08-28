#include "PreviewForm.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "Document.h"
#include "DrawingVisitor.h"
#include "resource.h"

#define MAXPIXELHEIGHT 700
#define MAXMILIMETERHEIGHT 43180

BEGIN_MESSAGE_MAP(PreviewForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(IDT_BUTTON_PRINT, IDT_BUTTON_EXIT, OnCommandRange)
END_MESSAGE_MAP()

PreviewForm::PreviewForm(CWnd* parent, Glyph* note) {
	this->parent = parent;
	this->note = note;
}

int PreviewForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	this->toolBar.CreateEx(this);
	this->toolBar.LoadToolBar(IDR_TOOLBAR1);

	return 0;
}

void PreviewForm::OnClose() {
	if (this->note != NULL) {
		delete this->note;
	}

	CFrameWnd::OnClose();
}

void PreviewForm::OnPaint() {
	CPaintDC dc(this);
	CRect rect;
	this->GetClientRect(rect);
	dc.FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);

	//Draw Paper
	CRect toolBarRect;
	this->toolBar.GetClientRect(toolBarRect);
	rect.top += toolBarRect.bottom;
	CSize paperSize = notepadForm->document->GetPaperSize();
	CRect paper;
	Long width = round(paperSize.cx * MAXPIXELHEIGHT / MAXMILIMETERHEIGHT);
	Long height = round(paperSize.cy * MAXPIXELHEIGHT / MAXMILIMETERHEIGHT);
	if (notepadForm->document->GetIsVertical() == false) {
		Long temp = width;
		width = height;
		height = temp;
	}
	paper.left = rect.left + (rect.Width() - width) / 2;
	paper.right = paper.left + width;
	paper.top = rect.top + (rect.Height() - height) / 2;
	paper.bottom = paper.top + height;
	dc.Rectangle(paper);

	//Draw Texts
	CRect milimeterMargins = notepadForm->document->GetMargins();
	CRect pixelMargins;
	pixelMargins.left = round(milimeterMargins.left * MAXPIXELHEIGHT / MAXMILIMETERHEIGHT);
	pixelMargins.top = round(milimeterMargins.top * MAXPIXELHEIGHT / MAXMILIMETERHEIGHT);
	pixelMargins.right = round(milimeterMargins.right * MAXPIXELHEIGHT / MAXMILIMETERHEIGHT);
	pixelMargins.bottom = round(milimeterMargins.bottom * MAXPIXELHEIGHT / MAXMILIMETERHEIGHT);
	CRect textRange;
	textRange.left = paper.left + pixelMargins.left;
	textRange.top = paper.top + pixelMargins.top;
	textRange.right = paper.right - pixelMargins.right;
	textRange.bottom = paper.bottom - pixelMargins.bottom;
	//dc.Rectangle(textRange); //margins test
	//=======set x, y for dc and calculate font size for preview
	dc.SetWindowExt(textRange.left, textRange.top);
	Visitor* drawingVisitor = new DrawingVisitor(&dc, notepadForm->characterMetrics);
	this->note->Accept(drawingVisitor);
}

void PreviewForm::OnCommandRange(UINT uID) {
	if (uID == IDT_BUTTON_PRINT) {
		//CPrintDialog printDialog(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION,
		//	this);

		//int ret = printDialog.DoModal();
		//if (ret == IDOK) {
		//	HDC hdcPrinter = printDialog.GetPrinterDC();
		//	if (hdcPrinter == NULL) {
		//		MessageBox(_T("Buy a printer!"));
		//	}
		//	else {
		//		CDC printerDC;
		//		printerDC.Attach(hdcPrinter);
		//		// if it fails, complain and exit gracefully
		//		if (printerDC.StartDocA(this->editor->fileOpenPath) < 0) {
		//			AfxMessageBox(_T("Printer wouldn't initialize"));
		//		}
		//		else {
		//			// start a page
		//			if (printerDC.StartPage() < 0) {
		//				AfxMessageBox(_T("Could not start page"));
		//				printerDC.AbortDoc();
		//			}
		//			else {
		//				// actually do some printing
		//				Painter printPainter(&printerDC, this->a4Paper->GetWidth(), this->a4Paper->GetHeight(), RGB(255, 255, 255), TRANSPARENT);
		//				//폰트 설정
		//				LOGFONT logFont = this->editor->font->GetFont();
		//				logFont.lfHeight *= 3;
		//				COLORREF color = this->editor->font->GetColor();
		//				this->font = new FlowChartFont(this->editor, logFont, color);
		//				HFONT hFont = this->font->Create();
		//				printPainter.ChangeFont(hFont, this->font->GetColor());

		//				Shape* cloneFlowChart = this->flowChart->Clone();
		//				Shape* shape;
		//				Long i = 0;
		//				while (i < dynamic_cast<FlowChart*>(cloneFlowChart)->GetLength()) {
		//					shape = dynamic_cast<FlowChart*>(cloneFlowChart)->GetAt(i);
		//					//가로는 6mm, 세로는 5mm 빼주기.
		//					shape->Move(shape->GetX() - this->a4Paper->GetX() - 142, shape->GetY() - this->a4Paper->GetY() - 118);
		//					i++;
		//				}

		//				FlowChartVisitor* drawVisitor = new DrawVisitor(&printPainter);
		//				cloneFlowChart->Accept(drawVisitor);
		//				printPainter.Render(&printerDC, 0, 0, this->a4Paper->GetWidth(), this->a4Paper->GetHeight());
		//				if (drawVisitor != NULL) {
		//					delete drawVisitor;
		//				}

		//				int result = printerDC.EndPage();
		//				result = printerDC.EndDoc();
		//			}
		//		}
		//	}
		//	CDC::FromHandle(hdcPrinter)->DeleteDC();
		//}
	}
	else if (uID == IDT_BUTTON_EXIT) {
		this->OnClose();
	}
}