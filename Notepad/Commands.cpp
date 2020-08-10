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
#include "Scanner.h"
#include "Selection.h"
#include "GlyphFactory.h"
#include "Note.h"

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

Long Command::Add(Command* command) {
	return -1;
}

Long Command::Remove(Long index) {
	return -1;
}

Command* Command::GetAt(Long index) {
	return 0;
}

Long Command::GetCapacity() const {
	return 0;
}

Long Command::GetLength() const {
	return -1;
}

//MacroCommand
MacroCommand::MacroCommand(NotepadForm* notepadForm, Long capacity)
	: Command(notepadForm), commands(10) {
	this->capacity = 10;
	this->length = 0;
}

MacroCommand::MacroCommand(const MacroCommand& source)
	: Command(source.notepadForm), commands(source.capacity) {
	Command* command;
	Long i = 0;
	while (i < source.length) {
		command = const_cast<MacroCommand&>(source).commands[i]->Clone();
		this->commands.Store(i, command);
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

MacroCommand::~MacroCommand() {
	Long i = 0;
	while (i < this->length) {
		if (this->commands[i] != 0) {
			delete this->commands[i];
		}
		i++;
	}
}

MacroCommand& MacroCommand::operator=(const MacroCommand& source) {
	Command::operator=(source);
	Long i = 0;
	while (i < this->length) {
		if (this->commands[i] != 0) {
			delete this->commands[i];
		}
		i++;
	}

	this->commands = source.commands;
	this->capacity = source.capacity;

	i = 0;
	while (i < this->length) {
		this->commands.Modify(i, const_cast<MacroCommand&>(source).commands[i]->Clone());
		i++;
	}

	this->length = source.length;

	return *this;
}

void MacroCommand::Execute() {
	Long i = 0;
	while (i < this->length) {
		this->commands[i]->Execute();
		i++;
	}
}

void MacroCommand::Unexecute() {
	Long i = this->length - 1;
	while (i >= 0) {
		this->commands[i]->Unexecute();
		i--;
	}
}

Long MacroCommand::Add(Command* command) {
	Long index;
	if (this->length < this->capacity) {
		index = this->commands.Store(this->length, command);
	}
	else {
		index = this->commands.AppendFromRear(command);
		this->capacity++;
	}
	this->length++;

	return index;
}

Long MacroCommand::Remove(Long index) {
	if (this->commands[index] != 0) {
		delete this->commands.GetAt(index);
	}
	index = this->commands.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

Command* MacroCommand::GetAt(Long index) {
	return this->commands.GetAt(index);
}

string MacroCommand::GetType() {
	return "Macro";
}

Command* MacroCommand::Clone() {
	return new MacroCommand(*this);
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
	this->nChar = -1;
	this->row = -1;
	this->column = -1;
}

WriteCommand::WriteCommand(const WriteCommand& source)
	: Command(source) {
	this->nChar = source.nChar;
	this->row = source.row;
	this->column = source.column;
}

WriteCommand::~WriteCommand() {

}

WriteCommand& WriteCommand::operator=(const WriteCommand& source) {
	Command::operator=(source);
	this->nChar = source.nChar;
	this->row = source.row;
	this->column = source.column;

	return *this;
}

void WriteCommand::Execute() {
	if (this->nChar == -1 && this->row == -1 && this->column == -1) {
		this->nChar = this->notepadForm->GetCurrentCharacter();
		this->row = this->notepadForm->note->GetCurrent();
		this->column = this->notepadForm->current->GetCurrent();
	}

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

		if (this->column >= this->notepadForm->current->GetLength()) {
			this->notepadForm->current->Add(character);
		}
		else {
			this->notepadForm->current->Add(this->column, character);
		}
	}
	else if (this->nChar == VK_RETURN) {
		if (this->column < this->notepadForm->current->GetLength()) {
			this->notepadForm->current = this->notepadForm->current->Divide(this->column);
			this->notepadForm->note->Add(this->row + 1, this->notepadForm->current);
			this->notepadForm->current->First();
		}
		else {
			this->notepadForm->current = glyphFactory.Make("\r\n");
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
	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
		this->notepadForm->note->UnselectAll();
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

void ImeCompositionCommand::Unexecute() {

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
	this->buffer = 0;
	this->row = -1;
	this->column = -1;
}

ImeCharCommand::ImeCharCommand(const ImeCharCommand& source)
	: Command(source) {
	this->buffer = source.buffer;
	this->row = source.row;
	this->column = source.column;
}

ImeCharCommand::~ImeCharCommand() {

}

ImeCharCommand& ImeCharCommand::operator=(const ImeCharCommand& source) {
	Command::operator=(source);
	this->buffer = source.buffer;
	this->row = source.row;
	this->column = source.column;

	return *this;
}

void ImeCharCommand::Execute() {
	if (this->buffer == 0 && this->row == -1 && this->column == -1) {
		this->buffer = new TCHAR[2];
		buffer = this->notepadForm->GetCurrentBuffer();
		this->row = this->notepadForm->note->GetCurrent();
		this->column = this->notepadForm->current->GetCurrent();
		if (this->notepadForm->GetIsComposing() == TRUE) {
			this->notepadForm->current->Remove(--this->column);
		}
	}

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Make(this->buffer);

	if (this->column >= this->notepadForm->current->GetLength()) {
		this->notepadForm->current->Add(glyph);
	}
	else {
		this->notepadForm->current->Add(this->column + 1, glyph);
	}

	this->notepadForm->SetIsComposing(FALSE);
}

void ImeCharCommand::Unexecute() {
	this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	this->notepadForm->current->Remove(this->column);

	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
		this->notepadForm->note->UnselectAll();
	}
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
	this->row = -1;
	this->noteLength = -1;
	this->column = -1;
	this->lineLength = -1;
	this->character = 0;
}

DeleteCommand::DeleteCommand(const DeleteCommand& source)
	: Command(source) {
	this->row = source.row;
	this->noteLength = source.noteLength;
	this->column = source.column;
	this->lineLength = source.lineLength;
	this->character = 0;
	if (source.character != 0) {
		this->character = source.character->Clone();
	}
}

DeleteCommand::~DeleteCommand() {
	if (this->character != NULL) {
		delete this->character;
	}
}

DeleteCommand& DeleteCommand::operator=(const DeleteCommand& source) {
	Command::operator=(source);
	this->row = source.row;
	this->noteLength = source.noteLength;
	this->column = source.column;
	this->lineLength = source.lineLength;
	this->character = 0;
	if (source.character != 0) {
		this->character = source.character->Clone();
	}

	return *this;
}

void DeleteCommand::Execute() {
	if (this->row == -1 && this->noteLength == -1 && this->column == -1 && this->lineLength == -1 && this->character == 0) {
		this->row = this->notepadForm->note->GetCurrent();
		this->noteLength = this->notepadForm->note->GetLength();
		this->column = this->notepadForm->current->GetCurrent();
		this->lineLength = this->notepadForm->current->GetLength();
		this->character = 0;
		if (this->column < this->notepadForm->current->GetLength()) {
			this->character = this->notepadForm->current->GetAt(this->column)->Clone();
		}
	}

	this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(this->row);

	if (this->column < this->lineLength) {
		this->notepadForm->current->Remove(this->column);
	}
	else if (this->column >= lineLength && this->row < this->noteLength - 1) {
		Glyph* other = this->notepadForm->note->GetAt(this->row + 1);
		this->notepadForm->current->Combine(other);
		this->notepadForm->note->Remove(this->row + 1);
	}
}

void DeleteCommand::Unexecute() {
	this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(this->row);

	Glyph* line;
	if (this->column < this->lineLength) {
		this->notepadForm->current->Add(this->column, this->character->Clone());
	}
	else if (this->column >= this->lineLength && this->row < this->noteLength - 1) {
		line = this->notepadForm->current->Divide(this->column);
		this->notepadForm->note->Add(this->row + 1, line);
	}
	this->notepadForm->note->Move(this->row);
	this->notepadForm->current->Move(this->column);
}

string DeleteCommand::GetType() {
	return "Delete";
}

Command* DeleteCommand::Clone() {
	return new DeleteCommand(*this);
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

void CopyCommand::Unexecute() {

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
	this->macroCommand = new MacroCommand(this->notepadForm);
}

PasteCommand::PasteCommand(const PasteCommand& source)
	: Command(source) {
	this->macroCommand = new MacroCommand(*const_cast<PasteCommand&>(source).macroCommand);
}

PasteCommand::~PasteCommand() {
	if (this->macroCommand != 0) {
		delete this->macroCommand;
	}
}

PasteCommand& PasteCommand::operator=(const PasteCommand& source) {
	Command::operator=(source);
	if (this->macroCommand != 0) {
		delete this->macroCommand;
	}
	this->macroCommand = source.macroCommand;

	return *this;
}

void PasteCommand::Execute() {
	Command* command = 0;

	if (this->notepadForm->selection != NULL) {
		command = new DeleteSelectionCommand(this->notepadForm);
		command->Execute();
		//this->notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));
	}

	Long count = 0;
	Long i;
	char* (*tokens);
	if (this->macroCommand->GetLength() <= 0) {
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

		i = 0;
		//복사한 문자열을 임시 Note로 만들다.
		Scanner scanner(clipBoard);
		while (scanner.IsEnd() == FALSE) {
			string token = scanner.GetToken();
			char(*cToken) = new char[token.length() + 1];
			strcpy(cToken, token.c_str());
			if (!(cToken[0] & 0x80)) {
				this->notepadForm->SetCurrentCharacter(cToken[0]);
				command = new WriteCommand(this->notepadForm);
			}
			else if (cToken[0] == '\r' && cToken[1] == '\n') {
				this->notepadForm->SetCurrentCharacter(VK_RETURN);
				command = new WriteCommand(this->notepadForm);
			}
			else if (cToken[0] & 0x80) {
				this->notepadForm->SetCurrentBuffer(cToken);
				command = new ImeCharCommand(this->notepadForm);
			}

			if (command != 0) {
				this->macroCommand->Add(command);
				command->Execute();
			}

			if (cToken != 0) {
				delete[] cToken;
			}
			scanner.Next();
		}
	}
	else {
		this->macroCommand->Execute();
	}
}

void PasteCommand::Unexecute() {
	this->macroCommand->Unexecute();

	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
		this->notepadForm->note->UnselectAll();
	}
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
	this->macroCommand = new MacroCommand(notepadForm);
}

CutCommand::CutCommand(const CutCommand& source)
	: Command(source) {
	this->macroCommand = new MacroCommand(*const_cast<CutCommand&>(source).macroCommand);
}

CutCommand::~CutCommand() {
	if (this->macroCommand != 0) {
		delete this->macroCommand;
	}
}

CutCommand& CutCommand::operator=(const CutCommand& source) {
	Command::operator=(source);
	if (this->macroCommand != 0) {
		delete this->macroCommand;
	}
	this->macroCommand = source.macroCommand;

	return *this;
}

void CutCommand::Execute() {
	Command* command;

	Long i;
	if (this->macroCommand->GetLength() <= 0) { //없으면 만들기
		command = new CopyCommand(this->notepadForm);
		this->macroCommand->Add(command);

		command = new DeleteSelectionCommand(this->notepadForm);
		this->macroCommand->Add(command);
	}

	this->macroCommand->Execute();
}

void CutCommand::Unexecute() {
	this->macroCommand->Unexecute();
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
	this->macroCommand = new MacroCommand(notepadForm);
}

DeleteSelectionCommand::DeleteSelectionCommand(const DeleteSelectionCommand& source)
	: Command(source) {
	this->macroCommand = new MacroCommand(*const_cast<DeleteSelectionCommand&>(source).macroCommand);
}

DeleteSelectionCommand::~DeleteSelectionCommand() {
	if (this->macroCommand != 0) {
		delete this->macroCommand;
	}
}

DeleteSelectionCommand& DeleteSelectionCommand::operator=(const DeleteSelectionCommand& source) {
	Command::operator=(source);
	if (this->macroCommand != 0) {
		delete this->macroCommand;
	}
	this->macroCommand = source.macroCommand;

	return *this;
}

void DeleteSelectionCommand::Execute() {
	Command* command;

	Long i;
	if (this->macroCommand->GetLength() <= 0) { //없으면 만들기
		Long startRow;
		Long startColumn = 0;

		Long start = this->notepadForm->selection->GetStart();
		Long end = this->notepadForm->selection->GetEnd();
		Glyph* character;
		Glyph* line;
		Long j;
		startRow = start;
		i = start;
		while (i <= end) {
			line = this->notepadForm->note->GetAt(i);
			j = 0;
			while (j < line->GetLength()) {
				character = line->GetAt(j);
				if (character->GetIsSelected()) {
					if (i == start && (j == 0 || line->GetAt(j - 1)->GetIsSelected() == false)) {
						startColumn = j;
					}
					command = new DeleteCommand(this->notepadForm);
					this->macroCommand->Add(command);
				}
				j++;
			}
			if (j >= this->notepadForm->current->GetLength() && i < end) {
				command = new DeleteCommand(this->notepadForm);
				this->macroCommand->Add(command);
			}
			i++;
		}
		this->notepadForm->note->Move(startRow);
		this->notepadForm->current = this->notepadForm->note->GetAt(startRow);
		this->notepadForm->current->Move(startColumn);
	}

	this->macroCommand->Execute();
}

void DeleteSelectionCommand::Unexecute() {
	this->macroCommand->Unexecute();
}

#if 0
void DeleteSelectionCommand::Execute() {
	GlyphFactory factory;
	Glyph* clipboardLine;

	Long start = this->notepadForm->selection->GetStart();
	Long end = this->notepadForm->selection->GetEnd();

	this->row = start; //undo

	Long index;
	Glyph* character;
	Glyph* line;
	Long j;
	Long i = start;
	while (i <= end) {
		line = this->notepadForm->note->GetAt(i);
		clipboardLine = factory.Make("\r\n"); //undo
		index = this->clipboard->Add(clipboardLine); //undo
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			if (character->GetIsSelected()) {
				clipboardLine->Add(character->Clone()); //undo
				line->Remove(j--);
			}
			j++;
		}

		//undo
		if (index == 0) {
			this->column = j;
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
	//this->notepadForm->note->UnselectAll();
}

void DeleteSelectionCommand::Unexecute() {
	Glyph* line = this->notepadForm->note->GetAt(this->row);
	Glyph* newLine;

	Glyph* clipboardCharacter;
	Glyph* clipboardLine;

	Long j;
	Long i = this->clipboard->GetLength() - 1;
	while (i >= 0) {
		clipboardLine = this->clipboard->GetAt(i);
		j = clipboardLine->GetLength() - 1;
		while (j >= 0) {
			clipboardCharacter = clipboardLine->GetAt(j);
			line->Add(this->column, clipboardCharacter->Clone());
			j--;
		}
		if (i > 0) {
			newLine = line->Divide(this->column);
			this->notepadForm->note->Add(this->row + 1, newLine);
		}
		i--;
	}
	Long index = this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Move(this->column);

	Long start = this->row;
	Long end = start + this->clipboard->GetLength() - 1;
	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
	}
	this->notepadForm->selection = new Selection(start, end);
}

void DeleteSelectionCommand::Reexecute() {
	GlyphFactory factory;

	Long start = this->row;
	Long end = start + this->clipboard->GetLength() - 1;

	Glyph* line;
	Long j;
	Long i = start;
	while (i <= end) {
		line = this->notepadForm->note->GetAt(i);
		j = this->column;
		while (j < line->GetLength()) {
			line->Remove(j);
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
}
#endif
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
		Command* command = this->notepadForm->undoHistoryBook->OpenAt();
		command->Unexecute();

		this->notepadForm->redoHistoryBook->Write(command->Clone());
		this->notepadForm->undoHistoryBook->Erase();
	}
}

string UndoCommand::GetType() {
	return "Undo";
}

Command* UndoCommand::Clone() {
	return new UndoCommand(*this);
}

//RedoCommand
RedoCommand::RedoCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

RedoCommand::RedoCommand(const RedoCommand& source)
	: Command(source) {
}

RedoCommand::~RedoCommand() {

}

RedoCommand& RedoCommand::operator=(const RedoCommand& source) {
	Command::operator=(source);

	return *this;
}

void RedoCommand::Execute() {
	if (this->notepadForm->redoHistoryBook->GetLength() > 0) {
		Command* command = this->notepadForm->redoHistoryBook->OpenAt();
		command->Execute();

		this->notepadForm->undoHistoryBook->Write(command->Clone());
		this->notepadForm->redoHistoryBook->Erase();
	}
}

string RedoCommand::GetType() {
	return "Redo";
}

Command* RedoCommand::Clone() {
	return new RedoCommand(*this);
}

//LeftCommand
LeftCommand::LeftCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

LeftCommand::LeftCommand(const LeftCommand& source)
	: Command(source) {
}

LeftCommand::~LeftCommand() {

}

LeftCommand& LeftCommand::operator=(const LeftCommand& source) {
	Command::operator=(source);

	return *this;
}

void LeftCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	else {
		if (this->notepadForm->current->GetCurrent() > 0) {
			this->notepadForm->current->Previous();
		}
		else if (this->notepadForm->note->GetCurrent() > 0) {
			Long index = this->notepadForm->note->Previous();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->Last();
		}
	}
}

string LeftCommand::GetType() {
	return "Left";
}

Command* LeftCommand::Clone() {
	return new LeftCommand(*this);
}

//RightCommand
RightCommand::RightCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

RightCommand::RightCommand(const RightCommand& source)
	: Command(source) {
}

RightCommand::~RightCommand() {

}

RightCommand& RightCommand::operator=(const RightCommand& source) {
	Command::operator=(source);

	return *this;
}

void RightCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	else {
		if (this->notepadForm->current->GetCurrent() < this->notepadForm->current->GetLength()) {
			this->notepadForm->current->Next();
		}
		else if (this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {
			Long index = this->notepadForm->note->Next();
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
			this->notepadForm->current->First();
		}
	}
}

string RightCommand::GetType() {
	return "Right";
}

Command* RightCommand::Clone() {
	return new RightCommand(*this);
}