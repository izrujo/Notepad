#include "NotepadApp.h"
#include "NotepadForm.h"

BOOL NotepadApp::InitInstance() {
	NotepadForm *notepadForm = new NotepadForm;
	notepadForm->Create(NULL, "���� ���� - �޸���");
	notepadForm->ShowWindow(SW_MAXIMIZE);
	notepadForm->UpdateWindow();
	this->m_pMainWnd = notepadForm;

	return TRUE;
}
NotepadApp notepadApp;