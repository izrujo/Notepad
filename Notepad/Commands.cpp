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
#include "CaretController.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "AutoNewlineController.h"
#include "FindReplaceDialog.h"
#include "PageSetupDialog.h"
#include "PreviewForm.h"
#include "LineDivider.h"
#include "PrintInformation.h"
#include "PrintingVisitor.h"
#include "PrintStateDialog.h"
#include "Printer.h"

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
		this->notepadForm->characterMetrics = new CharacterMetrics(this->notepadForm, this->notepadForm->font);
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

//AutoNewlineCommand
AutoNewlineCommand::AutoNewlineCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

AutoNewlineCommand::AutoNewlineCommand(const AutoNewlineCommand& source)
	: Command(source) {

}

AutoNewlineCommand::~AutoNewlineCommand() {

}

AutoNewlineCommand& AutoNewlineCommand::operator=(const AutoNewlineCommand& source) {
	Command::operator=(source);

	return *this;
}

void AutoNewlineCommand::Execute() {
	if (this->notepadForm->autoNewlineController == NULL) {
		this->notepadForm->menu.CheckMenuItem(IDM_FORMAT_AUTONEWLINE, MF_CHECKED | MF_BYCOMMAND);
		this->notepadForm->autoNewlineController = new AutoNewlineController(this->notepadForm);
	}
	else {
		this->notepadForm->autoNewlineController->Release();
		delete this->notepadForm->autoNewlineController;
		this->notepadForm->autoNewlineController = NULL;
		this->notepadForm->menu.CheckMenuItem(IDM_FORMAT_AUTONEWLINE, MF_UNCHECKED | MF_BYCOMMAND);
	}

	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
		this->notepadForm->note->UnselectAll();
	}
}

string AutoNewlineCommand::GetType() {
	return "AutoNewline";
}

