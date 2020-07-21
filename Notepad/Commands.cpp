#include "Commands.h"
#include "NotepadForm.h"
#include "Font.h"
#include "CharacterMetrics.h"
#include "FileManager.h"
#include "Document.h"
#include "FileDialog.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "ScrollController.h"
#include "Editor.h"
#include "Selector.h"
#include "Highlight.h"

#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>
#include <dlgs.h>

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
				FileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->notepadForm->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->notepadForm->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - 메모장");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);
				}
			}
			else {
				//===== See annotation of Save() in Files.h =====
				string encodingType = "ANSI";
				this->notepadForm->document->SetEncodingType(encodingType);
				//===============================================

				fileManager.Save();
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
				FileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->notepadForm->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->notepadForm->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - 메모장");
					this->notepadForm->SetWindowTextA((LPCTSTR)title);
				}
			}
			else {
				//===== See annotation of Save() in Files.h =====
				string encodingType = "ANSI";
				this->notepadForm->document->SetEncodingType(encodingType);
				//===============================================

				fileManager.Save();
			}
		}
	}

	if (isDirty == false || message != IDCANCEL) {
		FileDialog fileDialog(TRUE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			CString pathName = fileDialog.GetPathName();
			this->notepadForm->document->SetPathName((LPCTSTR)pathName);

			fileManager.Load();

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);

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

	FileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
	if (fileName == "제목 없음") {
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			string encodingType = fileDialog.GetEncodingType();
			this->notepadForm->document->SetEncodingType(encodingType);
			CString pathName = fileDialog.GetPathName();
			this->notepadForm->document->SetPathName((LPCTSTR)pathName);

			fileManager.Save();

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - 메모장");
			this->notepadForm->SetWindowTextA((LPCTSTR)title);
		}
	}
	else {
		//===== See annotation of Save() in Files.h =====
		string encodingType = "ANSI";
		this->notepadForm->document->SetEncodingType(encodingType);
		//===============================================

		fileManager.Save();
	}

	if (this->notepadForm->document->GetIsDirty() == true) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Delete(0);
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(false);
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
	FileManager fileManager(this->notepadForm);

	FileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
	int ret = fileDialog.DoModal();
	if (ret == IDOK) {
		string encodingType = fileDialog.GetEncodingType();
		this->notepadForm->document->SetEncodingType(encodingType);
		CString pathName = fileDialog.GetPathName();
		this->notepadForm->document->SetPathName((LPCTSTR)pathName);

		fileManager.Save();

		CString title = fileDialog.GetFileTitle();
		title.AppendFormat(" - 메모장");
		this->notepadForm->SetWindowTextA((LPCTSTR)title);
	}

	if (this->notepadForm->document->GetIsDirty() == true) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Delete(0);
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(false);
	}
}

//CloseCommand
CloseCommand::CloseCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

CloseCommand::CloseCommand(const CloseCommand& source)
	: Command(source) {
}

CloseCommand::~CloseCommand() {

}

CloseCommand& CloseCommand::operator=(const CloseCommand& source) {
	Command::operator=(source);

	return *this;
}

void CloseCommand::Execute() {
	//this->notepadForm->OnClose();
}

//WriteCommand
WriteCommand::WriteCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

WriteCommand::WriteCommand(const WriteCommand& source)
	: Command(source) {
}

WriteCommand::~WriteCommand() {

}

WriteCommand& WriteCommand::operator=(const WriteCommand& source) {
	Command::operator=(source);

	return *this;
}

void WriteCommand::Execute() {
	UINT nChar = this->notepadForm->GetCurrentCharacter();

	GlyphFactory glyphFactory;
	TCHAR content[2];
	Long index;

	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	SHORT isShift = GetKeyState(VK_SHIFT) & 0X8000;
	SHORT isAlt = GetKeyState(VK_MENU) & 0X8000;

	if (this->notepadForm->highlight != NULL && (!isCtrl && !isShift && !isAlt)) {
		this->notepadForm->editor->Delete();
	}

	if (nChar >= 32 || nChar == VK_TAB) {
		content[0] = nChar;
		Glyph* character = glyphFactory.Make(content);

		index = this->notepadForm->current->GetCurrent();

		if (index >= this->notepadForm->current->GetLength()) {
			this->notepadForm->current->Add(character);
		}
		else {
			this->notepadForm->current->Add(index, character);
		}
	}
	else if (nChar == VK_RETURN) {
		index = this->notepadForm->current->GetCurrent();

		if (index < this->notepadForm->current->GetLength()) {
			this->notepadForm->current = this->notepadForm->current->Divide(index);
			index = this->notepadForm->note->GetCurrent();
			this->notepadForm->note->Add(index + 1, this->notepadForm->current);
			this->notepadForm->current->First();
		}
		else {
			this->notepadForm->current = glyphFactory.Make("\r\n");
			index = this->notepadForm->note->GetCurrent();
			this->notepadForm->note->Add(index + 1, this->notepadForm->current);
		}
	}
	this->notepadForm->Notify();
	this->notepadForm->Invalidate();

	//Document 처리
	if (this->notepadForm->document->GetIsDirty() == false &&
		(nChar >= 32 || nChar == VK_TAB || nChar == VK_RETURN)) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Insert(0, '*');
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(true);
	}

	//스마트스크롤 부분
	Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current) + 1;
	Long y = this->notepadForm->characterMetrics->GetY(this->notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->notepadForm->scrollController->SmartScrollToPoint(x, y);
}

