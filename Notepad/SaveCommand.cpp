#include "SaveCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "Note.h"
#include <afxdlgs.h>

SaveCommand::SaveCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

SaveCommand::SaveCommand(const SaveCommand& source)
	: Command(source.notepadForm) {
}

SaveCommand::~SaveCommand() {
}

void SaveCommand::Execute() {
	if (this->notepadForm->fileName.Compare("제목 없음") == 0) {
		CFileDialog fileDialog(FALSE);
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();
			File file((LPCTSTR)pathName, ios_base::out);
			string content = this->notepadForm->note->GetContent();
			file.Save(content);
			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);
			this->notepadForm->fileName = pathName;
		}
	}
	else {
		File file((LPCTSTR)this->notepadForm->fileName, ios_base::out);
		string content = this->notepadForm->note->GetContent();
		file.Save(content);
	}
}

SaveCommand& SaveCommand::operator =(const SaveCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}