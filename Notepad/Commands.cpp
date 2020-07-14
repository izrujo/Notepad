#include "Commands.h"
#include "NotepadForm.h"
#include "Font.h"
#include "CharacterMetrics.h"
#include "FileManager.h"
#include "Document.h"

#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>

//Command
Command::Command(NotepadForm* notepadForm) {
	this->notepadForm = notepadForm;
}

Command::Command(const Command& source) {
	this->notepadForm = source.notepadForm;
}

Command::~Command() {

}

Command& Command::operator=(const Command& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

//FontCommand
FontCommand::FontCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

FontCommand::FontCommand(const FontCommand& source)
	: Command(source) {

}

FontCommand::~FontCommand() {

}

FontCommand& FontCommand::operator=(const FontCommand& source) {
	Command::operator=(source);

	return *this;
}

void FontCommand::Execute() {
	LOGFONT logFont = this->notepadForm->font->GetFont();
	COLORREF color;
	CFontDialog fontDialog(&logFont);
	int ret = fontDialog.DoModal();
	if (ret == IDOK) {
		//폰트 대화 상자에서 폰트 정보를 가져온다.
		fontDialog.GetCurrentFont(&logFont);
		color = fontDialog.GetColor();
		if (this->notepadForm->font != NULL) {
			delete this->notepadForm->font;
		}
		this->notepadForm->font = new Font(logFont, color, this->notepadForm);
		if (this->notepadForm->characterMetrics != NULL) {
			delete this->notepadForm->characterMetrics;
		}
		this->notepadForm->characterMetrics = new CharacterMetrics(this->notepadForm);
		this->notepadForm->Notify();
		this->notepadForm->Invalidate();
	}
}

//NewCommand
NewCommand::NewCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

NewCommand::NewCommand(const NewCommand& source)
	: Command(source) {
}

NewCommand::~NewCommand() {

}

NewCommand& NewCommand::operator=(const NewCommand& source) {
	Command::operator=(source);

	return *this;
}

void NewCommand::Execute() {
	FileManager fileManager(this->notepadForm);
	int message;
	bool isDirty = this->notepadForm->document->GetIsDirty();
	string fileName = this->notepadForm->document->GetPathName();

	if (isDirty == true) {
		CString messageText;
		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "메모장", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "제목 없음") {
				CFileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					CString pathName = fileDialog.GetPathName();

					fileManager.Save((LPCTSTR)pathName);

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - 메모장");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);

					this->notepadForm->document->SetPathName((LPCTSTR)pathName);
				}
			}
			else {
				fileManager.Save(fileName);
			}
		}
	}

	if (isDirty == false || message != IDCANCEL) {
		//새로 만들기 처리
		fileManager.New();

		this->notepadForm->SetWindowTextA("제목 없음 - 메모장");

		this->notepadForm->document->SetPathName("제목 없음");

		this->notepadForm->document->SetIsDirty(false);
	}
}

//OpenCommand
OpenCommand::OpenCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

OpenCommand::OpenCommand(const OpenCommand& source)
	: Command(source) {
}

OpenCommand::~OpenCommand() {

}

OpenCommand& OpenCommand::operator=(const OpenCommand& source) {
	Command::operator=(source);

	return *this;
}

void OpenCommand::Execute() {
	FileManager fileManager(this->notepadForm);
	int message;
	bool isDirty = this->notepadForm->document->GetIsDirty();
	string fileName = this->notepadForm->document->GetPathName();

	if (isDirty == true) {
		CString messageText;
		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "메모장", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "제목 없음") {
				CFileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					CString pathName = fileDialog.GetPathName();

					fileManager.Save((LPCTSTR)pathName);

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - 메모장");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);

					this->notepadForm->document->SetPathName((LPCTSTR)pathName);
				}
			}
			else {
				fileManager.Save(fileName);
			}
		}
	}

	if (isDirty == false || message != IDCANCEL) {
		CFileDialog fileDialog(TRUE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();

			fileManager.Load((LPCTSTR)pathName);

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);

			this->notepadForm->document->SetPathName((LPCTSTR)pathName);
			this->notepadForm->document->SetIsDirty(false);
		}
	}
}

//SaveCommand
SaveCommand::SaveCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

SaveCommand::SaveCommand(const SaveCommand& source)
	: Command(source) {
}

SaveCommand::~SaveCommand() {

}

SaveCommand& SaveCommand::operator=(const SaveCommand& source) {
	Command::operator=(source);

	return *this;
}

void SaveCommand::Execute() {
	FileManager fileManager(this->notepadForm);
	string fileName = this->notepadForm->document->GetPathName();

	CFileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
	if (fileName == "제목 없음") {
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();
			fileManager.Save((LPCTSTR)pathName);
			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);

			this->notepadForm->document->SetPathName((LPCTSTR)pathName);
		}
	}
	else {
		fileManager.Save(fileName);
	}
	this->notepadForm->document->SetIsDirty(false);
}

//SaveAsCommand
SaveAsCommand::SaveAsCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

SaveAsCommand::SaveAsCommand(const SaveAsCommand& source)
	: Command(source) {
}

SaveAsCommand::~SaveAsCommand() {

}

SaveAsCommand& SaveAsCommand::operator=(const SaveAsCommand& source) {
	Command::operator=(source);

	return *this;
}

void SaveAsCommand::Execute() {
	FileManager fileManager(this->notepadForm);

	CFileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
	int ret = fileDialog.DoModal();
	if (ret == IDOK) {
		CString pathName = fileDialog.GetPathName();

		fileManager.Save((LPCTSTR)pathName);

		CString title = fileDialog.GetFileTitle();
		title.AppendFormat(" - 메모장");
		this->notepadForm->SetWindowTextA((LPCTSTR)title);

		this->notepadForm->document->SetPathName((LPCTSTR)pathName);
	}
	this->notepadForm->document->SetIsDirty(false);
}