Command* AutoNewlineCommand::Clone() {
	return new AutoNewlineCommand(*this);
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

//PageSetupCommand
PageSetupCommand::PageSetupCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

PageSetupCommand::PageSetupCommand(const PageSetupCommand& source)
	: Command(source) {
}

PageSetupCommand::~PageSetupCommand() {

}

PageSetupCommand& PageSetupCommand::operator=(const PageSetupCommand& source) {
	Command::operator=(source);

	return *this;
}

void PageSetupCommand::Execute() {
	PageSetupDialog pageSetupDialog(this->notepadForm);
	pageSetupDialog.DoModal();
	this->notepadForm->document->deviceMode = pageSetupDialog.psd.hDevMode;
}

string PageSetupCommand::GetType() {
	return "PageSetup";
}

Command* PageSetupCommand::Clone() {
	return new PageSetupCommand(*this);
}

//PrintCommand
PrintCommand::PrintCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

PrintCommand::PrintCommand(const PrintCommand& source)
	: Command(source) {
}

PrintCommand::~PrintCommand() {

}

PrintCommand& PrintCommand::operator=(const PrintCommand& source) {
	Command::operator=(source);

	return *this;
}

void PrintCommand::Execute() {
	CPrintDialog pd(FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_NOSELECTION,
		this->notepadForm);
	if (IDOK == pd.DoModal()) {
		CString deviceName = pd.GetDeviceName();
		
		PrintInformation *printInformation = new PrintInformation(deviceName, this->notepadForm);

		PrintStateDialog* printStateDialog = new PrintStateDialog(this->notepadForm);
		printStateDialog->SetActiveWindow();
		printStateDialog->ShowWindow(TRUE);

		// start a page
		if (printInformation->printerDC.StartDocA(notepadForm->document->GetPathName().c_str()) < 0) {
			AfxMessageBox(_T("Printer wouldn't initialize"));
			if (printStateDialog != NULL) {
				printStateDialog->DestroyWindow();
			}
		}
		else {
			this->notepadForm->printer->Print(printInformation, printStateDialog);
		}
	}
}

string PrintCommand::GetType() {
	return "Print";
}

Command* PrintCommand::Clone() {
	return new PrintCommand(*this);
}

//PreviewCommand
PreviewCommand::PreviewCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

PreviewCommand::PreviewCommand(const PreviewCommand& source)
	: Command(source) {
}

PreviewCommand::~PreviewCommand() {

}

PreviewCommand& PreviewCommand::operator=(const PreviewCommand& source) {
	Command::operator=(source);

	return *this;
}

void PreviewCommand::Execute() {
	Glyph* note = this->notepadForm->note->Clone();
	Long row;
	Long column;
	LineDivider lineDivider(this->notepadForm->characterMetrics);
	lineDivider.Combine(note, &row, &column);
	PreviewForm* previewForm = new PreviewForm(this->notepadForm);
	previewForm->Create(NULL, "인쇄 미리 보기", 13565952UL, CRect(0, 0, 1200, 875));
	previewForm->ShowWindow(SW_NORMAL);
	previewForm->UpdateWindow();
}

string PreviewCommand::GetType() {
	return "Preview";
}

Command* PreviewCommand::Clone() {
	return new PreviewCommand(*this);
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

	this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(this->row);
	this->notepadForm->current->Move(this->column);

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
	this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(this->row);
	this->notepadForm->current->Move(this->column);

	Glyph* index;
	if (this->nChar >= 32 || this->nChar == VK_TAB) {
		this->notepadForm->current->Remove(this->column);
	}
	else if (this->nChar == VK_RETURN) {
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

string ImeCompositionCommand::GetType() {
	return "ImeComposition";
}

Command* ImeCompositionCommand::Clone() {
	return new ImeCompositionCommand(*this);
}

//ImeCharCommand
ImeCharCommand::ImeCharCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
	this->buffer = new TCHAR[2];
	this->buffer[0] = '\0';
	this->buffer[1] = '\0';
	this->row = -1;
	this->column = -1;
}

ImeCharCommand::ImeCharCommand(const ImeCharCommand& source)
	: Command(source) {
	this->buffer = new TCHAR[2];
	this->buffer[0] = source.buffer[0];
	this->buffer[1] = source.buffer[1];
	this->row = source.row;
	this->column = source.column;
}

ImeCharCommand::~ImeCharCommand() {
	if (this->buffer != 0) {
		delete[] this->buffer;
	}
}

ImeCharCommand& ImeCharCommand::operator=(const ImeCharCommand& source) {
	Command::operator=(source);
	if (this->buffer != 0) {
		delete[] this->buffer;
	}
	this->buffer = new TCHAR[2];
	this->buffer[0] = source.buffer[0];
	this->buffer[1] = source.buffer[1];
	this->row = source.row;
	this->column = source.column;

	return *this;
}

void ImeCharCommand::Execute() {
	if (this->buffer[0] == '\0' && this->row == -1 && this->column == -1) {
		this->buffer[0] = this->notepadForm->GetCurrentBuffer()[0];
		this->buffer[1] = this->notepadForm->GetCurrentBuffer()[1];
		this->row = this->notepadForm->note->GetCurrent();
		this->column = this->notepadForm->current->GetCurrent();
		if (this->notepadForm->GetIsComposing() == TRUE) {
			this->notepadForm->current->Remove(--this->column);
		}
	}

	this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(this->row);
	this->notepadForm->current->Move(this->column);

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Make(this->buffer);

	if (this->column >= this->notepadForm->current->GetLength()) {
		this->notepadForm->current->Add(glyph);
	}
	else {
		this->notepadForm->current->Add(this->column, glyph);
	}


}

void ImeCharCommand::Unexecute() {
	this->notepadForm->note->Move(this->row);
	this->notepadForm->current = this->notepadForm->note->GetAt(this->row);
	this->notepadForm->current->Move(this->column);

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
	this->notepadForm->current->Move(this->column);

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
	this->notepadForm->current->Move(this->column);

	Glyph* line;
	if (this->column < this->lineLength) {
		this->character->Select(false);
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

	if (this->macroCommand->GetLength() <= 0) {
		if (this->notepadForm->selection != NULL) {
			command = new DeleteSelectionCommand(this->notepadForm);
			this->macroCommand->Add(command);
			command->Execute();
		}

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

	//노트에 내용이 하나도 없으면 선택하지 않는다.
	if (!(this->notepadForm->note->GetLength() <= 1 && this->notepadForm->current->GetLength() <= 0)) {
		this->notepadForm->selection = new Selection(0, this->notepadForm->note->GetLength() - 1);
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
			if (j >= line->GetLength() && i < end) {
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

	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
		this->notepadForm->note->UnselectAll();
	}
}

void DeleteSelectionCommand::Unexecute() {
	this->macroCommand->Unexecute();
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
		Command* command = this->notepadForm->undoHistoryBook->OpenAt();
		command->Unexecute();

		this->notepadForm->redoHistoryBook->Write(command->Clone());
		this->notepadForm->undoHistoryBook->Erase();
	}
	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
		this->notepadForm->note->UnselectAll();
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
	if (this->notepadForm->selection != NULL) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
		this->notepadForm->note->UnselectAll();
	}
}

string RedoCommand::GetType() {
	return "Redo";
}

Command* RedoCommand::Clone() {
	return new RedoCommand(*this);
}

//FindCommand
FindCommand::FindCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

FindCommand::FindCommand(const FindCommand& source)
	: Command(source) {
}

FindCommand::~FindCommand() {

}

FindCommand& FindCommand::operator=(const FindCommand& source) {
	Command::operator=(source);

	return *this;
}

void FindCommand::Execute() {
	if (this->notepadForm->findReplaceDialog == NULL) {
		string selectedContent = "";
		if (notepadForm->selection != NULL) {
			selectedContent = notepadForm->note->GetSelectedContent();
		}
		this->notepadForm->findReplaceDialog = new FindReplaceDialog(TRUE, selectedContent, this->notepadForm);
		this->notepadForm->findReplaceDialog->SetActiveWindow();
		this->notepadForm->findReplaceDialog->ShowWindow(TRUE);
	}
}

string FindCommand::GetType() {
	return "Find";
}

Command* FindCommand::Clone() {
	return new FindCommand(*this);
}

//ReplaceCommand
ReplaceCommand::ReplaceCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

ReplaceCommand::ReplaceCommand(const ReplaceCommand& source)
	: Command(source) {
}

ReplaceCommand::~ReplaceCommand() {

}

ReplaceCommand& ReplaceCommand::operator=(const ReplaceCommand& source) {
	Command::operator=(source);

	return *this;
}

void ReplaceCommand::Execute() {
	if (this->notepadForm->findReplaceDialog == NULL) {
		string selectedContent = "";
		if (notepadForm->selection != NULL) {
			selectedContent = notepadForm->note->GetSelectedContent();
		}
		this->notepadForm->findReplaceDialog = new FindReplaceDialog(FALSE, selectedContent, this->notepadForm);
		this->notepadForm->findReplaceDialog->SetActiveWindow();
		this->notepadForm->findReplaceDialog->ShowWindow(TRUE);
	}
}

string ReplaceCommand::GetType() {
	return "Replace";
}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}

//=============== Move Command ===============
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

//UpCommand
UpCommand::UpCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

UpCommand::UpCommand(const UpCommand& source)
	: Command(source) {
}

UpCommand::~UpCommand() {

}

UpCommand& UpCommand::operator=(const UpCommand& source) {
	Command::operator=(source);

	return *this;
}

void UpCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	if (this->notepadForm->note->GetCurrent() > 0) {
		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);
	}
}

