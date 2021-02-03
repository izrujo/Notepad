#include "Commands.h"
#include "NotepadForm.h"
#include "FileManager.h"
#include "Document.h"
#include "FileDialog.h"
#include "PageSetupDialog.h"
#include "PreviewForm.h"
#include "PrintInformation.h"
#include "PrintingVisitor.h"
#include "PrintStateDialog.h"
#include "Printer.h"
#include "PrintJobManager.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Font.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/ScrollController.h"
#include "../TextEditor/HistoryBook.h"
#include "../TextEditor/Scanner.h"
#include "../TextEditor/Selection.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Note.h"
#include "../TextEditor/CaretController.h"
#include "../TextEditor/Scroll.h"
#include "../TextEditor/FindReplaceDialog.h"
#include "../TextEditor/DummyLine.h"
#include "../TextEditor/DummyManager.h"

#include "resource.h"
#include "../TextEditor/resource.h"

#include <afxdlgs.h>
#include <WinUser.h>
#include <direct.h>
#include <dlgs.h>
#include <winspool.h>

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
	LOGFONT logFont = this->notepadForm->textEditingForm->font->GetFont();
	COLORREF color;
	CFontDialog fontDialog(&logFont);
	int ret = fontDialog.DoModal();
	if (ret == IDOK) {
		//폰트 대화 상자에서 폰트 정보를 가져온다.
		fontDialog.GetCurrentFont(&logFont);
		color = fontDialog.GetColor();
		if (this->notepadForm->textEditingForm->font != NULL) {
			delete this->notepadForm->textEditingForm->font;
		}
		this->notepadForm->textEditingForm->font = new Font(logFont, color, this->notepadForm);
		if (this->notepadForm->textEditingForm->characterMetrics != NULL) {
			delete this->notepadForm->textEditingForm->characterMetrics;
		}
		this->notepadForm->textEditingForm->characterMetrics = new CharacterMetrics(this->notepadForm, this->notepadForm->textEditingForm->font);

		//=====자동 개행 처리=====
		if (this->notepadForm->textEditingForm->GetIsLockedHScroll() == TRUE) {
			CRect rect;
			this->notepadForm->textEditingForm->GetClientRect(rect);
			DummyManager dummyManager(this->notepadForm->textEditingForm->note, this->notepadForm->textEditingForm->characterMetrics, rect.Width());

			Long row = this->notepadForm->textEditingForm->note->GetCurrent();
			Long column = this->notepadForm->textEditingForm->current->GetCurrent();
			Long distance = dummyManager.CountDistance(row, column);
			Long i = 0;
			while (i < this->notepadForm->textEditingForm->note->GetLength()) {
				dummyManager.Unfold(i);
				i++;
			}
			i = 0;
			while (i < this->notepadForm->textEditingForm->note->GetLength()) {
				i = dummyManager.Fold(i);
				i++;
			}
			dummyManager.CountIndex(distance, &row, &column);
			this->notepadForm->textEditingForm->note->Move(row);
			this->notepadForm->textEditingForm->current = this->notepadForm->textEditingForm->note->GetAt(row);
			this->notepadForm->textEditingForm->current->Move(column);
		}
		//=====자동 개행 처리=====
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_FLAG_LOCKHSCROLL, 0));
	BOOL isLockedHScroll = this->notepadForm->textEditingForm->GetIsLockedHScroll();
	if (isLockedHScroll == FALSE) {
		this->notepadForm->menu.CheckMenuItem(IDM_FORMAT_AUTONEWLINE, MF_UNCHECKED | MF_BYCOMMAND);
	}
	else {
		this->notepadForm->menu.CheckMenuItem(IDM_FORMAT_AUTONEWLINE, MF_CHECKED | MF_BYCOMMAND);
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
	int ret = IDOK;
	FileDialog fileDialog(FALSE, "txt", "*", 524326, "Text File(*.txt) | *.txt ||");
	if (fileName == "제목 없음") {
		ret = fileDialog.DoModal();
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

	if (this->notepadForm->document->GetIsDirty() == true && ret == IDOK) {
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

	if (this->notepadForm->document->GetIsDirty() == true && ret == IDOK) {
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
	this->notepadForm->SendMessage(WM_CLOSE, 0);
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
	this->notepadForm->document->deviceMode = (DEVMODE*)GlobalLock(pageSetupDialog.psd.hDevMode);
	GlobalUnlock(pageSetupDialog.psd.hDevMode);
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
		memcpy(this->notepadForm->document->deviceMode->dmDeviceName, (VOID*)LPCTSTR(deviceName), 32);

		Glyph* note = this->notepadForm->textEditingForm->note->Clone();
		if (this->notepadForm->textEditingForm->GetIsLockedHScroll() == TRUE) {
			CRect rect;
			this->notepadForm->GetClientRect(rect);
			DummyManager dummyManager(note, this->notepadForm->textEditingForm->characterMetrics, rect.Width());
			Long i = 0;
			while (i < note->GetLength()) {
				dummyManager.Unfold(i);
				i++;
			}
		}

		PrintInformation* printInformation = new PrintInformation(this->notepadForm, note);

		if (printInformation->printerDC.StartDocA(this->notepadForm->document->GetPathName().c_str()) < 0) {
			AfxMessageBox(_T("Printer wouldn't initialize"));
		}
		else {
			this->notepadForm->printStateDialog = new PrintStateDialog(this->notepadForm);
			this->notepadForm->printStateDialog->SetActiveWindow();
			this->notepadForm->printStateDialog->ShowWindow(TRUE);

			Printer printer(this->notepadForm, printInformation);
			printer.Print();

			this->notepadForm->printJobManager->Check(this->notepadForm->printStateDialog);
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_COPY, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_PASTE, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_CUT, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_SELECTALL, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_UNDO, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REDO, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_FIND, 0));
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
	this->notepadForm->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_REPLACE, 0));
}

string ReplaceCommand::GetType() {
	return "Replace";
}

Command* ReplaceCommand::Clone() {
	return new ReplaceCommand(*this);
}

//ReportDirtyCommand
ReportDirtyCommand::ReportDirtyCommand(NotepadForm* notepadForm)
	: Command(notepadForm) {
}

ReportDirtyCommand::ReportDirtyCommand(const ReportDirtyCommand& source)
	: Command(source) {
}

ReportDirtyCommand::~ReportDirtyCommand() {

}

ReportDirtyCommand& ReportDirtyCommand::operator=(const ReportDirtyCommand& source) {
	Command::operator=(source);

	return *this;
}

void ReportDirtyCommand::Execute() {
	bool isDirty = this->notepadForm->document->GetIsDirty();
	if (isDirty == false) {
		CString title;
		this->notepadForm->GetWindowText(title);
		title.Insert(0, '*');
		isDirty = true;
		this->notepadForm->SetWindowTextA(title);
		this->notepadForm->document->SetIsDirty(isDirty);
	}
}

string ReportDirtyCommand::GetType() {
	return "ReportDirty";
}

Command* ReportDirtyCommand::Clone() {
	return new ReportDirtyCommand(*this);
}