//ImeCompositionCommand
ImeCompositionCommand::ImeCompositionCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

ImeCompositionCommand::ImeCompositionCommand(const ImeCompositionCommand& source)
	: Command(source) {
}

ImeCompositionCommand::~ImeCompositionCommand() {

}

ImeCompositionCommand& ImeCompositionCommand::operator=(const ImeCompositionCommand& source) {
	Command::operator=(source);

	return *this;
}

void ImeCompositionCommand::Execute() {
	TCHAR(*buffer) = new TCHAR[2];
	buffer = this->notepadForm->GetCurrentBuffer();
	
	Long index;
	if (this->notepadForm->highlight != NULL) {
		this->notepadForm->editor->Delete();
	}

	if (this->notepadForm->GetIsComposing() == TRUE) {
		index = this->notepadForm->current->GetCurrent();
		this->notepadForm->current->Remove(index - 1);
	}

	if (buffer[0] != '\0') {
		this->notepadForm->SetIsComposing(TRUE);
		GlyphFactory glyphFactory;
		Glyph* doubleByteCharacter = glyphFactory.Make(buffer);
		index = this->notepadForm->current->GetCurrent();

		if (index >= this->notepadForm->current->GetLength()) {
			this->notepadForm->current->Add(doubleByteCharacter);
		}
		else {
			this->notepadForm->current->Add(index, doubleByteCharacter);
		}
	}
	else {
		this->notepadForm->SetIsComposing(FALSE);
	}
	this->notepadForm->Notify();
	this->notepadForm->Invalidate();

	if (this->notepadForm->document->GetIsDirty() == false) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Insert(0, '*');
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(true);
	}

	//스마트스크롤 부분
	Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current) + 1; // 
	Long y = this->notepadForm->characterMetrics->GetY(this->notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->notepadForm->scrollController->SmartScrollToPoint(x, y);
}

//ImeCharCommand
ImeCharCommand::ImeCharCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

ImeCharCommand::ImeCharCommand(const ImeCharCommand& source)
	: Command(source) {
}

ImeCharCommand::~ImeCharCommand() {

}

ImeCharCommand& ImeCharCommand::operator=(const ImeCharCommand& source) {
	Command::operator=(source);

	return *this;
}

void ImeCharCommand::Execute() {
	TCHAR(*buffer) = new TCHAR[2];
	buffer = this->notepadForm->GetCurrentBuffer();

	Long index = this->notepadForm->current->GetCurrent();
	this->notepadForm->current->Remove(index - 1);

	if (this->notepadForm->highlight != NULL) {
		this->notepadForm->editor->Delete();
	}

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Make(buffer);

	if (this->notepadForm->current->GetCurrent() >= this->notepadForm->current->GetLength()) {
		this->notepadForm->current->Add(glyph);
	}
	else {
		this->notepadForm->current->Add(this->notepadForm->current->GetCurrent(), glyph);
	}

	this->notepadForm->SetIsComposing(FALSE);
	this->notepadForm->Notify();
	this->notepadForm->Invalidate();

	if (this->notepadForm->document->GetIsDirty() == false) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Insert(0, '*');
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(true);
	}

	//스마트스크롤 부분
	Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current) + 1; // 
	Long y = this->notepadForm->characterMetrics->GetY(this->notepadForm->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->notepadForm->scrollController->SmartScrollToPoint(x, y);
}

//DeleteCommand
DeleteCommand::DeleteCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

DeleteCommand::DeleteCommand(const DeleteCommand& source)
	: Command(source) {
}

DeleteCommand::~DeleteCommand() {

}

DeleteCommand& DeleteCommand::operator=(const DeleteCommand& source) {
	Command::operator=(source);

	return *this;
}

