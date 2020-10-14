#include "PrintStateDialog.h"
#include "NotepadForm.h"
#include "Document.h"
#include "PrintJobManager.h"
#include <winspool.h>

PrintStateDialog::PrintStateDialog(CWnd* parent)
	: CDialog(PrintStateDialog::IDD, parent) {
	this->Create(IDD, parent);
}

BOOL PrintStateDialog::OnInitDialog() {
	CDialog::OnInitDialog();

	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->GetParent());
	string fileName = notepadForm->document->GetPathName();
	//���� �̸��� ���ܵα�
	int index = fileName.rfind('\\');
	index++;
	fileName = fileName.erase(0, index);

	GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(fileName.c_str());

	return FALSE;
}

void PrintStateDialog::OnCancel() {
	NotepadForm* notepadForm = static_cast<NotepadForm*>(this->GetParent());

	//���⼭ ���� �������� �����尡 ���� ������ �ν���.
	//SetJob ���Ŀ� ������ ���������ӿ��� �ұ��ϰ� �������� �ν��� �� ����.
	notepadForm->printJobManager->End();

	HANDLE hPrinter; //Printer handle variable
	DWORD dwNeeded, dwReturned; //Mem needed, jobs found, variable for loop
	JOB_INFO_1* pJobInfo; // pointer to structure

	//Find printer handle
	OpenPrinter((LPSTR)notepadForm->document->deviceMode->dmDeviceName, &hPrinter, NULL);

	//Get amount of memory needed
	if (!EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, NULL, 0, &dwNeeded, &dwReturned)) {
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			ClosePrinter(hPrinter);
		}
	}
	//Allocate the memory, if you cant end function
	if ((pJobInfo = (JOB_INFO_1*)malloc(dwNeeded)) == NULL) {
		ClosePrinter(hPrinter);
	}
	//Get job info structure
	if (!EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, (LPBYTE)pJobInfo, dwNeeded, &dwNeeded, &dwReturned)) {
		ClosePrinter(hPrinter);
		free(pJobInfo);
	}
	//If there are printjobs, get document name and data type. put into docinfo1 struc and return true
	if (dwReturned > 0) {
		SetJob(hPrinter, pJobInfo[dwReturned - 1].JobId, 0, NULL, JOB_CONTROL_DELETE);
		ClosePrinter(hPrinter);
		free(pJobInfo);
	}

	AfxMessageBox(_T("����ڰ� �۾��� ����߽��ϴ�."));
	this->DestroyWindow();
	notepadForm->printStateDialog = NULL;
}