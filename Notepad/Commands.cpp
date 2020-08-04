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
#include "HistoryBook.h"
#include "History.h"
#include "Scanner.h"
#include "Selection.h"

#include "resource.h"

#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>
#include <dlgs.h>
#pragma warning(disable:4996)

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

void Command::Unexecute() {

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

string FontCommand::GetType() {
	return "Font";
}

Command* FontCommand::Clone() {
	return new FontCommand(*this);
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

string NewCommand::GetType() {
	return "New";
}

Command* NewCommand::Clone() {
	return new NewCommand(*this);
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

string OpenCommand::GetType() {
	return "Open";
}

Command* OpenCommand::Clone() {
	return new OpenCommand(*this);
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

string SaveCommand::GetType() {
	return "Save";
}

Command* SaveCommand::Clone() {
	return new SaveCommand(*this);
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

string SaveAsCommand::GetType() {
	return "SaveAs";
}

Command* SaveAsCommand::Clone() {
	return new SaveAsCommand(*this);
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

string CloseCommand::GetType() {
	return "Close";
}

Command* CloseCommand::Clone() {
	return new CloseCommand(*this);
}

//WriteCommand
WriteCommand::WriteCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
	this->nChar = 0;
	this->column = -1;
	this->row = -1;
}

WriteCommand::WriteCommand(const WriteCommand& source)
	: Command(source) {
	this->nChar = source.nChar;
	this->column = source.column;
	this->row = source.row;
}

WriteCommand::~WriteCommand() {

}

WriteCommand& WriteCommand::operator=(const WriteCommand& source) {
	Command::operator=(source);
	this->nChar = source.nChar;
	this->column = source.column;
	this->row = source.row;

	return *this;
}

void WriteCommand::Execute() {
	this->nChar = this->notepadForm->GetCurrentCharacter();

	GlyphFactory glyphFactory;
	TCHAR content[2];

	//User event intended to OnKeyDown should call this.
	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	SHORT isShift = GetKeyState(VK_SHIFT) & 0X8000;
	SHORT isAlt = GetKeyState(VK_MENU) & 0X8000;

	if (this->notepadForm->selection != NULL && (!isCtrl && !isShift && !isAlt)) {
		this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
	}

	if (this->nChar >= 32 || this->nChar == VK_TAB) {
		content[0] = this->nChar;
		Glyph* character = glyphFactory.Make(content);

		this->row = this->notepadForm->note->GetCurrent();
		this->column = this->notepadForm->current->GetCurrent();

		if (this->column >= this->notepadForm->current->GetLength()) {
			this->notepadForm->current->Add(character);
		}
		else {
			this->notepadForm->current->Add(this->column, character);
		}
	}
	else if (this->nChar == VK_RETURN) {
		this->column = this->notepadForm->current->GetCurrent();

		if (this->column < this->notepadForm->current->GetLength()) {
			this->notepadForm->current = this->notepadForm->current->Divide(this->column);
			this->row = this->notepadForm->note->GetCurrent();
			this->notepadForm->note->Add(this->row + 1, this->notepadForm->current);
			this->notepadForm->current->First();
		}
		else {
			this->notepadForm->current = glyphFactory.Make("\r\n");
			this->row = this->notepadForm->note->GetCurrent();
			this->notepadForm->note->Add(this->row + 1, this->notepadForm->current);
		}
	}

	//Document 처리
	if (this->notepadForm->document->GetIsDirty() == false &&
		(this->nChar >= 32 || this->nChar == VK_TAB || this->nChar == VK_RETURN)) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Insert(0, '*');
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(true);
	}
}

void WriteCommand::Unexecute() {
	Glyph* index;
	if (this->nChar >= 32 || this->nChar == VK_TAB) {
		this->notepadForm->note->Move(this->row);
		this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
		this->notepadForm->current->Remove(this->column);
	}
	else if (this->nChar == VK_RETURN) {
		this->notepadForm->note->Move(this->row);
		this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
		index = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent() + 1);
		this->notepadForm->current->Combine(index);
		this->notepadForm->note->Remove(this->notepadForm->note->GetCurrent() + 1);
		this->notepadForm->current->Move(this->column);
	}
}

string WriteCommand::GetType() {
	return "Write";
}

Command* WriteCommand::Clone() {
	return new WriteCommand(*this);
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
	if (this->notepadForm->note->IsSelecting()) {
		this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
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

	if (this->notepadForm->document->GetIsDirty() == false) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Insert(0, '*');
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(true);
	}
}

string ImeCompositionCommand::GetType() {
	return "ImeComposition";
}

Command* ImeCompositionCommand::Clone() {
	return new ImeCompositionCommand(*this);
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

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Make(buffer);

	if (this->notepadForm->current->GetCurrent() >= this->notepadForm->current->GetLength()) {
		this->notepadForm->current->Add(glyph);
	}
	else {
		this->notepadForm->current->Add(this->notepadForm->current->GetCurrent(), glyph);
	}

	this->notepadForm->SetIsComposing(FALSE);
}

string ImeCharCommand::GetType() {
	return "ImeChar";
}

Command* ImeCharCommand::Clone() {
	return new ImeCharCommand(*this);
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

string DeleteCommand::GetType() {
	return "Delete";
}

Command* DeleteCommand::Clone() {
	return new DeleteCommand(*this);
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

string BackspaceCommand::GetType() {
	return "Backspace";
}

Command* BackspaceCommand::Clone() {
	return new BackspaceCommand(*this);
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
	if (this->notepadForm->selection != NULL) {
		Long start = this->notepadForm->selection->GetStart();
		Long end = this->notepadForm->selection->GetEnd();
		CString clipBoard;
		string content;
		Glyph* line;
		Glyph* character;
		Long j;
		Long i = start;
		while (i <= end) {
			content = "";
			line = this->notepadForm->note->GetAt(i);
			j = 0;
			while (j < line->GetLength()) {
				character = line->GetAt(j);
				if (character->GetIsSelected()) {
					content += character->GetContent();
				}
				j++;
			}
			content.append("\r\n");
			clipBoard.Append(content.c_str());
			i++;
		}

		HANDLE handle;
		char* address = NULL;
		handle = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, clipBoard.GetLength() - 1);
		address = (char*)::GlobalLock(handle);
		if (address == NULL) {
			::GlobalFree(handle);
		}
		strcpy(address, clipBoard);
		if (::OpenClipboard(this->notepadForm->m_hWnd)) {
			::EmptyClipboard();
			::SetClipboardData(CF_TEXT, handle);
			::CloseClipboard();
		}
		::GlobalUnlock(handle);
	}

}

string CopyCommand::GetType() {
	return "Copy";
}

Command* CopyCommand::Clone() {
	return new CopyCommand(*this);
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
	//시스템 클립보드에서 복사된 문자열을 가져오다.
	string clipBoard;
	HANDLE handle;
	LPSTR address = NULL;
	if (::IsClipboardFormatAvailable(CF_TEXT) != FALSE) {
		if (::OpenClipboard(this->notepadForm->m_hWnd)) {
			handle = GetClipboardData(CF_TEXT);
			if (handle != NULL) {
				address = (LPSTR)::GlobalLock(handle);
				clipBoard = address;
				::GlobalUnlock(handle);
			}
			CloseClipboard();
		}
	}
	//복사한 문자열을 임시 Note로 만들다.
	Scanner scanner(clipBoard);
	GlyphFactory glyphFactory;
	Glyph* glyphClipBoard = glyphFactory.Make("");
	Glyph* clipBoardLine = glyphFactory.Make("\r\n");
	glyphClipBoard->Add(clipBoardLine);
	while (scanner.IsEnd() == FALSE) {
		string token = scanner.GetToken();
		if (token != "\r\n") {
			Glyph* glyph = glyphFactory.Make(token.c_str());
			clipBoardLine->Add(glyph);
		}
		else {
			clipBoardLine = glyphFactory.Make(token.c_str());
			glyphClipBoard->Add(clipBoardLine);
		}
		scanner.Next();
	}

	Long i = 0;
	//현재 줄의 현재 위치에서 나누다.
	Long current = this->notepadForm->current->GetCurrent();
	Glyph* line = this->notepadForm->current->Divide(current);
	//현재 줄의 현재 위치부터 복사한 노트의 첫 번째 줄의 글자를 하나씩 추가한다.
	Glyph* copiedLine = glyphClipBoard->GetAt(i++);
	Long j = 0;
	while (j < copiedLine->GetLength()) {
		this->notepadForm->current->Add(copiedLine->GetAt(j));
		j++;
	}
	//복사한 노트의 줄 수만큼 반복한다.
	while (i < glyphClipBoard->GetLength()) {
		//복사한 노트의 현재 줄을 가져오다.
		copiedLine = glyphClipBoard->GetAt(i);
		//원래 노트의 현재 위치에 가져온 줄을 추가하다.
		Long noteCurrent = this->notepadForm->note->GetCurrent();
		this->notepadForm->note->Add(noteCurrent + 1, copiedLine);
		i++;
	}
	//마지막으로 추가한 줄을 현재 줄로 한다.
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	//마지막 줄에 아까 캐럿 위치에서 나눈 줄을 이어 붙이다.
	this->notepadForm->current->Combine(line);
}

string PasteCommand::GetType() {
	return "Paste";
}

Command* PasteCommand::Clone() {
	return new PasteCommand(*this);
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
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_COPY, 0));
	this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
}

string CutCommand::GetType() {
	return "Cut";
}

Command* CutCommand::Clone() {
	return new CutCommand(*this);
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
	Glyph* line;
	Long j;
	Long i = 0;
	while (i < this->notepadForm->note->GetLength()) {
		line = this->notepadForm->note->GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			line->GetAt(j)->Select(true);
			j++;
		}
		i++;
	}
	Long index = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();

	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
	}
	this->notepadForm->selection = new Selection(0, this->notepadForm->note->GetLength() - 1);
}