void DeleteCommand::Execute() {
	Long column = this->notepadForm->current->GetCurrent();
	Long lineLength = this->notepadForm->current->GetLength();
	if (column < lineLength) {
		this->notepadForm->current->Remove(column);
	}
	Long row = this->notepadForm->note->GetCurrent();
	Long noteLength = this->notepadForm->note->GetLength();
	if (column >= lineLength && row < noteLength - 1) {
		Glyph* other = this->notepadForm->note->GetAt(row + 1);
		this->notepadForm->current->Combine(other);
		this->notepadForm->note->Remove(row + 1);
	}
}

//BackspaceCommand
BackspaceCommand::BackspaceCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

BackspaceCommand::BackspaceCommand(const BackspaceCommand& source)
	: Command(source) {
}

BackspaceCommand::~BackspaceCommand() {

}

BackspaceCommand& BackspaceCommand::operator=(const BackspaceCommand& source) {
	Command::operator=(source);

	return *this;
}

void BackspaceCommand::Execute() {
	if (this->notepadForm->GetIsComposing() == FALSE) {
		Long lineCurrent = this->notepadForm->current->GetCurrent();
		Long noteCurrent = this->notepadForm->note->GetCurrent();
		if (lineCurrent > 0) {
			this->notepadForm->current->Remove(lineCurrent - 1);
		}
		else if (lineCurrent <= 0 && noteCurrent > 0) {
			Glyph* previousLine = this->notepadForm->note->GetAt(noteCurrent - 1);
			Long index = previousLine->GetLength();
			this->notepadForm->current = previousLine->Combine(this->notepadForm->current);
			this->notepadForm->note->Remove(noteCurrent);
			this->notepadForm->current->Move(index);
		}
	}
}

//CopyCommand
CopyCommand::CopyCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

CopyCommand::CopyCommand(const CopyCommand& source)
	: Command(source) {
}

CopyCommand::~CopyCommand() {

}

CopyCommand& CopyCommand::operator=(const CopyCommand& source) {
	Command::operator=(source);

	return *this;
}

void CopyCommand::Execute() {
	if (this->notepadForm->highlight != NULL) {
		this->notepadForm->editor->Copy();
	}
}

//PasteCommand
PasteCommand::PasteCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

PasteCommand::PasteCommand(const PasteCommand& source)
	: Command(source) {
}

PasteCommand::~PasteCommand() {

}

PasteCommand& PasteCommand::operator=(const PasteCommand& source) {
	Command::operator=(source);

	return *this;
}

void PasteCommand::Execute() {
	this->notepadForm->editor->Paste();
	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}
}

//CutCommand
CutCommand::CutCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

CutCommand::CutCommand(const CutCommand& source)
	: Command(source) {
}

CutCommand::~CutCommand() {

}

CutCommand& CutCommand::operator=(const CutCommand& source) {
	Command::operator=(source);

	return *this;
}

void CutCommand::Execute() {
	this->notepadForm->editor->Copy();

	this->notepadForm->editor->Delete();
}

//SelectAllCommand
SelectAllCommand::SelectAllCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

SelectAllCommand::SelectAllCommand(const SelectAllCommand& source)
	: Command(source) {
}

SelectAllCommand::~SelectAllCommand() {

}

SelectAllCommand& SelectAllCommand::operator=(const SelectAllCommand& source) {
	Command::operator=(source);

	return *this;
}

void SelectAllCommand::Execute() {
	if (this->notepadForm->highlight == NULL) {
		this->notepadForm->highlight = new Highlight;
		this->notepadForm->editor->selector = new Selector(this->notepadForm);

		Long index = this->notepadForm->note->Last();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		this->notepadForm->current->Last();

		Long i = 0;
		while (i <= index) {
			Glyph* line = this->notepadForm->note->GetAt(i);
			Long startColumn = 0;
			Long endColumn = line->GetLength();
			this->notepadForm->editor->selector->Right(i, startColumn, endColumn);
			i++;
		}
	}
}

//DeleteSelectionCommand
DeleteSelectionCommand::DeleteSelectionCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

DeleteSelectionCommand::DeleteSelectionCommand(const DeleteSelectionCommand& source)
	: Command(source) {
}

DeleteSelectionCommand::~DeleteSelectionCommand() {

}

DeleteSelectionCommand& DeleteSelectionCommand::operator=(const DeleteSelectionCommand& source) {
	Command::operator=(source);

	return *this;
}

void DeleteSelectionCommand::Execute() {
	this->notepadForm->editor->Delete();
}