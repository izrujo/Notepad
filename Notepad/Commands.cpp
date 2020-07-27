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
#include "HistoryBook.h"
#include "History.h"
#include "Scanner.h"
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
		//��Ʈ ��ȭ ���ڿ��� ��Ʈ ������ �����´�.
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
		messageText.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "�޸���", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "���� ����") {
				FileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->notepadForm->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->notepadForm->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - �޸���");
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
		//���� ����� ó��
		fileManager.New();

		this->notepadForm->SetWindowTextA("���� ���� - �޸���");

		this->notepadForm->document->SetPathName("���� ����");

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
		messageText.Format("���� ������ %s�� �����Ͻðڽ��ϱ�?", fileName.c_str());
		message = MessageBox(NULL, (LPCTSTR)messageText, "�޸���", MB_YESNOCANCEL);
		if (message == IDYES) {
			if (fileName == "���� ����") {
				FileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
				int ret = fileDialog.DoModal();
				if (ret == IDOK) {
					string encodingType = fileDialog.GetEncodingType();
					this->notepadForm->document->SetEncodingType(encodingType);
					CString pathName = fileDialog.GetPathName();
					this->notepadForm->document->SetPathName((LPCTSTR)pathName);

					fileManager.Save();

					CString title = fileDialog.GetFileTitle();
					title.AppendFormat(" - �޸���");
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
			title.AppendFormat(" - �޸���");
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
	if (fileName == "���� ����") {
		int ret = fileDialog.DoModal();
		if (ret == IDOK) {
			string encodingType = fileDialog.GetEncodingType();
			this->notepadForm->document->SetEncodingType(encodingType);
			CString pathName = fileDialog.GetPathName();
			this->notepadForm->document->SetPathName((LPCTSTR)pathName);

			fileManager.Save();

			CString title = fileDialog.GetFileTitle();
			title.AppendFormat(" - �޸���");
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
		title.AppendFormat(" - �޸���");
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

	if (this->notepadForm->highlight != NULL && (!isCtrl && !isShift && !isAlt)) {
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

	//Document ó��
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
	if (this->notepadForm->highlight != NULL) {
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
	if (this->notepadForm->highlight != NULL) {
		CString clipBoard;
		Long i = 0;
		while (i < this->notepadForm->highlight->GetLength()) {
			Glyph* line = this->notepadForm->highlight->GetAt(i);
			string content = line->GetContent();
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
	//�ý��� Ŭ�����忡�� ����� ���ڿ��� ��������.
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
	//������ ���ڿ��� �ӽ� Note�� �����.
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
	//���� ���� ���� ��ġ���� ������.
	Long current = this->notepadForm->current->GetCurrent();
	Glyph* line = this->notepadForm->current->Divide(current);
	//���� ���� ���� ��ġ���� ������ ��Ʈ�� ù ��° ���� ���ڸ� �ϳ��� �߰��Ѵ�.
	Glyph* copiedLine = glyphClipBoard->GetAt(i++);
	Long j = 0;
	while (j < copiedLine->GetLength()) {
		this->notepadForm->current->Add(copiedLine->GetAt(j));
		j++;
	}
	//������ ��Ʈ�� �� ����ŭ �ݺ��Ѵ�.
	while (i < glyphClipBoard->GetLength()) {
		//������ ��Ʈ�� ���� ���� ��������.
		copiedLine = glyphClipBoard->GetAt(i);
		//���� ��Ʈ�� ���� ��ġ�� ������ ���� �߰��ϴ�.
		Long noteCurrent = this->notepadForm->note->GetCurrent();
		this->notepadForm->note->Add(noteCurrent + 1, copiedLine);
		i++;
	}
	//���������� �߰��� ���� ���� �ٷ� �Ѵ�.
	this->notepadForm->current = this->notepadForm->note->GetAt(this->notepadForm->note->GetCurrent());
	//������ �ٿ� �Ʊ� ĳ�� ��ġ���� ���� ���� �̾� ���̴�.
	this->notepadForm->current->Combine(line);

	if (this->notepadForm->highlight != NULL) {
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
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
	this->highlight = new Highlight(10);
}

DeleteSelectionCommand::DeleteSelectionCommand(const DeleteSelectionCommand& source)
	: Command(source) {
	this->column = source.column;
	this->row = source.row;
	this->highlight = new Highlight(*dynamic_cast<Highlight*>(source.highlight));
}

DeleteSelectionCommand::~DeleteSelectionCommand() {
	if (this->highlight != NULL) {
		delete this->highlight;
	}
}

DeleteSelectionCommand& DeleteSelectionCommand::operator=(const DeleteSelectionCommand& source) {
	Command::operator=(source);
	this->column = source.column;
	this->row = source.row;
	this->highlight = dynamic_cast<Highlight*>(source.highlight);

	return *this;
}

void DeleteSelectionCommand::Execute() {
	Long start;
	Long end;
	Long noteStart = this->notepadForm->editor->selector->GetNoteStartPosition();
	Long noteEnd = this->notepadForm->editor->selector->GetNoteEndPosition();
	if (noteStart < noteEnd) {
		start = this->notepadForm->editor->selector->GetLineStartPosition();
		end = this->notepadForm->editor->selector->GetLineEndPosition();
	}
	else if (noteStart > noteEnd) {
		noteStart = this->notepadForm->editor->selector->GetNoteEndPosition();
		noteEnd = this->notepadForm->editor->selector->GetNoteStartPosition();
		start = this->notepadForm->editor->selector->GetLineEndPosition();
		end = this->notepadForm->editor->selector->GetLineStartPosition();
	}
	else {
		if (this->notepadForm->editor->selector->GetLineStartPosition() < this->notepadForm->editor->selector->GetLineEndPosition()) {
			start = this->notepadForm->editor->selector->GetLineStartPosition();
			end = this->notepadForm->editor->selector->GetLineEndPosition();
		}
		else {
			start = this->notepadForm->editor->selector->GetLineEndPosition();
			end = this->notepadForm->editor->selector->GetLineStartPosition();
		}
	}

	this->column = start;
	this->row = noteStart;

	Glyph* line = this->notepadForm->note->GetAt(noteStart);
	Long noteRepeatCount = noteEnd - noteStart + 1;
	Long i = 0;
	while (i < noteRepeatCount) {
		Long lineEnd = end;
		if (noteRepeatCount > 1) {
			if (i == noteRepeatCount - 1) {
				lineEnd = end + start;
			}
			else {
				lineEnd = line->GetLength();
			}
		}
		Long repeatCount = lineEnd - start;
		Long j = 0;
		while (j < repeatCount) {
			line->Remove(start);
			j++;
		}
		if (i < noteRepeatCount - 1) {
			Glyph* nextLine = this->notepadForm->note->GetAt(noteStart + 1);
			line->Combine(nextLine);
			this->notepadForm->note->Remove(noteStart + 1);
		}
		i++;
	}
	this->notepadForm->current = this->notepadForm->note->GetAt(noteStart);
	if (this->notepadForm->highlight != NULL) {
		this->highlight = this->notepadForm->highlight->Clone();
		delete this->notepadForm->highlight;
		this->notepadForm->highlight = NULL;
	}
}

void DeleteSelectionCommand::Unexecute() {
	if (this->highlight != NULL) {
		if (this->notepadForm->highlight != NULL) {
			delete this->notepadForm->highlight;
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