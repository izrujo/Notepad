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
	
	NotepadForm* notepadForm = static_cast<NotepadForm*>(parent);

	this->psd.hDevMode = GlobalAlloc(GHND, sizeof(DEVMODE)); // �̵� ������ �޸� ����� �Ҵ��ϰ� 0���� �ʱ�ȭ�մϴ�.
	if (this->psd.hDevMode) {
		DEVMODE* pDevMode = (DEVMODE*)GlobalLock(this->psd.hDevMode); // �޸𸮸� ��װ� �����͸� ��ȯ
		if (pDevMode) {
			pDevMode->dmSize = sizeof(DEVMODE); // DEVMODE ������ ũ�⸦ �����մϴ�.
			pDevMode->dmFields = DM_ORIENTATION; // dmOrientation �ʵ带 ������ ������ Windows���˸��ϴ�.
			(notepadForm->document->GetIsVertical()) ?
				(pDevMode->dmOrientation = DMORIENT_PORTRAIT) : (pDevMode->dmOrientation = DMORIENT_LANDSCAPE);
		}
		GlobalUnlock(this->psd.hDevMode); // �ٸ� �Լ��� �̰��� ����� �� �ֵ��� �޸� ����� �����մϴ�.
	}

	this->psd.rtMargin.left = notepadForm->document->GetMargins().left * 100;
	this->psd.rtMargin.top = notepadForm->document->GetMargins().top * 100;
	this->psd.rtMargin.right = notepadForm->document->GetMargins().right * 100;
	this->psd.rtMargin.bottom = notepadForm->document->GetMargins().bottom * 100;
}

BOOL PageSetupDialog::DoModal() {
	return PageSetupDlg(&this->psd);
}

UINT APIENTRY PageSetupHook(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	NotepadForm* notepadForm = static_cast<NotepadForm*>(CWnd::FromHandle(hdlg)->GetParent());

	/*CSize sizes[10] =
	{ CSize(29700, 42000), CSize(21000, 29700), CSize(14800, 21000), CSize(25700, 36400), CSize(18200, 25700),
	CSize(18410, 26670), CSize(21590, 35560), CSize(21590, 27940), CSize(13970, 21590), CSize(27940, 43180) };*/

	if (uiMsg == WM_INITDIALOG) {
		string header = notepadForm->document->GetHeader();
		string footer = notepadForm->document->GetFooter();
		CWnd::FromHandle(GetDlgItem(hdlg, IDC_EDIT_HEADER))->SetWindowTextA(header.c_str());
		CWnd::FromHandle(GetDlgItem(hdlg, IDC_EDIT_FOOTER))->SetWindowTextA(footer.c_str());
	
		//10.05 ���ڱ� �� ����� �ڵ����� �ǳ׿�?
		//CSize paperSize = notepadForm->document->GetPaperSize();
		//Long i = 0;
		//while (i < 10 && sizes[i] != paperSize) {			
		//	i++;
		//}
		//if (i < 10) {
		////	((CComboBox*)CWnd::FromHandle(GetDlgItem(hdlg, 1137)))->SetCurSel(i);
		//}
	}
	else if (uiMsg == WM_COMMAND && LOWORD(wParam) == IDOK) {
		/*CSize paperSize;
		CString size;
		CWnd::FromHandle(GetDlgItem(hdlg, 1137))->GetWindowTextA(size);
		int index = ((CComboBox*)CWnd::FromHandle(GetDlgItem(hdlg, 1137)))->FindString(0, size);
		paperSize = sizes[index];
		notepadForm->document->SetPaperSize(paperSize);*/

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