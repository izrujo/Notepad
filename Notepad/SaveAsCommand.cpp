#include "SaveAsCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "Note.h"
#include <afxdlgs.h>

SaveAsCommand::SaveAsCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

SaveAsCommand::SaveAsCommand(const SaveAsCommand& source)
	: Command(source.notepadForm) {
}

SaveAsCommand::~SaveAsCommand() {
}

void SaveAsCommand::Execute() {
	CFileDialog fileDialog(FALSE);
	int ret = fileDialog.DoModal();
	if (ret == IDOK) {
		CString pathName = fileDialog.GetPathName();
		File file((LPCTSTR)pathName, ios_base::out);
		string content = this->notepadForm->note->GetContent();
		file.Save(content);
		CString title = fileDialog.GetFileTitle();
		title.AppendFormat(" - ¸Þ¸ðÀå");
		this->notepadForm->SetWindowTextA((LPCTSTR)title);
		this->notepadForm->fileName = pathName;
	}
}
SaveAsCommand& SaveAsCommand::operator =(const SaveAsCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}