string UpCommand::GetType() {
	return "Up";
}

Command* UpCommand::Clone() {
	return new UpCommand(*this);
}

//DownCommand
DownCommand::DownCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

DownCommand::DownCommand(const DownCommand& source)
	: Command(source) {
}

DownCommand::~DownCommand() {

}

DownCommand& DownCommand::operator=(const DownCommand& source) {
	Command::operator=(source);

	return *this;
}

void DownCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	if (this->notepadForm->note->GetCurrent() < this->notepadForm->note->GetLength() - 1) {
		Long x = this->notepadForm->characterMetrics->GetX(this->notepadForm->current);
		Long index = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(index);
		Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
		this->notepadForm->current->Move(column);
	}
}

string DownCommand::GetType() {
	return "Down";
}

Command* DownCommand::Clone() {
	return new DownCommand(*this);
}

//HomeCommand
HomeCommand::HomeCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

HomeCommand::HomeCommand(const HomeCommand& source)
	: Command(source) {
}

HomeCommand::~HomeCommand() {

}

HomeCommand& HomeCommand::operator=(const HomeCommand& source) {
	Command::operator=(source);

	return *this;
}

void HomeCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->current->First();
}

string HomeCommand::GetType() {
	return "Home";
}

Command* HomeCommand::Clone() {
	return new HomeCommand(*this);
}

