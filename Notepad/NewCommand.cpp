#include "NewCommand.h"
#include "NotepadForm.h"
#include "Note.h"
#include "File.h"
#include "GlyphFactory.h"
#include "CaretController.h"
#include "ScrollController.h"
#include <afxdlgs.h>


NewCommand::NewCommand(NotepadForm *notepadForm)
	: Command(notepadForm) {
}

NewCommand::NewCommand(const NewCommand& source)
	: Command(source.notepadForm) {
}

NewCommand::~NewCommand() {
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
			//새로 만들기 처리
			if (this->notepadForm->note != NULL) {
				delete this->notepadForm->note;
			}
			this->notepadForm->note = glyphFactory.Make("");
			Glyph *line = glyphFactory.Make("\r\n");
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
			Glyph *line = glyphFactory.Make("\r\n");
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
		Glyph *line = glyphFactory.Make("\r\n");
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

NewCommand& NewCommand::operator =(const NewCommand& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}