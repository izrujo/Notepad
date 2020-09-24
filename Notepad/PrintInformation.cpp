#include "PrintInformation.h"
#include "NotepadForm.h"
#include "Document.h"
#include "CharacterMetrics.h"
#include "Font.h"
#include "LineDivider.h"
#include "Glyph.h"
#include "Book.h"
#include <afxdlgs.h>


PrintInformation::PrintInformation(NotepadForm* notepadForm) {
	DEVMODE* pDevMode = (DEVMODE*)GlobalLock(notepadForm->document->deviceMode);
	if (pDevMode) {
		this->printerDC.CreateDC("WINSPOOL", (LPCTSTR)pDevMode->dmDeviceName, NULL, pDevMode);
	}
	else {
		CPrintDialog printDialog(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION | PD_RETURNDEFAULT, notepadForm);
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
	this->font = new Font(logFont, notepadForm->font->GetColor(), notepadForm);

	this->characterMetrics = new CharacterMetrics(notepadForm, this->font);

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
	Glyph* currentNote = notepadForm->note->Clone();
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
}

PrintInformation::PrintInformation(const PrintInformation& source) {

}

PrintInformation::~PrintInformation() {

}

PrintInformation& PrintInformation::operator=(const PrintInformation& source) {
	return *this;
}