//EndCommand
EndCommand::EndCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

EndCommand::EndCommand(const EndCommand& source)
	: Command(source) {
}

EndCommand::~EndCommand() {

}

EndCommand& EndCommand::operator=(const EndCommand& source) {
	Command::operator=(source);

	return *this;
}

void EndCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->current->Last();
}

string EndCommand::GetType() {
	return "End";
}

Command* EndCommand::Clone() {
	return new EndCommand(*this);
}

//CtrlLeftCommand
CtrlLeftCommand::CtrlLeftCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

CtrlLeftCommand::CtrlLeftCommand(const CtrlLeftCommand& source)
	: Command(source) {
}

CtrlLeftCommand::~CtrlLeftCommand() {

}

CtrlLeftCommand& CtrlLeftCommand::operator=(const CtrlLeftCommand& source) {
	Command::operator=(source);

	return *this;
}

void CtrlLeftCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	Long index = this->notepadForm->note->MovePreviousWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

string CtrlLeftCommand::GetType() {
	return "CtrlLeft";
}

Command* CtrlLeftCommand::Clone() {
	return new CtrlLeftCommand(*this);
}

//CtrlRightCommand
CtrlRightCommand::CtrlRightCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

CtrlRightCommand::CtrlRightCommand(const CtrlRightCommand& source)
	: Command(source) {
}

CtrlRightCommand::~CtrlRightCommand() {

}

CtrlRightCommand& CtrlRightCommand::operator=(const CtrlRightCommand& source) {
	Command::operator=(source);

	return *this;
}

void CtrlRightCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	Long index = this->notepadForm->note->MoveNextWord();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
}

string CtrlRightCommand::GetType() {
	return "CtrlRight";
}

Command* CtrlRightCommand::Clone() {
	return new CtrlRightCommand(*this);
}

//CtrlHomeCommand
CtrlHomeCommand::CtrlHomeCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

CtrlHomeCommand::CtrlHomeCommand(const CtrlHomeCommand& source)
	: Command(source) {
}

CtrlHomeCommand::~CtrlHomeCommand() {

}

CtrlHomeCommand& CtrlHomeCommand::operator=(const CtrlHomeCommand& source) {
	Command::operator=(source);

	return *this;
}

void CtrlHomeCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	Long index = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();
}

string CtrlHomeCommand::GetType() {
	return "CtrlHome";
}

Command* CtrlHomeCommand::Clone() {
	return new CtrlHomeCommand(*this);
}

//CtrlEndCommand
CtrlEndCommand::CtrlEndCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

CtrlEndCommand::CtrlEndCommand(const CtrlEndCommand& source)
	: Command(source) {
}

CtrlEndCommand::~CtrlEndCommand() {

}

CtrlEndCommand& CtrlEndCommand::operator=(const CtrlEndCommand& source) {
	Command::operator=(source);

	return *this;
}

