#include "PrintStateDialog.h"
#include "NotepadForm.h"
#include "Document.h"
#include "Printer.h"

PrintStateDialog::PrintStateDialog(CWnd* parent)
	: CDialog(PrintStateDialog::IDD, parent) {
	this->Create(IDD, parent);
}

BOOL PrintStateDialog::OnInitDialog() {
	CDialog::OnInitDialog();
	
	NotepadForm *notepadForm = static_cast<NotepadForm*>(this->GetParent());
	string fileName = notepadForm->document->GetPathName();
	//���� �̸��� ���ܵα�

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(fileName.c_str());

	return FALSE;
}

void PrintStateDialog::OnCancel() {
	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->GetParent());
	notepadForm->printer->End();
	int ret = AfxMessageBox(_T("����ڰ� �۾��� ����߽��ϴ�."));
	if (ret == IDOK) {
		this->DestroyWindow();
	}
}