string SelectAllCommand::GetType() {
	return "SelectAll";
}

Command* SelectAllCommand::Clone() {
	return new SelectAllCommand(*this);
}

//DeleteSelectionCommand
DeleteSelectionCommand::DeleteSelectionCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
	this->column = -1;
	this->row = -1;
}

DeleteSelectionCommand::DeleteSelectionCommand(const DeleteSelectionCommand& source)
	: Command(source) {
	this->column = source.column;
	this->row = source.row;
}

DeleteSelectionCommand::~DeleteSelectionCommand() {

}

DeleteSelectionCommand& DeleteSelectionCommand::operator=(const DeleteSelectionCommand& source) {
	Command::operator=(source);
	this->column = source.column;
	this->row = source.row;

	return *this;
}

void DeleteSelectionCommand::Execute() {
	Long start = this->notepadForm->selection->GetStart();
	Long end = this->notepadForm->selection->GetEnd();

	Long lineCurrent = this->notepadForm->current->GetCurrent();
	Glyph* character;
	Glyph* line;
	Long j;
	Long i = start;
	while (i <= end) {
		line = this->notepadForm->note->GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			if (character->GetIsSelected()) {
				line->Remove(j--);
			}
			j++;
		}
		if (i < end) {
			this->notepadForm->current = line->Combine(this->notepadForm->note->GetAt(i + 1));
			this->notepadForm->note->Remove(i + 1);
			i--;
			end--;
		}
		i++;
	}

	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->note->UnselectAll();
}