void CtrlEndCommand::Execute() {
	if (this->notepadForm->selection != NULL) {
		this->notepadForm->note->UnselectAll();
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	Long index = this->notepadForm->note->Last();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->Last();
}

string CtrlEndCommand::GetType() {
	return "CtrlEnd";
}

Command* CtrlEndCommand::Clone() {
	return new CtrlEndCommand(*this);
}

//PageUpCommand
PageUpCommand::PageUpCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

PageUpCommand::PageUpCommand(const PageUpCommand& source)
	: Command(source) {
}

PageUpCommand::~PageUpCommand() {

}

PageUpCommand& PageUpCommand::operator=(const PageUpCommand& source) {
	Command::operator=(source);

	return *this;
}

void PageUpCommand::Execute() {
	Long position = this->notepadForm->scrollController->PageUp();
	Long previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다. 따라서 현재 x값을 다시 더한다.
	Long x = this->notepadForm->caretController->GetCaretX() +
		this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다.따라서 이전 y값을 다시 더한다.
	Long y = this->notepadForm->caretController->GetCaretY() + previous;

	Long row = this->notepadForm->characterMetrics->GetRow(y - (previous - position));
	if (row < 0) {
		row = 0;
	}
	Long index = this->notepadForm->note->Move(row); // row가 벗어나는 경우는 없나?
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
	this->notepadForm->current->Move(column);
}

string PageUpCommand::GetType() {
	return "PageUp";
}

Command* PageUpCommand::Clone() {
	return new PageUpCommand(*this);
}

//PageDownCommand
PageDownCommand::PageDownCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

PageDownCommand::PageDownCommand(const PageDownCommand& source)
	: Command(source) {
}

PageDownCommand::~PageDownCommand() {

}

PageDownCommand& PageDownCommand::operator=(const PageDownCommand& source) {
	Command::operator=(source);

	return *this;
}

void PageDownCommand::Execute() {
	Long position = this->notepadForm->scrollController->PageDown();
	Long previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	this->notepadForm->ScrollWindow(0, previous - position);
	Long x = this->notepadForm->caretController->GetCaretX() +
		this->notepadForm->scrollController->GetHorizontalScroll()->GetPosition();
	Long y = this->notepadForm->caretController->GetCaretY() + previous;
	Long row = this->notepadForm->characterMetrics->GetRow(y - (previous - position));
	if (row > this->notepadForm->note->GetLength() - 1) {
		row = this->notepadForm->note->GetLength() - 1;
	}
	Long index = this->notepadForm->note->Move(row);
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	Long column = this->notepadForm->characterMetrics->GetColumn(this->notepadForm->current, x);
	this->notepadForm->current->Move(column);
}

string PageDownCommand::GetType() {
	return "PageDown";
}

Command* PageDownCommand::Clone() {
	return new PageDownCommand(*this);
}

//ShiftLeftCommand
ShiftLeftCommand::ShiftLeftCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

ShiftLeftCommand::ShiftLeftCommand(const ShiftLeftCommand& source)
	: Command(source) {
}

ShiftLeftCommand::~ShiftLeftCommand() {

}

ShiftLeftCommand& ShiftLeftCommand::operator=(const ShiftLeftCommand& source) {
	Command::operator=(source);

	return *this;
}

void ShiftLeftCommand::Execute() {
	Glyph* character;
	Long column;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (lineCurrent > 0) {
		column = this->notepadForm->current->Previous();
		character = this->notepadForm->current->GetAt(column);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
	}
	else if (noteCurrent > 0) {
		row = this->notepadForm->note->Previous();
		this->notepadForm->current = this->notepadForm->note->GetAt(row);
		this->notepadForm->current->Last();
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
}

string ShiftLeftCommand::GetType() {
	return "ShiftLeft";
}

Command* ShiftLeftCommand::Clone() {
	return new ShiftLeftCommand(*this);
}

//ShiftRightCommand
ShiftRightCommand::ShiftRightCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

ShiftRightCommand::ShiftRightCommand(const ShiftRightCommand& source)
	: Command(source) {
}

ShiftRightCommand::~ShiftRightCommand() {

}

ShiftRightCommand& ShiftRightCommand::operator=(const ShiftRightCommand& source) {
	Command::operator=(source);

	return *this;
}

void ShiftRightCommand::Execute() {
	Glyph* character;
	Long column;
	Long noteCurrent = this->notepadForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->notepadForm->current->GetCurrent();
	if (lineCurrent < this->notepadForm->current->GetLength()) {
		column = this->notepadForm->current->Next();
		character = this->notepadForm->current->GetAt(column - 1);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
	}
	else if (noteCurrent < this->notepadForm->note->GetLength() - 1) {
		row = this->notepadForm->note->Next();
		this->notepadForm->current = this->notepadForm->note->GetAt(row);
		this->notepadForm->current->First();
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->notepadForm->selection != NULL) {
		Long originStart = this->notepadForm->selection->GetStart();
		Long originEnd = this->notepadForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
	this->notepadForm->selection = new Selection(start, end);

	if (start == end && this->notepadForm->note->IsSelecting() == false) {
		delete this->notepadForm->selection;
		this->notepadForm->selection = NULL;
	}
}

string ShiftRightCommand::GetType() {
	return "ShiftRight";
}

Command* ShiftRightCommand::Clone() {
	return new ShiftRightCommand(*this);
}
//=============== Move Command ===============