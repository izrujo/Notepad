#include "Commands.h"
#include "NotepadForm.h"

#include "Font.h"
#include "CharacterMetrics.h"

#include "Note.h"
#include "File.h"
#include "GlyphFactory.h"
#include "CaretController.h"
#include "ScrollController.h"

#include "Line.h"
#include "Glyph.h"
#include "Scanner.h"

#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>

//Command
Command::Command(NotepadForm *notepadForm) {
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
	GlyphFactory glyphFactory;
	if (this->notepadForm->GetIsSaved() == FALSE) {
		CString messageText;
		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", (LPCTSTR)this->notepadForm->fileName);
		int message = MessageBox(NULL, (LPCTSTR)messageText, "메모장", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (this->notepadForm->fileName.Compare("제목 없음") == 0) {
				//여기부터
				CFileDialog fileDialog(FALSE);
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					CString pathName = fileDialog.GetPathName();
					File file((LPCTSTR)pathName);
					string content = this->notepadForm->note->GetContent();
					file.Save(content);
					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - 메모장");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);
					this->notepadForm->fileName = pathName;
				}
				//여기까지 동일
			}
			else {
				File file((LPCTSTR)this->notepadForm->fileName);
				string content = this->notepadForm->note->GetContent();
				file.Save(content);
			}
			//새로 만들기 처리
			if (this->notepadForm->note != NULL) {
				delete this->notepadForm->note;
			}
			this->notepadForm->note = glyphFactory.Make("");
			Glyph* line = glyphFactory.Make("\r\n");
			this->notepadForm->note->Add(line);
			this->notepadForm->SetWindowTextA("제목 없음 - 메모장");
			this->notepadForm->fileName = "제목 없음";

			Long index = this->notepadForm->note->First();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->First();
			// 스크롤 컨트롤러 재생성
			if (this->notepadForm->scrollController != NULL) {
				delete this->notepadForm->scrollController;
				this->notepadForm->scrollController = new ScrollController(this->notepadForm);
			}
			// 스크롤 컨트롤러 재생성
			this->notepadForm->Notify();
		}
		else if (message == IDNO) {
			if (this->notepadForm->note != NULL) {
				delete this->notepadForm->note;
			}
			this->notepadForm->note = glyphFactory.Make("");
			Glyph* line = glyphFactory.Make("\r\n");
			this->notepadForm->note->Add(line);
			this->notepadForm->SetWindowTextA("제목 없음 - 메모장");
			this->notepadForm->fileName = "제목 없음";

			Long index = this->notepadForm->note->First();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->First();
			// 스크롤 컨트롤러 재생성
			if (this->notepadForm->scrollController != NULL) {
				delete this->notepadForm->scrollController;
				this->notepadForm->scrollController = new ScrollController(this->notepadForm);
			}
			// 스크롤 컨트롤러 재생성
			this->notepadForm->Notify();
		}
	}
	else {
		if (this->notepadForm->note != NULL) {
			delete this->notepadForm->note;
		}
		this->notepadForm->note = glyphFactory.Make("");
		Glyph* line = glyphFactory.Make("\r\n");
		this->notepadForm->note->Add(line);
		this->notepadForm->SetWindowTextA("제목 없음 - 메모장");
		this->notepadForm->fileName = "제목 없음";

		Long index = this->notepadForm->note->First();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->First();
		// 스크롤 컨트롤러 재생성
		if (this->notepadForm->scrollController != NULL) {
			delete this->notepadForm->scrollController;
			this->notepadForm->scrollController = new ScrollController(this->notepadForm);
		}
		// 스크롤 컨트롤러 재생성
		this->notepadForm->Notify();
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
	GlyphFactory glyphFactory;
	if (this->notepadForm->GetIsSaved() == FALSE) {
		CString messageText;
		messageText.Format("변경 내용을 %s에 저장하시겠습니까?", (LPCTSTR)this->notepadForm->fileName);
		int message = MessageBox(NULL, (LPCTSTR)messageText, "메모장", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (this->notepadForm->fileName.Compare("제목 없음") == 0) {
				//여기부터
				CFileDialog fileDialog(FALSE);
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					CString pathName = fileDialog.GetPathName();
					File file((LPCTSTR)pathName);
					string content = this->notepadForm->note->GetContent();
					file.Save(content);
					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - 메모장");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);
					this->notepadForm->fileName = pathName;
				}
				//여기까지 동일
			}
			else {
				File file((LPCTSTR)this->notepadForm->fileName);
				string content = this->notepadForm->note->GetContent();
				file.Save(content);
			}
			CFileDialog fileDialog(TRUE);
			int ret = fileDialog.DoModal();
			if (ret == IDOK) {
				CString pathName = fileDialog.GetPathName();
				File file((LPCTSTR)pathName);
				string content = file.Load();
				if (this->notepadForm->note != NULL) {
					delete this->notepadForm->note;
				}
				this->notepadForm->note = glyphFactory.Make("");
				this->notepadForm->current = glyphFactory.Make("\r\n");
				this->notepadForm->note->Add(this->notepadForm->current);
				Scanner scanner(content);
				while (scanner.IsEnd() == FALSE) {
					string token = scanner.GetToken();
					Glyph* glyph = glyphFactory.Make(token.c_str());
					if (token != "\r\n") {
						this->notepadForm->current->Add(glyph);
					}
					else {
						Long index = this->notepadForm->note->Add(glyph);
						this->notepadForm->current = this->notepadForm->note->GetAt(index);
					}
					scanner.Next();
				}
				CString title = fileDialog.GetFileTitle();
				title.AppendFormat(" - 메모장");
				this->notepadForm->SetWindowTextA((LPCTSTR)title);
				this->notepadForm->fileName = pathName;

				Long index = this->notepadForm->note->First();
				this->notepadForm->current = this->notepadForm->note->GetAt(index);
				this->notepadForm->current->First();
				// 스크롤 컨트롤러 재생성
				if (this->notepadForm->scrollController != NULL) {
					delete this->notepadForm->scrollController;
					this->notepadForm->scrollController = new ScrollController(this->notepadForm);
				}
				// 스크롤 컨트롤러 재생성
				this->notepadForm->Notify();
			}
		}
		else if (message == IDNO) {
			CFileDialog fileDialog(TRUE);
			int ret = fileDialog.DoModal();
			if (ret == IDOK) {
				CString pathName = fileDialog.GetPathName();
				File file((LPCTSTR)pathName);
				string content = file.Load();
				if (this->notepadForm->note != NULL) {
					delete this->notepadForm->note;
				}
				this->notepadForm->note = glyphFactory.Make("");
				this->notepadForm->current = glyphFactory.Make("\r\n");
				this->notepadForm->note->Add(this->notepadForm->current);
				Scanner scanner(content);
				while (scanner.IsEnd() == FALSE) {
					string token = scanner.GetToken();
					Glyph* glyph = glyphFactory.Make(token.c_str());
					if (token != "\r\n") {
						this->notepadForm->current->Add(glyph);
					}
					else {
						Long index = this->notepadForm->note->Add(glyph);
						this->notepadForm->current = this->notepadForm->note->GetAt(index);
					}
					scanner.Next();
				}
				CString title = fileDialog.GetFileTitle();
				title.AppendFormat(" - 메모장");
				this->notepadForm->SetWindowTextA((LPCTSTR)title);
				this->notepadForm->fileName = pathName;

				Long index = this->notepadForm->note->First();
				this->notepadForm->current = this->notepadForm->note->GetAt(index);
				this->notepadForm->current->First();
				// 스크롤 컨트롤러 재생성
				if (this->notepadForm->scrollController != NULL) {
					delete this->notepadForm->scrollController;
					this->notepadForm->scrollController = new ScrollController(this->notepadForm);
				}
				// 스크롤 컨트롤러 재생성
				this->notepadForm->Notify();
			}
		}
	}
	else {
		CFileDialog fileDialog(TRUE);
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();
			File file((LPCTSTR)pathName);
			string content = file.Load();
			if (this->notepadForm->note != NULL) {
				delete this->notepadForm->note;
			}
			this->notepadForm->note = glyphFactory.Make("");
			this->notepadForm->current = glyphFactory.Make("\r\n");
			this->notepadForm->note->Add(this->notepadForm->current);
			Scanner scanner(content);
			while (scanner.IsEnd() == FALSE) {
				string token = scanner.GetToken();
				Glyph* glyph = glyphFactory.Make(token.c_str());
				if (token != "\r\n") {
					this->notepadForm->current->Add(glyph);
				}
				else {
					Long index = this->notepadForm->note->Add(glyph);
					this->notepadForm->current = this->notepadForm->note->GetAt(index);
				}
				scanner.Next();
			}
			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);
			this->notepadForm->fileName = pathName;

			Long index = this->notepadForm->note->First();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->First();
			// 스크롤 컨트롤러 재생성
			if (this->notepadForm->scrollController != NULL) {
				delete this->notepadForm->scrollController;
				this->notepadForm->scrollController = new ScrollController(this->notepadForm);
			}
			// 스크롤 컨트롤러 재생성
			this->notepadForm->Notify();
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
	if (this->notepadForm->fileName.Compare("제목 없음") == 0) {
		CFileDialog fileDialog(FALSE);
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();
			File file((LPCTSTR)pathName);
			string content = this->notepadForm->note->GetContent();
			file.Save(content);
			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);
			this->notepadForm->fileName = pathName;
		}
	}
	else {
		File file((LPCTSTR)this->notepadForm->fileName);
		string content = this->notepadForm->note->GetContent();
		file.Save(content);
	}
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
	CFileDialog fileDialog(FALSE);
	int ret = fileDialog.DoModal();
	if (ret == IDOK) {
		CString pathName = fileDialog.GetPathName();
		File file((LPCTSTR)pathName);
		string content = this->notepadForm->note->GetContent();
		file.Save(content);
		CString title = fileDialog.GetFileTitle();
		title.AppendFormat(" - 메모장");
		this->notepadForm->SetWindowTextA((LPCTSTR)title);
		this->notepadForm->fileName = pathName;
	}
}