void DeleteSelectionCommand::Unexecute() {
#if 0
	if (this->highlight != NULL) {
		if (this->notepadForm->selector != NULL) {
			delete this->notepadForm->selector;
		}

		this->notepadForm->current = this->notepadForm->note->GetAt(this->row);
		Glyph* line;
		Glyph* completeLine;
		Long j;
		Long rowLength = this->highlight->GetLength() - 1;
		Long lastColumn = this->highlight->GetAt(rowLength)->GetLength() - 1;
		Long i = rowLength;
		while (i >= this->row) {
			line = this->highlight->GetAt(i)->Clone();
			j = line->GetLength() - 1;
			while (j >= 0) {
				this->notepadForm->current->Add(this->column, line->GetAt(j));
				j--;
			}
			if (i > this->row) {
				completeLine = this->notepadForm->current->Divide(this->column);
				this->notepadForm->note->Add(this->row + 1, completeLine);
			}
			i--;
		}

		this->notepadForm->editor->Select(this->row, this->column, this->highlight->GetLength() - 1, lastColumn);
	}
#endif
}

string DeleteSelectionCommand::GetType() {
	return "DeleteSelection";
}

Command* DeleteSelectionCommand::Clone() {
	return new DeleteSelectionCommand(*this);
}

//UndoCommand
UndoCommand::UndoCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

UndoCommand::UndoCommand(const UndoCommand& source)
	: Command(source) {
}

UndoCommand::~UndoCommand() {

}

UndoCommand& UndoCommand::operator=(const UndoCommand& source) {
	Command::operator=(source);

	return *this;
}

void UndoCommand::Execute() {
	if (this->notepadForm->undoHistoryBook->GetLength() > 0) {
		Command* command;
		History* redo = new History(10);
		History* history = this->notepadForm->undoHistoryBook->OpenAt();
		while (history->GetLength() > 0) {
			command = history->Reveal();
			command->Unexecute();
			redo->Happen(command->Clone());
			history->Forget();
		}

		this->notepadForm->redoHistoryBook->Write(redo);
		this->notepadForm->undoHistoryBook->Erase();
	}
}

string UndoCommand::GetType() {
	return "Undo";
}

Command* UndoCommand::Clone() {
	return new UndoCommand(*this);
}