#include "FileDialog.h"

#define IDC_COMBOBOX_ENCODE 1001
#define IDC_STATIC_ANSI 1011
#define IDC_STATIC_UTF16LE 1012
#define IDC_STATIC_UTF16BE 1013
#define IDC_STATIC_UTF8 1014
#define IDC_STATIC_UTF8BOM 1015

BEGIN_MESSAGE_MAP(FileDialog, CFileDialog)

END_MESSAGE_MAP()


FileDialog::FileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
	LPCTSTR lpszFilter, CWnd* pParentWnd, DWORD dwSize, BOOL bVistaStyle)
	: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags,
		lpszFilter, pParentWnd, dwSize, bVistaStyle) {

	this->customizeDlg = NULL;
	if (bOpenFileDialog == FALSE) {
		this->customizeDlg = this->GetIFileDialogCustomize();

		this->customizeDlg->StartVisualGroup(0, L"ÀÎÄÚµù(&E):");

		this->customizeDlg->AddComboBox(IDC_COMBOBOX_ENCODE);
		
		this->customizeDlg->AddControlItem(IDC_COMBOBOX_ENCODE, IDC_STATIC_ANSI, L"ANSI");
		//this->customizeDlg->AddControlItem(IDC_COMBOBOX_ENCODE, IDC_STATIC_UTF16LE, L"UTF-16 LE");
		//this->customizeDlg->AddControlItem(IDC_COMBOBOX_ENCODE, IDC_STATIC_UTF16BE, L"UTF-16 BE");
		//this->customizeDlg->AddControlItem(IDC_COMBOBOX_ENCODE, IDC_STATIC_UTF8, L"UTF-8");
		//this->customizeDlg->AddControlItem(IDC_COMBOBOX_ENCODE, IDC_STATIC_UTF8BOM, L"UTF-8 BOM");

		this->customizeDlg->SetSelectedControlItem(IDC_COMBOBOX_ENCODE, IDC_STATIC_ANSI);

		this->customizeDlg->EndVisualGroup();

		this->encodingType = "ANSI";
	}
}


#if 0
void FileDialog::OnOK() {
	DWORD selectedItemID;
	this->customizeDlg->GetSelectedControlItem(IDC_COMBOBOX_ENCODE, &selectedItemID);
	
	switch (selectedItemID) {
	case IDC_STATIC_ANSI:
		this->encodingType = "ANSI";
		break;
	case IDC_STATIC_UTF16LE:
		this->encodingType = "UTF-16 LE";
		break;
	case IDC_STATIC_UTF16BE:
		this->encodingType = "UTF-16 BE";
		break;
	case IDC_STATIC_UTF8:
		this->encodingType = "UTF-8";
		break;
	case IDC_STATIC_UTF8BOM:
		this->encodingType = "UTF-8 BOM";
		break;
	default:
		break;
	}
}
#endif