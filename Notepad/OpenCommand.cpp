#include "OpenCommand.h"
#include "NotepadForm.h"
#include "File.h"
#include "Note.h"
#include "Line.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "Scanner.h"
#include "ScrollController.h"
#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>

OpenCommand::OpenCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

OpenCommand::OpenCommand(const OpenCommand& source)
	: Command(source.notepadForm) {
}

OpenCommand::~OpenCommand() {
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
					File file((LPCTSTR)pathName, ios_base::out);
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
				File file((LPCTSTR)this->notepadForm->fileName, ios_base::out);
				string content = this->notepadForm->note->GetContent();
				file.Save(content);
			}
			CFileDialog fileDialog(TRUE);
			int ret = fileDialog.DoModal();
			if (ret == IDOK) {
				CString pathName = fileDialog.GetPathName();
				File file((LPCTSTR)pathName, ios_base::in);
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
					Glyph *glyph = glyphFactory.Make(token.c_str());
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
				File file((LPCTSTR)pathName, ios_base::in);
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
					Glyph *glyph = glyphFactory.Make(token.c_str());
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
			File file((LPCTSTR)pathName, ios_base::in);
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
				Glyph *glyph = glyphFactory.Make(token.c_str());
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

OpenCommand& OpenCommand::operator =(const OpenCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}