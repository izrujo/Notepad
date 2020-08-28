#include "PageSetupDialog.h"
#include "NotepadForm.h"
#include "Document.h"
#include "resource.h"

PageSetupDialog::PageSetupDialog(CWnd* parent) {
	this->psd = { 0, };

	this->psd.lStructSize = sizeof(PAGESETUPDLG);
	this->psd.hwndOwner = parent->GetSafeHwnd();
	this->psd.Flags = PSD_MARGINS | PSD_ENABLEPAGESETUPTEMPLATE | PSD_ENABLEPAGESETUPHOOK;
	this->psd.lpPageSetupTemplateName = MAKEINTRESOURCE(IDD);
	UINT(*pageSetupHookFunction)(HWND, UINT, WPARAM, LPARAM) = PageSetupHook;
	this->psd.lpfnPageSetupHook = (LPPAGESETUPHOOK)pageSetupHookFunction;
	this->psd.hInstance = AfxGetInstanceHandle();
	
	// Initialize margins
	this->psd.rtMargin.left = 2000;
	this->psd.rtMargin.top = 2500;
	this->psd.rtMargin.right = 2000;
	this->psd.rtMargin.bottom = 2500;
}

BOOL PageSetupDialog::DoModal() {
	return PageSetupDlg(&this->psd);
}

UINT APIENTRY PageSetupHook(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	if (uiMsg == WM_COMMAND && LOWORD(wParam) == IDOK) {
		NotepadForm* notepadForm = static_cast<NotepadForm*>(CWnd::FromHandle(hdlg)->GetParent());

		CSize sizes[10] = 
		{ CSize(29700, 42000), CSize(21000, 29700), CSize(14800, 21000), CSize(25700, 36400), CSize(18200, 25700),
		CSize(18410, 26670), CSize(21590, 35560), CSize(21590, 27940), CSize(13970, 21590), CSize(27940, 43180) };
		CSize paperSize;
		CString size;
		CWnd::FromHandle(GetDlgItem(hdlg, 1137))->GetWindowTextA(size);
		int index = ((CComboBox*)CWnd::FromHandle(GetDlgItem(hdlg, 1137)))->FindString(0, size);
		paperSize = sizes[index];
		notepadForm->document->SetPaperSize(paperSize);

		bool isVertical;
		int selectedRadio = CWnd::FromHandle(hdlg)->GetCheckedRadioButton(1056, 1057);
		if (selectedRadio == 1056) {
			isVertical = true;
		}
		else {
			isVertical = false;
		}
		notepadForm->document->SetIsVertical(isVertical);

		CRect margins;
		CString margin;
		CWnd::FromHandle(GetDlgItem(hdlg, 1155))->GetWindowTextA(margin);
		margins.left = atoi(margin);
		CWnd::FromHandle(GetDlgItem(hdlg, 1157))->GetWindowTextA(margin);
		margins.right = atoi(margin);
		CWnd::FromHandle(GetDlgItem(hdlg, 1156))->GetWindowTextA(margin);
		margins.top = atoi(margin);
		CWnd::FromHandle(GetDlgItem(hdlg, 1158))->GetWindowTextA(margin);
		margins.bottom = atoi(margin);
		notepadForm->document->SetMargins(margins);
		
		CString header;
		CString footer;
		CWnd::FromHandle(GetDlgItem(hdlg, IDC_EDIT_HEADER))->GetWindowTextA(header);
		CWnd::FromHandle(GetDlgItem(hdlg, IDC_EDIT_FOOTER))->GetWindowTextA(footer);
		notepadForm->document->SetHeader((LPCTSTR)header);
		notepadForm->document->SetFooter((LPCTSTR)footer);

	}


	return 0;
}

//
//INT_PTR PageSetupDialog::DoModal() {
//	INT_PTR ret = CPageSetupDialog::DoModal();
//
//	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->parent);
//
//	CString header;
//	CString footer;
//	GetDlgItem(IDC_EDIT_HEADER)->GetWindowTextA(header);
//	GetDlgItem(IDC_EDIT_FOOTER)->GetWindowTextA(footer);
//	notepadForm->document->SetHeader((LPCTSTR)header);
//	notepadForm->document->SetFooter((LPCTSTR)footer);
//	
//	return ret;
//}