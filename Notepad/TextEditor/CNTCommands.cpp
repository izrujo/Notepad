#include "CNTCommands.h"
#include "TextEditingForm.h"
#include "GlyphFactory.h"
#include "Glyph.h"
#include "resource.h"
#include "Selection.h"
#include "Scanner.h"
#include "CharacterMetrics.h"
#include "CaretController.h"
#include "ScrollController.h"
#include "Scroll.h"
#include "HistoryBook.h"
#include "FindReplaceDialog.h"
#include "DummyManager.h"
#include "DummyLine.h"

#include "../Notepad/NotepadForm.h"
#include "../Notepad/resource.h"

#include <afxwin.h>

#pragma warning(disable:4996)

CNTCommand::CNTCommand(TextEditingForm* textEditingForm) {
	this->textEditingForm = textEditingForm;
}

CNTCommand::CNTCommand(const CNTCommand& source) {
	this->textEditingForm = source.textEditingForm;
}

CNTCommand::~CNTCommand() {

}

CNTCommand& CNTCommand::operator=(const CNTCommand& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

void CNTCommand::Unexecute() {

}

Long CNTCommand::Add(CNTCommand* command) {
	return -1;
}

Long CNTCommand::Remove(Long index) {
	return -1;
}

CNTCommand* CNTCommand::GetAt(Long index) {
	return 0;
}

Long CNTCommand::GetCapacity() const {
	return 0;
}

Long CNTCommand::GetLength() const {
	return -1;
}

Long CNTCommand::Write(CNTCommand* command) {
	return -1;
}

Long CNTCommand::Erase() {
	return -1;
}

CNTCommand* CNTCommand::OpenAt() {
	return 0;
}

void CNTCommand::Empty() {

}

bool CNTCommand::IsEmpty() {
	return false;
}

Long CNTCommand::GetTop() const {
	return -1;
}

Long CNTCommand::GetWidth() const {
	return 0;
}
//////////////////// Composite ////////////////////
//CNTMacroCommand
CNTMacroCommand::CNTMacroCommand(TextEditingForm* textEditingForm, Long capacity)
	: CNTCommand(textEditingForm), commands(10) {
	this->capacity = 10;
	this->length = 0;
}

CNTMacroCommand::CNTMacroCommand(const CNTMacroCommand& source)
	: CNTCommand(source.textEditingForm), commands(source.capacity) {
	CNTCommand* command;
	Long i = 0;
	while (i < source.length) {
		command = const_cast<CNTMacroCommand&>(source).commands[i]->Clone();
		this->commands.Store(i, command);
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

CNTMacroCommand::~CNTMacroCommand() {
	Long i = 0;
	while (i < this->length) {
		if (this->commands[i] != 0) {
			delete this->commands[i];
		}
		i++;
	}
}

CNTMacroCommand& CNTMacroCommand::operator=(const CNTMacroCommand& source) {
	CNTCommand::operator=(source);
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
		this->commands.Modify(i, const_cast<CNTMacroCommand&>(source).commands[i]->Clone());
		i++;
	}

	this->length = source.length;

	return *this;
}

void CNTMacroCommand::Execute() {
	Long i = 0;
	while (i < this->length) {
		this->commands[i]->Execute();
		i++;
	}
}

void CNTMacroCommand::Unexecute() {
	Long i = this->length - 1;
	while (i >= 0) {
		this->commands[i]->Unexecute();
		i--;
	}
}

Long CNTMacroCommand::Add(CNTCommand* command) {
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

Long CNTMacroCommand::Remove(Long index) {
	if (this->commands[index] != 0) {
		delete this->commands.GetAt(index);
	}
	index = this->commands.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

CNTCommand* CNTMacroCommand::GetAt(Long index) {
	return this->commands.GetAt(index);
}

string CNTMacroCommand::GetType() {
	return "CNTMacro";
}

CNTCommand* CNTMacroCommand::Clone() {
	return new CNTMacroCommand(*this);
}

//CNTSizeCommand
CNTSizeCommand::CNTSizeCommand(TextEditingForm* textEditingForm, Long capacity)
	: CNTCommand(textEditingForm), commands(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->top = 0;
	this->width = 0;
}

CNTSizeCommand::CNTSizeCommand(const CNTSizeCommand& source)
	: CNTCommand(source.textEditingForm), commands(source.capacity) {

	Stack<CNTCommand*> tempForSource(source.commands);
	Stack<CNTCommand*> temp(source.capacity);
	Long i = 0;
	while (i < source.length) {
		temp.Push(tempForSource.Top()->Clone());
		tempForSource.Pop();
		i++;
	}

	i = 0;
	while (i < source.length) {
		this->commands.Push(temp.Top());
		temp.Pop();
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->top = source.top;
	this->width = source.width;
}

CNTSizeCommand::~CNTSizeCommand() {
	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}
}

CNTSizeCommand& CNTSizeCommand::operator=(const CNTSizeCommand& source) {
	CNTCommand::operator=(source);

	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}

	this->commands = source.commands;

	Stack<CNTCommand*> temp(source.capacity);
	i = 0;
	while (i < source.length) {
		temp.Push(this->commands.Top()->Clone());
		this->commands.Pop();
		i++;
	}

	i = 0;
	while (i < source.length) {
		this->commands.Push(temp.Top());
		temp.Pop();
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->top = source.top;

	this->width = source.width;

	return *this;
}

void CNTSizeCommand::Execute() {
	if (this->width == 0) {
		this->width = this->textEditingForm->GetSizedWidth();
	}

	CRect rect;
	this->textEditingForm->GetClientRect(rect);

	if (this->width != rect.Width()) { // 최초 Execute 는 의미 없음. 어차피 현재 너비로 재배열하는 것임.
		DummyManager dummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, rect.Width());

		Long start;
		Long startColumn;
		Long end;
		Long endColumn;
		Long startDistance;
		Long endDistance;
		if (this->textEditingForm->selection != NULL) {
			start = this->textEditingForm->selection->GetStart();
			end = this->textEditingForm->selection->GetEnd();
			startColumn = this->textEditingForm->note->GetSelectedStartColumn(start);
			endColumn = this->textEditingForm->note->GetSelectedEndColumn(end);
			startDistance = dummyManager.CountDistance(start, startColumn);
			endDistance = dummyManager.CountDistance(end, endColumn);
		}

		Long row = this->textEditingForm->note->GetCurrent();
		Long column = this->textEditingForm->current->GetCurrent();
		Long distance = dummyManager.CountDistance(row, column);
		Long i = 0;
		while (i < this->textEditingForm->note->GetLength()) {
			dummyManager.Unfold(i);
			i++;
		}
		i = 0;
		while (i < this->textEditingForm->note->GetLength()) {
			i = dummyManager.Fold(i);
			i++;
		}
		dummyManager.CountIndex(distance, &row, &column);
		this->textEditingForm->note->Move(row);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		this->textEditingForm->current->Move(column);

		if (this->textEditingForm->selection != NULL) {
			delete this->textEditingForm->selection;
			dummyManager.CountIndex(startDistance, &start, &startColumn);
			dummyManager.CountIndex(endDistance, &end, &endColumn);
			this->textEditingForm->selection = new Selection(start, end);
		}
	}
}

void CNTSizeCommand::Unexecute() {
	CRect rect;
	this->textEditingForm->GetClientRect(rect);

	if (this->width != rect.Width()) {
		DummyManager dummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, this->width);

		Long start;
		Long startColumn;
		Long end;
		Long endColumn;
		Long startDistance;
		Long endDistance;
		if (this->textEditingForm->selection != NULL) {
			start = this->textEditingForm->selection->GetStart();
			end = this->textEditingForm->selection->GetEnd();
			startColumn = this->textEditingForm->note->GetSelectedStartColumn(start);
			endColumn = this->textEditingForm->note->GetSelectedEndColumn(end);
			startDistance = dummyManager.CountDistance(start, startColumn);
			endDistance = dummyManager.CountDistance(end, endColumn);
		}

		Long row = this->textEditingForm->note->GetCurrent();
		Long column = this->textEditingForm->current->GetCurrent();
		Long distance = dummyManager.CountDistance(row, column);
		Long i = 0;
		while (i < this->textEditingForm->note->GetLength()) {
			dummyManager.Unfold(i);
			i++;
		}
		i = 0;
		while (i < this->textEditingForm->note->GetLength()) {
			i = dummyManager.Fold(i);
			i++;
		}
		dummyManager.CountIndex(distance, &row, &column);
		this->textEditingForm->note->Move(row);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		this->textEditingForm->current->Move(column);

		if (this->textEditingForm->selection != NULL) {
			delete this->textEditingForm->selection;
			dummyManager.CountIndex(startDistance, &start, &startColumn);
			dummyManager.CountIndex(endDistance, &end, &endColumn);
			this->textEditingForm->selection = new Selection(start, end);
		}
	}
	this->textEditingForm->SetPreviousWidth(this->width);
}

Long CNTSizeCommand::Write(CNTCommand* command) {
	this->top = this->commands.Push(command);
	if (this->length >= this->capacity) {
		this->capacity++;
	}
	this->length++;

	return this->top;
}

Long CNTSizeCommand::Erase() {
	if (this->commands.Top() != 0) {
		delete this->commands.Top();
	}

	this->commands.Pop();
	this->length--;
	this->top--;

	return -1;
}

CNTCommand* CNTSizeCommand::OpenAt() {
	return this->commands.Top();
}

void CNTSizeCommand::Empty() {
	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}
	this->length = 0;
	this->top = 0;
}

bool CNTSizeCommand::IsEmpty() {
	return this->commands.IsEmpty();
}

string CNTSizeCommand::GetType() {
	return "CNTSize";
}

CNTCommand* CNTSizeCommand::Clone() {
	return new CNTSizeCommand(*this);
}
//////////////////// Composite ////////////////////

//////////////////// Basic ////////////////////
//CNTWriteBasicCommand
CNTWriteBasicCommand::CNTWriteBasicCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTWriteBasicCommand::CNTWriteBasicCommand(const CNTWriteBasicCommand& source)
	: CNTCommand(source) {

}

CNTWriteBasicCommand::~CNTWriteBasicCommand() {

}

CNTWriteBasicCommand& CNTWriteBasicCommand::operator=(const CNTWriteBasicCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTWriteBasicCommand::Execute() {
	GlyphFactory glyphFactory;
	TCHAR content[2];
	int nChar = this->textEditingForm->GetCurrentCharacter();
	Long row = this->textEditingForm->note->GetCurrent();
	Long column = this->textEditingForm->current->GetCurrent();

	if (nChar >= 32 || nChar == VK_TAB) {
		content[0] = nChar;
		Glyph* character = glyphFactory.Make(content);

		if (column >= this->textEditingForm->current->GetLength()) {
			this->textEditingForm->current->Add(character);
		}
		else {
			this->textEditingForm->current->Add(column, character);
		}
	}
	else if (nChar == VK_RETURN) {
		if (column < this->textEditingForm->current->GetLength()) {
			this->textEditingForm->current = this->textEditingForm->current->Divide(column);
			this->textEditingForm->note->Add(row + 1, this->textEditingForm->current);
			this->textEditingForm->current->First();
		}
		else {
			this->textEditingForm->current = glyphFactory.Make("\r\n");
			this->textEditingForm->note->Add(row + 1, this->textEditingForm->current);
		}
	}
}

string CNTWriteBasicCommand::GetType() {
	return "CNTWriteBasic";
}

CNTCommand* CNTWriteBasicCommand::Clone() {
	return new CNTWriteBasicCommand(*this);
}

//CNTImeCompositionBasicCommand
CNTImeCompositionBasicCommand::CNTImeCompositionBasicCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTImeCompositionBasicCommand::CNTImeCompositionBasicCommand(const CNTImeCompositionBasicCommand& source)
	: CNTCommand(source) {

}

CNTImeCompositionBasicCommand::~CNTImeCompositionBasicCommand() {

}

CNTImeCompositionBasicCommand& CNTImeCompositionBasicCommand::operator=(const CNTImeCompositionBasicCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTImeCompositionBasicCommand::Execute() {
	TCHAR(*buffer) = new TCHAR[2];
	buffer = this->textEditingForm->GetCurrentBuffer();

	Long index;

	if (this->textEditingForm->GetIsComposing() == TRUE) {
		index = this->textEditingForm->current->GetCurrent();
		this->textEditingForm->current->Remove(index - 1);
	}

	if (buffer[0] != '\0') {
		this->textEditingForm->SetIsComposing(TRUE);
		GlyphFactory glyphFactory;
		Glyph* doubleByteCharacter = glyphFactory.Make(buffer);
		index = this->textEditingForm->current->GetCurrent();

		if (index >= this->textEditingForm->current->GetLength()) {
			this->textEditingForm->current->Add(doubleByteCharacter);
		}
		else {
			this->textEditingForm->current->Add(index, doubleByteCharacter);
		}
	}
	else {
		this->textEditingForm->SetIsComposing(FALSE);
	}
}

string CNTImeCompositionBasicCommand::GetType() {
	return "CNTImeCompositionBasic";
}

CNTCommand* CNTImeCompositionBasicCommand::Clone() {
	return new CNTImeCompositionBasicCommand(*this);
}

//CNTImeCharBasicCommand
CNTImeCharBasicCommand::CNTImeCharBasicCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTImeCharBasicCommand::CNTImeCharBasicCommand(const CNTImeCharBasicCommand& source)
	: CNTCommand(source) {

}

CNTImeCharBasicCommand::~CNTImeCharBasicCommand() {

}

CNTImeCharBasicCommand& CNTImeCharBasicCommand::operator=(const CNTImeCharBasicCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTImeCharBasicCommand::Execute() {
	TCHAR buffer[2];
	buffer[0] = this->textEditingForm->GetCurrentBuffer()[0];
	buffer[1] = this->textEditingForm->GetCurrentBuffer()[1];
	Long column = this->textEditingForm->current->GetCurrent();

	if (this->textEditingForm->GetIsComposing() == TRUE) {
		this->textEditingForm->current->Remove(column);
	}

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Make(buffer);

	if (column >= this->textEditingForm->current->GetLength()) {
		this->textEditingForm->current->Add(glyph);
	}
	else {
		this->textEditingForm->current->Add(column, glyph);
	}
}

string CNTImeCharBasicCommand::GetType() {
	return "CNTImeCharBasic";
}

CNTCommand* CNTImeCharBasicCommand::Clone() {
	return new CNTImeCharBasicCommand(*this);
}

//CNTDeleteBasicCommand
CNTDeleteBasicCommand::CNTDeleteBasicCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTDeleteBasicCommand::CNTDeleteBasicCommand(const CNTDeleteBasicCommand& source)
	: CNTCommand(source) {

}

CNTDeleteBasicCommand::~CNTDeleteBasicCommand() {

}

CNTDeleteBasicCommand& CNTDeleteBasicCommand::operator=(const CNTDeleteBasicCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTDeleteBasicCommand::Execute() {
	Long row = this->textEditingForm->note->GetCurrent();
	Long column = this->textEditingForm->current->GetCurrent();
	Long noteLength = this->textEditingForm->note->GetLength();
	Long lineLength = this->textEditingForm->current->GetLength();

	if (column < lineLength) {
		this->textEditingForm->current->Remove(column);
	}
	else if (column >= lineLength && row < noteLength - 1) {
		Glyph* other = this->textEditingForm->note->GetAt(row + 1);
		this->textEditingForm->current->Combine(other);
		this->textEditingForm->note->Remove(row + 1);
	}
}

string CNTDeleteBasicCommand::GetType() {
	return "DeleteBasic";
}

CNTCommand* CNTDeleteBasicCommand::Clone() {
	return new CNTDeleteBasicCommand(*this);
}

//CNTCopyBasicCommand
CNTCopyBasicCommand::CNTCopyBasicCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTCopyBasicCommand::CNTCopyBasicCommand(const CNTCopyBasicCommand& source)
	: CNTCommand(source) {

}

CNTCopyBasicCommand::~CNTCopyBasicCommand() {

}

CNTCopyBasicCommand& CNTCopyBasicCommand::operator=(const CNTCopyBasicCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTCopyBasicCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		Long start = this->textEditingForm->selection->GetStart();
		Long end = this->textEditingForm->selection->GetEnd();
		CString clipBoard;
		string content;
		string characterContent;
		Glyph* line;
		Glyph* character;
		Long column = 0;
		Long j;
		Long i = start;
		while (i <= end) {
			content = "";
			line = this->textEditingForm->note->GetAt(i);
			j = 0;
			while (j < line->GetLength()) {
				character = line->GetAt(j);
				if (character->GetIsSelected()) {
					column = j + 1;
					characterContent = character->GetContent();
					if (characterContent == "        ") {
						characterContent = '\t';
					}
					content += characterContent;
				}
				j++;
			}

			if (column >= line->GetLength() && i < end) {
				content.append("\r\n");
			}
			clipBoard.Append(content.c_str());
			i++;
		}

		HANDLE handle = 0;
		char* address = NULL;
		handle = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, clipBoard.GetLength() + 1);
		address = (char*)::GlobalLock(handle);
		if (address == NULL) {
			::GlobalFree(handle);
		}
		else {
			strcpy(address, clipBoard);
		}
		if (::OpenClipboard(this->textEditingForm->m_hWnd)) {
			::EmptyClipboard();
			::SetClipboardData(CF_TEXT, handle);
			::CloseClipboard();
		}
		::GlobalUnlock(handle);
	}
}

string CNTCopyBasicCommand::GetType() {
	return "CNTCopyBasic";
}

CNTCommand* CNTCopyBasicCommand::Clone() {
	return new CNTCopyBasicCommand(*this);
}

//CNTDeleteSelectionBasicCommand
CNTDeleteSelectionBasicCommand::CNTDeleteSelectionBasicCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTDeleteSelectionBasicCommand::CNTDeleteSelectionBasicCommand(const CNTDeleteSelectionBasicCommand& source)
	: CNTCommand(source) {

}

CNTDeleteSelectionBasicCommand::~CNTDeleteSelectionBasicCommand() {

}

CNTDeleteSelectionBasicCommand& CNTDeleteSelectionBasicCommand::operator=(const CNTDeleteSelectionBasicCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTDeleteSelectionBasicCommand::Execute() {
	Long i;
	Long start = this->textEditingForm->selection->GetStart();
	Long end = this->textEditingForm->selection->GetEnd();
	Glyph* character;
	Glyph* line;
	Long j;
	i = start;
	while (i <= end) {
		line = this->textEditingForm->note->GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			if (character->GetIsSelected()) {
				line->Remove(j--);
			}
			j++;
		}
		if (j >= line->GetLength() && i < end) {
			this->textEditingForm->current = line->Combine(this->textEditingForm->note->GetAt(i + 1));
			this->textEditingForm->note->Remove(i + 1);
			i--;
			end--;
		}
		i++;
	}

	this->textEditingForm->note->Move(start);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(start);

	if (this->textEditingForm->selection != NULL) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
		this->textEditingForm->note->UnselectAll();
	}
}

string CNTDeleteSelectionBasicCommand::GetType() {
	return "CNTDeleteSelectionBasic";
}

CNTCommand* CNTDeleteSelectionBasicCommand::Clone() {
	return new CNTDeleteSelectionBasicCommand(*this);
}
//////////////////// Basic ////////////////////

//////////////////// Main ////////////////////
//CNTWriteCommand
CNTWriteCommand::CNTWriteCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
	this->nChar = -1;
	this->row = -1;
	this->column = -1;
}

CNTWriteCommand::CNTWriteCommand(const CNTWriteCommand& source)
	: CNTCommand(source) {
	this->nChar = source.nChar;
	this->row = source.row;
	this->column = source.column;
}

CNTWriteCommand::~CNTWriteCommand() {

}

CNTWriteCommand& CNTWriteCommand::operator=(const CNTWriteCommand& source) {
	CNTCommand::operator=(source);
	this->nChar = source.nChar;
	this->row = source.row;
	this->column = source.column;

	return *this;
}

void CNTWriteCommand::Execute() {
	if (this->nChar == -1 && this->row == -1 && this->column == -1) {
		this->nChar = this->textEditingForm->GetCurrentCharacter();
		this->row = this->textEditingForm->note->GetCurrent();
		this->column = this->textEditingForm->current->GetCurrent();
	}
	else {
		this->textEditingForm->SetCurrentCharacter(this->nChar);
	}

	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		distance = dummyManager->CountDistance(this->row, this->column);
		this->row = dummyManager->Unfold(this->row);
		dummyManager->CountIndex(distance, &this->row, &this->column);
	}

	this->textEditingForm->note->Move(this->row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->row);
	this->textEditingForm->current->Move(this->column);
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_BASIC_WRITE, 0));

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		Long lastFoldedRow = dummyManager->Fold(this->row);
		if (this->nChar == VK_RETURN) {
			dummyManager->Fold(lastFoldedRow + 1);
		}
		dummyManager->CountIndex(distance + 1, &rowIndex, &columnIndex);

		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	if (this->nChar >= 32 || this->nChar == VK_TAB || this->nChar == VK_RETURN) {
		NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
		notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
	}
}

void CNTWriteCommand::Unexecute() {
	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		Long unfoldedRow = dummyManager->Unfold(this->row);
		if (this->nChar == VK_RETURN) {
			dummyManager->Unfold(unfoldedRow + 1);
		}
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->note->Move(this->row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->row);
	this->textEditingForm->current->Move(this->column);

	Glyph* index;
	if (this->nChar >= 32 || this->nChar == VK_TAB) {
		this->textEditingForm->current->Remove(this->column);
	}
	else if (this->nChar == VK_RETURN) {
		index = this->textEditingForm->note->GetAt(this->textEditingForm->note->GetCurrent() + 1);
		this->textEditingForm->current->Combine(index);
		this->textEditingForm->note->Remove(this->textEditingForm->note->GetCurrent() + 1);
		this->textEditingForm->current->Move(this->column);
	}
	if (this->textEditingForm->selection != NULL) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
		this->textEditingForm->note->UnselectAll();
	}

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		dummyManager->Fold(this->row);
		distance = dummyManager->CountDistance(this->row, this->column);
		dummyManager->CountIndex(distance, &rowIndex, &columnIndex);
		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

string CNTWriteCommand::GetType() {
	return "CNTWrite";
}

CNTCommand* CNTWriteCommand::Clone() {
	return new CNTWriteCommand(*this);
}

//CNTImeCompositionCommand
CNTImeCompositionCommand::CNTImeCompositionCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTImeCompositionCommand::CNTImeCompositionCommand(const CNTImeCompositionCommand& source)
	: CNTCommand(source) {

}

CNTImeCompositionCommand::~CNTImeCompositionCommand() {

}

CNTImeCompositionCommand& CNTImeCompositionCommand::operator=(const CNTImeCompositionCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTImeCompositionCommand::Execute() {
	TCHAR(*buffer) = new TCHAR[2];
	buffer = this->textEditingForm->GetCurrentBuffer();

	Long row = this->textEditingForm->note->GetCurrent();
	Long column = this->textEditingForm->current->GetCurrent();
	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance = 0;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		CRect rect;
		this->textEditingForm->GetClientRect(rect);
		//Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, rect.Width());

		distance = dummyManager->CountDistance(row, column);
		row = dummyManager->Unfold(row);
		dummyManager->CountIndex(distance, &row, &column);
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->note->Move(row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
	this->textEditingForm->current->Move(column);

	if (this->textEditingForm->GetIsComposing() == TRUE) {
		distance--; //자동개행 추가
	}

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_BASIC_IMECOMPOSITION, 0));

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		dummyManager->Fold(row);
		dummyManager->CountIndex(distance + 1, &rowIndex, &columnIndex);
		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

string CNTImeCompositionCommand::GetType() {
	return "CNTImeComposition";
}

CNTCommand* CNTImeCompositionCommand::Clone() {
	return new CNTImeCompositionCommand(*this);
}

//CNTImeCharCommand
CNTImeCharCommand::CNTImeCharCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
	this->buffer = new TCHAR[2];
	this->buffer[0] = '\0';
	this->buffer[1] = '\0';
	this->row = -1;
	this->column = -1;
}

CNTImeCharCommand::CNTImeCharCommand(const CNTImeCharCommand& source)
	: CNTCommand(source) {
	this->buffer = new TCHAR[2];
	this->buffer[0] = source.buffer[0];
	this->buffer[1] = source.buffer[1];
	this->row = source.row;
	this->column = source.column;
}

CNTImeCharCommand::~CNTImeCharCommand() {
	if (this->buffer != 0) {
		delete[] this->buffer;
	}
}

CNTImeCharCommand& CNTImeCharCommand::operator=(const CNTImeCharCommand& source) {
	CNTCommand::operator=(source);
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

void CNTImeCharCommand::Execute() {
	bool needtoRemove = false;
	if (this->buffer[0] == '\0' && this->row == -1 && this->column == -1) {
		this->buffer[0] = this->textEditingForm->GetCurrentBuffer()[0];
		this->buffer[1] = this->textEditingForm->GetCurrentBuffer()[1];
		this->row = this->textEditingForm->note->GetCurrent();
		this->column = this->textEditingForm->current->GetCurrent();
		if (this->textEditingForm->GetIsComposing() == TRUE) {
			needtoRemove = true;
		}
	}
	else {
		this->textEditingForm->SetCurrentBuffer(this->buffer);
	}

	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance = 0;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		distance = dummyManager->CountDistance(this->row, this->column);
		this->row = dummyManager->Unfold(this->row);
		dummyManager->CountIndex(distance, &this->row, &this->column);
	}
	//========== 자동 개행 처리 1 ==========

	if (needtoRemove == true) {
		this->column--;
	}
	else {
		distance++;
	}

	this->textEditingForm->note->Move(this->row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->row);
	this->textEditingForm->current->Move(this->column);

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_BASIC_IMECHAR, 0));

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		dummyManager->Fold(this->row);
		dummyManager->CountIndex(distance, &rowIndex, &columnIndex);
		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

void CNTImeCharCommand::Unexecute() {
	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		dummyManager->Unfold(this->row);
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->note->Move(this->row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->row);
	this->textEditingForm->current->Move(this->column);

	this->textEditingForm->current->Remove(this->column);

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		dummyManager->Fold(this->row);
		distance = dummyManager->CountDistance(this->row, this->column);
		dummyManager->CountIndex(distance, &rowIndex, &columnIndex);
		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	if (this->textEditingForm->selection != NULL) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
		this->textEditingForm->note->UnselectAll();
	}

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

string CNTImeCharCommand::GetType() {
	return "CNTImeChar";
}

CNTCommand* CNTImeCharCommand::Clone() {
	return new CNTImeCharCommand(*this);
}

//CNTDeleteCommand
CNTDeleteCommand::CNTDeleteCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
	this->row = -1;
	this->noteLength = -1;
	this->column = -1;
	this->lineLength = -1;
	this->character = 0;
}

CNTDeleteCommand::CNTDeleteCommand(const CNTDeleteCommand& source)
	: CNTCommand(source) {
	this->row = source.row;
	this->noteLength = source.noteLength;
	this->column = source.column;
	this->lineLength = source.lineLength;
	this->character = 0;
	if (source.character != 0) {
		this->character = source.character->Clone();
	}
}

CNTDeleteCommand::~CNTDeleteCommand() {
	if (this->character != NULL) {
		delete this->character;
	}
}

CNTDeleteCommand& CNTDeleteCommand::operator=(const CNTDeleteCommand& source) {
	CNTCommand::operator=(source);
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

void CNTDeleteCommand::Execute() {
	if (this->row == -1 && this->noteLength == -1 && this->column == -1 && this->lineLength == -1 && this->character == 0) {
		this->row = this->textEditingForm->note->GetCurrent();
		this->noteLength = this->textEditingForm->note->GetLength();
		this->column = this->textEditingForm->current->GetCurrent();
		this->lineLength = this->textEditingForm->current->GetLength();
		this->character = 0;
		if (this->column < this->textEditingForm->current->GetLength()) {
			this->character = this->textEditingForm->current->GetAt(this->column)->Clone();
		}
	}

	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		distance = dummyManager->CountDistance(this->row, this->column);
		this->row = dummyManager->Unfold(this->row);
		dummyManager->CountIndex(distance, &this->row, &this->column);
		if (this->character == 0 && this->column >= this->textEditingForm->note->GetAt(this->row)->GetLength()
			&& this->row + 1 < this->textEditingForm->note->GetLength() - 1) {
			dummyManager->Unfold(this->row + 1);
		}
		this->noteLength = this->textEditingForm->note->GetLength();
		this->lineLength = this->textEditingForm->note->GetAt(this->row)->GetLength();
		if (this->column < this->lineLength) {
			this->character = this->textEditingForm->note->GetAt(this->row)->GetAt(this->column)->Clone();
		}
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->note->Move(this->row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->row);
	this->textEditingForm->current->Move(this->column);

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_BASIC_DELETE, 0));

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		dummyManager->Fold(this->row);
		dummyManager->CountIndex(distance, &rowIndex, &columnIndex);
		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

void CNTDeleteCommand::Unexecute() {
	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		dummyManager->Unfold(this->row);
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->note->Move(this->row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->row);
	this->textEditingForm->current->Move(this->column);

	Glyph* line;
	if (this->column < this->lineLength) {
		this->character->Select(false);
		this->textEditingForm->current->Add(this->column, this->character->Clone());
	}
	else if (this->column >= this->lineLength && this->row < this->noteLength - 1) {
		if (this->character == 0) {
			line = this->textEditingForm->current->Divide(this->column);
			this->textEditingForm->note->Add(this->row + 1, line);
		}
		else {
			this->character->Select(false);
			this->textEditingForm->current->Add(this->column, this->character->Clone());
		}
	}
	this->textEditingForm->note->Move(this->row);
	this->textEditingForm->current->Move(this->column);

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		Long lastFoldedRow = dummyManager->Fold(this->row);
		if (this->character == 0) {
			dummyManager->Fold(lastFoldedRow + 1);
		}

		distance = dummyManager->CountDistance(this->row, this->column);
		dummyManager->CountIndex(distance, &rowIndex, &columnIndex);
		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

string CNTDeleteCommand::GetType() {
	return "CNTDelete";
}

CNTCommand* CNTDeleteCommand::Clone() {
	return new CNTDeleteCommand(*this);
}

//CNTCopyCommand
CNTCopyCommand::CNTCopyCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTCopyCommand::CNTCopyCommand(const CNTCopyCommand& source)
	: CNTCommand(source) {
}

CNTCopyCommand::~CNTCopyCommand() {

}

CNTCopyCommand& CNTCopyCommand::operator=(const CNTCopyCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTCopyCommand::Execute() {
	Long row = this->textEditingForm->note->GetCurrent();
	Long column = this->textEditingForm->current->GetCurrent();
	Long originStart = this->textEditingForm->selection->GetStart();
	Long originEnd = this->textEditingForm->selection->GetEnd();
	Long start = originStart;
	Long end = originEnd;
	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		CRect rect;
		this->textEditingForm->GetClientRect(rect);
		//Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, rect.Width());
		dummyManager->Unfold(&start, &end);

		if (this->textEditingForm->selection != NULL) {
			delete this->textEditingForm->selection;
			this->textEditingForm->selection = NULL;
		}
		this->textEditingForm->selection = new Selection(start, end);
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_BASIC_COPY, 0));

	//========== 자동 개행 처리 2 ==========
	if (dummyManager != NULL) {
		Long lastFoldedRow;
		Long i = start;
		while (i <= end && end < this->textEditingForm->note->GetLength()) {
			lastFoldedRow = dummyManager->Fold(i);
			end += lastFoldedRow - i;
			i = lastFoldedRow + 1;
		}
		delete dummyManager;

		if (this->textEditingForm->selection != NULL) {
			delete this->textEditingForm->selection;
			this->textEditingForm->selection = NULL;
		}
		this->textEditingForm->selection = new Selection(originStart, originEnd);

		this->textEditingForm->note->Move(row);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		this->textEditingForm->current->Move(column);
	}
	//========== 자동 개행 처리 2 ==========
}

string CNTCopyCommand::GetType() {
	return "CNTCopy";
}

CNTCommand* CNTCopyCommand::Clone() {
	return new CNTCopyCommand(*this);
}

//CNTDeleteSelectionCommand
CNTDeleteSelectionCommand::CNTDeleteSelectionCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
	this->startRow = -1;
	this->startColumn = -1;
	this->endRow = -1;
	this->endColumn = -1;
	this->selecteds = "";
}

CNTDeleteSelectionCommand::CNTDeleteSelectionCommand(const CNTDeleteSelectionCommand& source)
	: CNTCommand(source) {
	this->startRow = source.startRow;
	this->startColumn = source.startColumn;
	this->endRow = source.endRow;
	this->endColumn = source.endColumn;
	this->selecteds = source.selecteds;
}

CNTDeleteSelectionCommand::~CNTDeleteSelectionCommand() {

}

CNTDeleteSelectionCommand& CNTDeleteSelectionCommand::operator=(const CNTDeleteSelectionCommand& source) {
	CNTCommand::operator=(source);
	this->startRow = source.startRow;
	this->startColumn = source.startColumn;
	this->endRow = source.endRow;
	this->endColumn = source.endColumn;
	this->selecteds = source.selecteds;

	return *this;
}

void CNTDeleteSelectionCommand::Execute() {
	//startRow, startColumn, endRow, endColumn, selecteds(string)
	if (this->startRow == -1 && this->startColumn == -1 && this->endRow == -1 && this->endColumn == -1 && this->selecteds == "") {
		this->startRow = this->textEditingForm->selection->GetStart();
		this->endRow = this->textEditingForm->selection->GetEnd();

		this->startColumn = this->textEditingForm->note->GetSelectedStartColumn(this->startRow);
		this->endColumn = this->textEditingForm->note->GetSelectedEndColumn(this->endRow);
		
		this->selecteds = this->textEditingForm->note->GetContent(this->startRow, this->startColumn, this->endRow, this->endColumn);
	}

	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long startDistance;
	Long endDistance;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		startDistance = dummyManager->CountDistance(this->startRow, this->startColumn);
		endDistance = dummyManager->CountDistance(this->endRow, this->endColumn);
		dummyManager->Unfold(&this->startRow, &this->endRow);
		dummyManager->CountIndex(startDistance, &this->startRow, &this->startColumn); //reexecute에서는 필요없음.
		dummyManager->CountIndex(endDistance, &this->endRow, &this->endColumn); //reexecute에서는 필요없음.

		this->selecteds = this->textEditingForm->note->GetContent(this->startRow, this->startColumn, this->endRow, this->endColumn);
	}
	//========== 자동 개행 처리 1 ==========

	if (this->textEditingForm->selection != NULL) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
		this->textEditingForm->note->UnselectAll();
	}

	this->textEditingForm->selection = new Selection(this->startRow, this->endRow);
	this->textEditingForm->note->Select(this->startRow, this->startColumn, this->endRow, this->endColumn);

	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_BASIC_DELETESELECTION, 0));

	//========== 자동 개행 처리 2 ==========
	Long rowIndex;
	Long columnIndex;
	if (dummyManager != NULL) {
		dummyManager->Fold(this->startRow);
		dummyManager->CountIndex(startDistance, &rowIndex, &columnIndex);
		delete dummyManager;

		this->textEditingForm->note->Move(rowIndex);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(rowIndex);
		this->textEditingForm->current->Move(columnIndex);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

void CNTDeleteSelectionCommand::Unexecute() {
	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		dummyManager->Unfold(this->startRow);
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->note->Move(this->startRow);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->startRow);
	this->textEditingForm->current->Move(this->startColumn);

	GlyphFactory glyphFactory;
	Scanner scanner(this->selecteds);
	while (scanner.IsEnd() == false) {
		string token = scanner.GetToken();
		Glyph* glyph = glyphFactory.Make(token.c_str());
		Long row = this->textEditingForm->note->GetCurrent();
		Long column = this->textEditingForm->current->GetCurrent();
		if (token != "\n") {
			if (column >= this->textEditingForm->current->GetLength()) {
				this->textEditingForm->current->Add(glyph);
			}
			else {
				this->textEditingForm->current->Add(column, glyph);
			}
		}
		else {
			if (column < this->textEditingForm->current->GetLength()) {
				this->textEditingForm->current = this->textEditingForm->current->Divide(column);
				this->textEditingForm->note->Add(row + 1, this->textEditingForm->current);
				this->textEditingForm->current->First();
			}
			else {
				this->textEditingForm->current = glyphFactory.Make("\r\n");
				this->textEditingForm->note->Add(row + 1, this->textEditingForm->current);
			}
		}
		scanner.Next();
	}

	//========== 자동 개행 처리 2 ==========
	Long startDistance;
	Long endDistance;
	if (dummyManager != NULL) {
		startDistance = dummyManager->CountDistance(this->startRow, this->startColumn);
		endDistance = dummyManager->CountDistance(this->endRow, this->endColumn);
		Long end = this->endRow;
		Long lastFoldedRow;
		Long i = this->startRow;
		while (i <= end && end < this->textEditingForm->note->GetLength()) {
			lastFoldedRow = dummyManager->Fold(i);
			end += lastFoldedRow - i;
			i = lastFoldedRow + 1;
		}
		dummyManager->CountIndex(startDistance, &this->startRow, &this->startColumn);
		dummyManager->CountIndex(endDistance, &this->endRow, &this->endColumn);
		delete dummyManager;

		if (this->textEditingForm->selection != 0) {
			delete this->textEditingForm->selection;
			this->textEditingForm->selection = 0;
			this->textEditingForm->note->UnselectAll();
		}
	}
	//========== 자동 개행 처리 2 ==========

	this->textEditingForm->note->Select(this->startRow, this->startColumn, this->endRow, this->endColumn);
	this->textEditingForm->selection = new Selection(this->startRow, this->endRow);

	this->textEditingForm->note->Move(this->endRow);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->endRow);
	this->textEditingForm->current->Move(this->endColumn);

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

string CNTDeleteSelectionCommand::GetType() {
	return "CNTDeleteSelection";
}

CNTCommand* CNTDeleteSelectionCommand::Clone() {
	return new CNTDeleteSelectionCommand(*this);
}

//CNTCutCommand
CNTCutCommand::CNTCutCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTCutCommand::CNTCutCommand(const CNTCutCommand& source)
	: CNTCommand(source) {

}

CNTCutCommand::~CNTCutCommand() {

}

CNTCutCommand& CNTCutCommand::operator=(const CNTCutCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTCutCommand::Execute() {
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_COPY, 0));
	this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
}

string CNTCutCommand::GetType() {
	return "CNTCut";
}

CNTCommand* CNTCutCommand::Clone() {
	return new CNTCutCommand(*this);
}


//CNTPasteCommand
CNTPasteCommand::CNTPasteCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
	this->startRow = -1;
	this->startColumn = -1;
	this->endRow = -1;
	this->endColumn = -1;
	this->pasteds = "";
}

CNTPasteCommand::CNTPasteCommand(const CNTPasteCommand& source)
	: CNTCommand(source) {
	this->startRow = source.startRow;
	this->startColumn = source.startColumn;
	this->endRow = source.endRow;
	this->endColumn = source.endColumn;
	this->pasteds = source.pasteds;
}

CNTPasteCommand::~CNTPasteCommand() {

}

CNTPasteCommand& CNTPasteCommand::operator=(const CNTPasteCommand& source) {
	CNTCommand::operator=(source);
	this->startRow = source.startRow;
	this->startColumn = source.startColumn;
	this->endRow = source.endRow;
	this->endColumn = source.endColumn;
	this->pasteds = source.pasteds;

	return *this;
}

void CNTPasteCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->SetIsDeleteSelectionByInput(TRUE);
		this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
		this->textEditingForm->SetIsDeleteSelectionByInput(FALSE);
	}

	//정보 저장
	if (this->startRow == -1 && this->startColumn == -1 && this->pasteds == "") {
		this->startRow = this->textEditingForm->note->GetCurrent();
		this->startColumn = this->textEditingForm->current->GetCurrent();
		HANDLE handle;
		LPSTR address = NULL;
		if (::IsClipboardFormatAvailable(CF_TEXT) != FALSE) {
			if (::OpenClipboard(this->textEditingForm->m_hWnd)) {
				handle = GetClipboardData(CF_TEXT);
				if (handle != NULL) {
					address = (LPSTR)::GlobalLock(handle);
					this->pasteds = address;
					::GlobalUnlock(handle);
				}
				CloseClipboard();
			}
		}
	}

	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long distance = 0;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		distance = dummyManager->CountDistance(this->startRow, this->startColumn);
		this->startRow = dummyManager->Unfold(this->startRow);
		dummyManager->CountIndex(distance, &this->startRow, &this->startColumn);
	}
	//========== 자동 개행 처리 1 ==========

	this->textEditingForm->note->Move(this->startRow);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->startRow);
	this->textEditingForm->current->Move(this->startColumn);

	//========== CNTPaste 실제 처리 ==========
	//복사한 문자열을 임시 Note로 만들다.
	Scanner scanner(this->pasteds);
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
	Long current = this->textEditingForm->current->GetCurrent();
	Glyph* line = this->textEditingForm->current->Divide(current);
	//현재 줄의 현재 위치부터 복사한 노트의 첫 번째 줄의 글자를 하나씩 추가한다.
	Glyph* copiedLine = glyphClipBoard->GetAt(i++);
	Long j = 0;
	while (j < copiedLine->GetLength()) {
		this->textEditingForm->current->Add(copiedLine->GetAt(j));
		j++;
	}
	//복사한 노트의 줄 수만큼 반복한다.
	while (i < glyphClipBoard->GetLength()) {
		//복사한 노트의 현재 줄을 가져오다.
		copiedLine = glyphClipBoard->GetAt(i);
		//원래 노트의 현재 위치에 가져온 줄을 추가하다.
		Long noteCurrent = this->textEditingForm->note->GetCurrent();
		this->textEditingForm->note->Add(noteCurrent + 1, copiedLine);
		i++;
	}
	//마지막으로 추가한 줄을 현재 줄로 한다.
	this->textEditingForm->current = this->textEditingForm->note->GetAt(this->textEditingForm->note->GetCurrent());
	//마지막 줄에 아까 캐럿 위치에서 나눈 줄을 이어 붙이다.
	this->textEditingForm->current->Combine(line);

	//정보 저장
	if (this->endRow == -1 && this->endColumn == -1) {
		this->endRow = this->textEditingForm->note->GetCurrent();
		this->endColumn = this->textEditingForm->current->GetCurrent();
	}
	//========== CNTPaste 실제 처리 ==========

	//========== 자동 개행 처리 2 ==========
	if (dummyManager != NULL) {
		distance = dummyManager->CountDistance(this->endRow, this->endColumn); //편 상태
		Long lastFoldedRow;
		Long end = this->endRow;
		Long i = this->startRow;
		while (i <= end) {
			lastFoldedRow = dummyManager->Fold(i);
			end += lastFoldedRow - i;
			i = lastFoldedRow + 1;
		}
		dummyManager->CountIndex(distance, &this->endRow, &this->endColumn); //접은 상태
		delete dummyManager;

		this->textEditingForm->note->Move(this->endRow);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(this->endRow);
		this->textEditingForm->current->Move(this->endColumn);
	}
	//========== 자동 개행 처리 2 ==========

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

void CNTPasteCommand::Unexecute() {
	//========== 자동 개행 처리 1 ==========
	DummyManager* dummyManager = 0;
	Long startDistance;
	Long endDistance;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		//CRect rect;
		//this->textEditingForm->GetClientRect(rect);
		Long width = this->textEditingForm->GetPreviousWidth();
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, width);
		startDistance = dummyManager->CountDistance(this->startRow, this->startColumn);
		endDistance = dummyManager->CountDistance(this->endRow, this->endColumn); //접은 상태
		dummyManager->Unfold(&this->startRow, &this->endRow); //편 상태
		dummyManager->CountIndex(startDistance, &this->startRow, &this->startColumn);
		dummyManager->CountIndex(endDistance, &this->endRow, &this->endColumn); //편 상태
	}
	//========== 자동 개행 처리 1 ==========

	Long i;
	Glyph* line;
	Long length;
	Long j;
	Long end = this->endRow;
	i = this->startRow;
	while (i <= end) {
		line = this->textEditingForm->note->GetAt(i);
		j = this->startColumn;
		length = line->GetLength();
		if (i == end) {
			length = j + this->endColumn;
		}
		while (j < length) {
			line->Remove(j--);
			length--;
			j++;
		}
		if (j >= line->GetLength() && i < end) {
			this->textEditingForm->current = line->Combine(this->textEditingForm->note->GetAt(i + 1));
			this->textEditingForm->note->Remove(i + 1);
			i--;
			end--;
		}
		i++;
	}

	//========== 자동 개행 처리 2 ==========
	if (dummyManager != NULL) {
		dummyManager->Fold(this->startRow);
		dummyManager->CountIndex(startDistance, &this->startRow, &this->startColumn);
		delete dummyManager;

		this->textEditingForm->note->Move(this->startRow);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(this->startRow);
		this->textEditingForm->current->Move(this->startColumn);
	}
	//========== 자동 개행 처리 2 ==========

	if (this->textEditingForm->selection != 0) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = 0;
		this->textEditingForm->note->UnselectAll();
	}

	//Document 처리
	NotepadForm* notepadForm = (NotepadForm*)this->textEditingForm->GetParent();
	notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_REPORT_DIRTY, 0));
}

string CNTPasteCommand::GetType() {
	return "CNTPaste";
}

CNTCommand* CNTPasteCommand::Clone() {
	return new CNTPasteCommand(*this);
}

//CNTSelectAllCommand
CNTSelectAllCommand::CNTSelectAllCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {

}

CNTSelectAllCommand::CNTSelectAllCommand(const CNTSelectAllCommand& source)
	: CNTCommand(source) {

}

CNTSelectAllCommand::~CNTSelectAllCommand() {

}

CNTSelectAllCommand& CNTSelectAllCommand::operator=(const CNTSelectAllCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTSelectAllCommand::Execute() {
	Glyph* line;
	Long j;
	Long i = 0;
	while (i < this->textEditingForm->note->GetLength()) {
		line = this->textEditingForm->note->GetAt(i);
		j = 0;
		while (j < line->GetLength()) {
			line->GetAt(j)->Select(true);
			j++;
		}
		i++;
	}
	Long index = this->textEditingForm->note->Last();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	this->textEditingForm->current->Last();

	if (this->textEditingForm->selection != NULL) {
		delete this->textEditingForm->selection;
	}

	//노트에 내용이 하나도 없으면 선택하지 않는다.
	if (!(this->textEditingForm->note->GetLength() <= 1 && this->textEditingForm->current->GetLength() <= 0)) {
		this->textEditingForm->selection = new Selection(0, this->textEditingForm->note->GetLength() - 1);
	}
}

string CNTSelectAllCommand::GetType() {
	return "CNTSelectAll";
}

CNTCommand* CNTSelectAllCommand::Clone() {
	return new CNTSelectAllCommand(*this);
}

//CNTUndoCommand
CNTUndoCommand::CNTUndoCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTUndoCommand::CNTUndoCommand(const CNTUndoCommand& source)
	: CNTCommand(source) {
}

CNTUndoCommand::~CNTUndoCommand() {

}

CNTUndoCommand& CNTUndoCommand::operator=(const CNTUndoCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTUndoCommand::Execute() {
	if (!this->textEditingForm->undoHistoryBook->IsEmpty()) {
		if (this->textEditingForm->selection != NULL) {
			delete this->textEditingForm->selection;
			this->textEditingForm->selection = NULL;
			this->textEditingForm->note->UnselectAll();
		}

		CNTCommand* undoSizeCommand = this->textEditingForm->undoHistoryBook->OpenAt();
		undoSizeCommand->Unexecute();

		CNTCommand* command = undoSizeCommand->OpenAt();
		command->Unexecute();

		undoSizeCommand->Execute();

		CNTCommand* redoSizeCommand = this->textEditingForm->redoHistoryBook->OpenAt();
		if (!this->textEditingForm->redoHistoryBook->IsEmpty() && undoSizeCommand->GetWidth() == redoSizeCommand->GetWidth()) {
			redoSizeCommand->Write(command->Clone());
		}
		else {
			CNTCommand* newRedoSizeCommand = undoSizeCommand->Clone();
			newRedoSizeCommand->Empty();
			newRedoSizeCommand->Write(command->Clone());
			this->textEditingForm->redoHistoryBook->Write(newRedoSizeCommand);
		}

		undoSizeCommand->Erase();
		if (undoSizeCommand->IsEmpty()) {
			this->textEditingForm->undoHistoryBook->Erase();
		}
	}
}

string CNTUndoCommand::GetType() {
	return "CNTUndo";
}

CNTCommand* CNTUndoCommand::Clone() {
	return new CNTUndoCommand(*this);
}

//CNTRedoCommand
CNTRedoCommand::CNTRedoCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTRedoCommand::CNTRedoCommand(const CNTRedoCommand& source)
	: CNTCommand(source) {
}

CNTRedoCommand::~CNTRedoCommand() {

}

CNTRedoCommand& CNTRedoCommand::operator=(const CNTRedoCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTRedoCommand::Execute() {
	if (this->textEditingForm->redoHistoryBook->GetLength() > 0) {
		CNTCommand* redoSizeCommand = this->textEditingForm->redoHistoryBook->OpenAt();
		redoSizeCommand->Unexecute();

		CNTCommand* command = redoSizeCommand->OpenAt();
		command->Execute();

		redoSizeCommand->Execute();

		CNTCommand* undoSizeCommand = this->textEditingForm->undoHistoryBook->OpenAt();
		if (!this->textEditingForm->undoHistoryBook->IsEmpty() && redoSizeCommand->GetWidth() == undoSizeCommand->GetWidth()) {
			undoSizeCommand->Write(command->Clone());
		}
		else {
			CNTCommand* newUndoSizeCommand = redoSizeCommand->Clone();
			newUndoSizeCommand->Empty();
			newUndoSizeCommand->Write(command->Clone());
			this->textEditingForm->undoHistoryBook->Write(newUndoSizeCommand);
		}

		redoSizeCommand->Erase();
		if (redoSizeCommand->IsEmpty()) {
			this->textEditingForm->redoHistoryBook->Erase();
		}
	}
}

string CNTRedoCommand::GetType() {
	return "CNTRedo";
}

CNTCommand* CNTRedoCommand::Clone() {
	return new CNTRedoCommand(*this);
}

//CNTFindCommand
CNTFindCommand::CNTFindCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTFindCommand::CNTFindCommand(const CNTFindCommand& source)
	: CNTCommand(source) {
}

CNTFindCommand::~CNTFindCommand() {

}

CNTFindCommand& CNTFindCommand::operator=(const CNTFindCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTFindCommand::Execute() {
	if (this->textEditingForm->GetIsUnlockedFindReplaceDialog() == TRUE && this->textEditingForm->findReplaceDialog == NULL) {
		string selectedContent = "";
		if (this->textEditingForm->selection != NULL) {
			selectedContent = textEditingForm->note->GetSelectedContent
			(this->textEditingForm->selection->GetStart(), this->textEditingForm->selection->GetEnd());
		}
		this->textEditingForm->findReplaceDialog = new FindReplaceDialog(TRUE, selectedContent, this->textEditingForm);
		this->textEditingForm->findReplaceDialog->SetActiveWindow();
		this->textEditingForm->findReplaceDialog->ShowWindow(TRUE);
	}
}

string CNTFindCommand::GetType() {
	return "CNTFind";
}

CNTCommand* CNTFindCommand::Clone() {
	return new CNTFindCommand(*this);
}

//CNTReplaceCommand
CNTReplaceCommand::CNTReplaceCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTReplaceCommand::CNTReplaceCommand(const CNTReplaceCommand& source)
	: CNTCommand(source) {
}

CNTReplaceCommand::~CNTReplaceCommand() {

}

CNTReplaceCommand& CNTReplaceCommand::operator=(const CNTReplaceCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTReplaceCommand::Execute() {
	if (this->textEditingForm->GetIsUnlockedFindReplaceDialog() == TRUE && this->textEditingForm->findReplaceDialog == NULL) {
		string selectedContent = "";
		if (this->textEditingForm->selection != NULL) {
			selectedContent = textEditingForm->note->GetSelectedContent
			(this->textEditingForm->selection->GetStart(), this->textEditingForm->selection->GetEnd());
		}
		this->textEditingForm->findReplaceDialog = new FindReplaceDialog(FALSE, selectedContent, this->textEditingForm);
		this->textEditingForm->findReplaceDialog->SetActiveWindow();
		this->textEditingForm->findReplaceDialog->ShowWindow(TRUE);
	}
}

string CNTReplaceCommand::GetType() {
	return "Replace";
}

CNTCommand* CNTReplaceCommand::Clone() {
	return new CNTReplaceCommand(*this);
}
//////////////////// Main ////////////////////

//////////////////// Move ////////////////////
//CNTLeftCommand
CNTLeftCommand::CNTLeftCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTLeftCommand::CNTLeftCommand(const CNTLeftCommand& source)
	: CNTCommand(source) {
}

CNTLeftCommand::~CNTLeftCommand() {

}

CNTLeftCommand& CNTLeftCommand::operator=(const CNTLeftCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTLeftCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		Long start = this->textEditingForm->selection->GetStart();
		this->textEditingForm->note->Move(start);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(start);
		bool isSelected = false;
		Long i = 0;
		while (i < this->textEditingForm->current->GetLength() && isSelected == false) {
			isSelected = this->textEditingForm->current->GetAt(i)->GetIsSelected();
			i++;
		}
		Long startColumn = i - 1;
		if (isSelected == false) {
			startColumn++;
		}
		this->textEditingForm->current->Move(startColumn);

		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	else {
		if (this->textEditingForm->current->GetCurrent() > 0) {
			this->textEditingForm->current->Previous();
		}
		else if (this->textEditingForm->note->GetCurrent() > 0) {
			Long index = this->textEditingForm->note->Previous();
			this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
			this->textEditingForm->current->Last();
			if (dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(index + 1))) {
				this->textEditingForm->current->Previous();
			}
		}
	}
}

string CNTLeftCommand::GetType() {
	return "CNTLeft";
}

CNTCommand* CNTLeftCommand::Clone() {
	return new CNTLeftCommand(*this);
}

//CNTRightCommand
CNTRightCommand::CNTRightCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTRightCommand::CNTRightCommand(const CNTRightCommand& source)
	: CNTCommand(source) {
}

CNTRightCommand::~CNTRightCommand() {

}

CNTRightCommand& CNTRightCommand::operator=(const CNTRightCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTRightCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		Long end = this->textEditingForm->selection->GetEnd();
		this->textEditingForm->note->Move(end);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(end);
		bool isSelected = false;
		Long i = this->textEditingForm->current->GetLength() - 1;
		while (i >= 0 && isSelected == false) {
			isSelected = this->textEditingForm->current->GetAt(i)->GetIsSelected();
			i--;
		}
		Long endColumn = i + 2;
		if (isSelected == false) {
			endColumn--;
		}
		this->textEditingForm->current->Move(endColumn);

		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	else {
		if (this->textEditingForm->current->GetCurrent() < this->textEditingForm->current->GetLength()) {
			this->textEditingForm->current->Next();
		}
		else if (this->textEditingForm->note->GetCurrent() < this->textEditingForm->note->GetLength() - 1) {
			Long index = this->textEditingForm->note->Next();
			this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
			this->textEditingForm->current->First();
			if (dynamic_cast<DummyLine*>(this->textEditingForm->current)) {
				this->textEditingForm->current->Next();
			}
		}
	}
}

string CNTRightCommand::GetType() {
	return "CNTRight";
}

CNTCommand* CNTRightCommand::Clone() {
	return new CNTRightCommand(*this);
}

//CNTUpCommand
CNTUpCommand::CNTUpCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTUpCommand::CNTUpCommand(const CNTUpCommand& source)
	: CNTCommand(source) {
}

CNTUpCommand::~CNTUpCommand() {

}

CNTUpCommand& CNTUpCommand::operator=(const CNTUpCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTUpCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	if (this->textEditingForm->note->GetCurrent() > 0) {
		Long x = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current);
		Long index = this->textEditingForm->note->Previous();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
		Long column = this->textEditingForm->characterMetrics->GetColumn(this->textEditingForm->current, x);
		this->textEditingForm->current->Move(column);
	}
}

string CNTUpCommand::GetType() {
	return "CNTUp";
}

CNTCommand* CNTUpCommand::Clone() {
	return new CNTUpCommand(*this);
}

//CNTDownCommand
CNTDownCommand::CNTDownCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTDownCommand::CNTDownCommand(const CNTDownCommand& source)
	: CNTCommand(source) {
}

CNTDownCommand::~CNTDownCommand() {

}

CNTDownCommand& CNTDownCommand::operator=(const CNTDownCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTDownCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	if (this->textEditingForm->note->GetCurrent() < this->textEditingForm->note->GetLength() - 1) {
		Long x = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current);
		Long index = this->textEditingForm->note->Next();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
		Long column = this->textEditingForm->characterMetrics->GetColumn(this->textEditingForm->current, x);
		this->textEditingForm->current->Move(column);
	}
}

string CNTDownCommand::GetType() {
	return "CNTDown";
}

CNTCommand* CNTDownCommand::Clone() {
	return new CNTDownCommand(*this);
}

//CNTHomeCommand
CNTHomeCommand::CNTHomeCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTHomeCommand::CNTHomeCommand(const CNTHomeCommand& source)
	: CNTCommand(source) {
}

CNTHomeCommand::~CNTHomeCommand() {

}

CNTHomeCommand& CNTHomeCommand::operator=(const CNTHomeCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTHomeCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	Long current = this->textEditingForm->current->GetCurrent();
	Long index = this->textEditingForm->current->First();
	Long row = this->textEditingForm->note->GetCurrent();
	if (current == index
		&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(row))) {
		row = this->textEditingForm->note->Previous();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		this->textEditingForm->current->First();
	}
}

string CNTHomeCommand::GetType() {
	return "CNTHome";
}

CNTCommand* CNTHomeCommand::Clone() {
	return new CNTHomeCommand(*this);
}

//CNTEndCommand
CNTEndCommand::CNTEndCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTEndCommand::CNTEndCommand(const CNTEndCommand& source)
	: CNTCommand(source) {
}

CNTEndCommand::~CNTEndCommand() {

}

CNTEndCommand& CNTEndCommand::operator=(const CNTEndCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTEndCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	Long current = this->textEditingForm->current->GetCurrent();
	Long index = this->textEditingForm->current->Last();
	Long row = this->textEditingForm->note->GetCurrent();
	if (current == index
		&& row + 1 < this->textEditingForm->note->GetLength()
		&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(row + 1))) {
		row = this->textEditingForm->note->Next();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		this->textEditingForm->current->Last();
	}
}

string CNTEndCommand::GetType() {
	return "CNTEnd";
}

CNTCommand* CNTEndCommand::Clone() {
	return new CNTEndCommand(*this);
}

//CNTCtrlLeftCommand
CNTCtrlLeftCommand::CNTCtrlLeftCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTCtrlLeftCommand::CNTCtrlLeftCommand(const CNTCtrlLeftCommand& source)
	: CNTCommand(source) {
}

CNTCtrlLeftCommand::~CNTCtrlLeftCommand() {

}

CNTCtrlLeftCommand& CNTCtrlLeftCommand::operator=(const CNTCtrlLeftCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTCtrlLeftCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	Long index = this->textEditingForm->note->MovePreviousWord();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	while (index >= 0
		&& (this->textEditingForm->current->GetCurrent() <= 0
			&& dynamic_cast<DummyLine*>(this->textEditingForm->current))
		|| (this->textEditingForm->current->GetCurrent() >= this->textEditingForm->current->GetLength()
			&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(index + 1)))) {
		index = this->textEditingForm->note->MovePreviousWord();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	}
}

string CNTCtrlLeftCommand::GetType() {
	return "CNTCtrlLeft";
}

CNTCommand* CNTCtrlLeftCommand::Clone() {
	return new CNTCtrlLeftCommand(*this);
}

//CNTCtrlRightCommand
CNTCtrlRightCommand::CNTCtrlRightCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTCtrlRightCommand::CNTCtrlRightCommand(const CNTCtrlRightCommand& source)
	: CNTCommand(source) {
}

CNTCtrlRightCommand::~CNTCtrlRightCommand() {

}

CNTCtrlRightCommand& CNTCtrlRightCommand::operator=(const CNTCtrlRightCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTCtrlRightCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	Long index = this->textEditingForm->note->MoveNextWord();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	while (index + 1 < this->textEditingForm->note->GetLength()
		&& (this->textEditingForm->current->GetCurrent() >= this->textEditingForm->current->GetLength()
			&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(index + 1)))
		|| (this->textEditingForm->current->GetCurrent() <= 0
			&& dynamic_cast<DummyLine*>(this->textEditingForm->current))) {
		index = this->textEditingForm->note->MoveNextWord();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	}
}

string CNTCtrlRightCommand::GetType() {
	return "CNTCtrlRight";
}

CNTCommand* CNTCtrlRightCommand::Clone() {
	return new CNTCtrlRightCommand(*this);
}

//CNTCtrlHomeCommand
CNTCtrlHomeCommand::CNTCtrlHomeCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTCtrlHomeCommand::CNTCtrlHomeCommand(const CNTCtrlHomeCommand& source)
	: CNTCommand(source) {
}

CNTCtrlHomeCommand::~CNTCtrlHomeCommand() {

}

CNTCtrlHomeCommand& CNTCtrlHomeCommand::operator=(const CNTCtrlHomeCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTCtrlHomeCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	Long index = this->textEditingForm->note->First();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	this->textEditingForm->current->First();
}

string CNTCtrlHomeCommand::GetType() {
	return "CNTCtrlHome";
}

CNTCommand* CNTCtrlHomeCommand::Clone() {
	return new CNTCtrlHomeCommand(*this);
}

//CNTCtrlEndCommand
CNTCtrlEndCommand::CNTCtrlEndCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTCtrlEndCommand::CNTCtrlEndCommand(const CNTCtrlEndCommand& source)
	: CNTCommand(source) {
}

CNTCtrlEndCommand::~CNTCtrlEndCommand() {

}

CNTCtrlEndCommand& CNTCtrlEndCommand::operator=(const CNTCtrlEndCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTCtrlEndCommand::Execute() {
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->note->UnselectAll();
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	Long index = this->textEditingForm->note->Last();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	this->textEditingForm->current->Last();
}

string CNTCtrlEndCommand::GetType() {
	return "CNTCtrlEnd";
}

CNTCommand* CNTCtrlEndCommand::Clone() {
	return new CNTCtrlEndCommand(*this);
}

//CNTPageUpCommand
CNTPageUpCommand::CNTPageUpCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTPageUpCommand::CNTPageUpCommand(const CNTPageUpCommand& source)
	: CNTCommand(source) {
}

CNTPageUpCommand::~CNTPageUpCommand() {

}

CNTPageUpCommand& CNTPageUpCommand::operator=(const CNTPageUpCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTPageUpCommand::Execute() {
	Long position = this->textEditingForm->scrollController->PageUp();
	Long previous = this->textEditingForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_VERT);
	this->textEditingForm->scrollController->MoveVerticalScroll(position);
	this->textEditingForm->ScrollWindow(0, previous - position);
	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다. 따라서 현재 x값을 다시 더한다.
	Long x = this->textEditingForm->caretController->GetCaretX() +
		this->textEditingForm->scrollController->GetHorizontalScroll()->GetPosition();
	// Caret의 x값은 노트의 위치에 스크롤 위치를 뺀 상태에 값이 들어가 있다.따라서 이전 y값을 다시 더한다.
	Long y = this->textEditingForm->caretController->GetCaretY() + previous;

	Long row = this->textEditingForm->characterMetrics->GetRow(y - (previous - position));
	if (row < 0) {
		row = 0;
	}
	Long index = this->textEditingForm->note->Move(row); // row가 벗어나는 경우는 없나?
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	Long column = this->textEditingForm->characterMetrics->GetColumn(this->textEditingForm->current, x);
	this->textEditingForm->current->Move(column);
}

string CNTPageUpCommand::GetType() {
	return "CNTPageUp";
}

CNTCommand* CNTPageUpCommand::Clone() {
	return new CNTPageUpCommand(*this);
}

//CNTPageDownCommand
CNTPageDownCommand::CNTPageDownCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTPageDownCommand::CNTPageDownCommand(const CNTPageDownCommand& source)
	: CNTCommand(source) {
}

CNTPageDownCommand::~CNTPageDownCommand() {

}

CNTPageDownCommand& CNTPageDownCommand::operator=(const CNTPageDownCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTPageDownCommand::Execute() {
	Long position = this->textEditingForm->scrollController->PageDown();
	Long previous = this->textEditingForm->SetScrollPos(SB_VERT, position, TRUE);
	this->textEditingForm->ScrollWindow(0, previous - position);
	Long x = this->textEditingForm->caretController->GetCaretX() +
		this->textEditingForm->scrollController->GetHorizontalScroll()->GetPosition();
	Long y = this->textEditingForm->caretController->GetCaretY() + previous;
	Long row = this->textEditingForm->characterMetrics->GetRow(y - (previous - position));
	if (row > this->textEditingForm->note->GetLength() - 1) {
		row = this->textEditingForm->note->GetLength() - 1;
	}
	Long index = this->textEditingForm->note->Move(row);
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	Long column = this->textEditingForm->characterMetrics->GetColumn(this->textEditingForm->current, x);
	this->textEditingForm->current->Move(column);
}

string CNTPageDownCommand::GetType() {
	return "CNTPageDown";
}

CNTCommand* CNTPageDownCommand::Clone() {
	return new CNTPageDownCommand(*this);
}
//////////////////// Move ////////////////////

//////////////////// Select ////////////////////
//CNTShiftLeftCommand
CNTShiftLeftCommand::CNTShiftLeftCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftLeftCommand::CNTShiftLeftCommand(const CNTShiftLeftCommand& source)
	: CNTCommand(source) {
}

CNTShiftLeftCommand::~CNTShiftLeftCommand() {

}

CNTShiftLeftCommand& CNTShiftLeftCommand::operator=(const CNTShiftLeftCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftLeftCommand::Execute() {
	Glyph* character;
	Long column;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->textEditingForm->current->GetCurrent();
	if (lineCurrent > 0) {
		column = this->textEditingForm->current->Previous();
		character = this->textEditingForm->current->GetAt(column);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
	}
	else if (noteCurrent > 0) {
		row = this->textEditingForm->note->Previous();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		this->textEditingForm->current->Last();
		if (dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(row + 1))) {
			column = this->textEditingForm->current->Previous();
			character = this->textEditingForm->current->GetAt(column);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		}
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftLeftCommand::GetType() {
	return "CNTShiftLeft";
}

CNTCommand* CNTShiftLeftCommand::Clone() {
	return new CNTShiftLeftCommand(*this);
}

//CNTShiftRightCommand
CNTShiftRightCommand::CNTShiftRightCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftRightCommand::CNTShiftRightCommand(const CNTShiftRightCommand& source)
	: CNTCommand(source) {
}

CNTShiftRightCommand::~CNTShiftRightCommand() {

}

CNTShiftRightCommand& CNTShiftRightCommand::operator=(const CNTShiftRightCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftRightCommand::Execute() {
	Glyph* character;
	Long column;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->textEditingForm->current->GetCurrent();
	if (lineCurrent < this->textEditingForm->current->GetLength()) {
		column = this->textEditingForm->current->Next();
		character = this->textEditingForm->current->GetAt(column - 1);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
	}
	else if (noteCurrent < this->textEditingForm->note->GetLength() - 1) {
		row = this->textEditingForm->note->Next();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		this->textEditingForm->current->First();
		if (dynamic_cast<DummyLine*>(this->textEditingForm->current)) {
			column = this->textEditingForm->current->Next();
			character = this->textEditingForm->current->GetAt(column - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		}
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftRightCommand::GetType() {
	return "CNTShiftRight";
}

CNTCommand* CNTShiftRightCommand::Clone() {
	return new CNTShiftRightCommand(*this);
}

//CNTShiftUpCommand
CNTShiftUpCommand::CNTShiftUpCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftUpCommand::CNTShiftUpCommand(const CNTShiftUpCommand& source)
	: CNTCommand(source) {
}

CNTShiftUpCommand::~CNTShiftUpCommand() {

}

CNTShiftUpCommand& CNTShiftUpCommand::operator=(const CNTShiftUpCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftUpCommand::Execute() {
	Glyph* character;
	Long lineCurrent = this->textEditingForm->current->GetCurrent();
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long row = noteCurrent;
	if (noteCurrent > 0) {
		Long i = lineCurrent;
		while (i > 0) {
			character = this->textEditingForm->current->GetAt(i - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i--;
		}

		Long x = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current);
		row = this->textEditingForm->note->Previous();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		Long column = this->textEditingForm->characterMetrics->GetColumn(this->textEditingForm->current, x);
		this->textEditingForm->current->Move(column);

		i = this->textEditingForm->current->GetLength();
		while (i > column) {
			character = this->textEditingForm->current->GetAt(i - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i--;
		}
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftUpCommand::GetType() {
	return "CNTShiftUp";
}

CNTCommand* CNTShiftUpCommand::Clone() {
	return new CNTShiftUpCommand(*this);
}

//CNTShiftDownCommand
CNTShiftDownCommand::CNTShiftDownCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftDownCommand::CNTShiftDownCommand(const CNTShiftDownCommand& source)
	: CNTCommand(source) {
}

CNTShiftDownCommand::~CNTShiftDownCommand() {

}

CNTShiftDownCommand& CNTShiftDownCommand::operator=(const CNTShiftDownCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftDownCommand::Execute() {
	Glyph* character;
	Long lineCurrent = this->textEditingForm->current->GetCurrent();
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long row = noteCurrent;
	if (noteCurrent < this->textEditingForm->note->GetLength() - 1) {
		Long i = lineCurrent;
		while (i < this->textEditingForm->current->GetLength()) {
			character = this->textEditingForm->current->GetAt(i);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i++;
		}

		Long x = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current);
		row = this->textEditingForm->note->Next();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		Long column = this->textEditingForm->characterMetrics->GetColumn(this->textEditingForm->current, x);
		this->textEditingForm->current->Move(column);

		i = 0;
		while (i < column) {
			character = this->textEditingForm->current->GetAt(i);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			i++;
		}
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftDownCommand::GetType() {
	return "CNTShiftDown";
}

CNTCommand* CNTShiftDownCommand::Clone() {
	return new CNTShiftDownCommand(*this);
}

//CNTShiftHomeCommand
CNTShiftHomeCommand::CNTShiftHomeCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftHomeCommand::CNTShiftHomeCommand(const CNTShiftHomeCommand& source)
	: CNTCommand(source) {
}

CNTShiftHomeCommand::~CNTShiftHomeCommand() {

}

CNTShiftHomeCommand& CNTShiftHomeCommand::operator=(const CNTShiftHomeCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftHomeCommand::Execute() {
	Glyph* character;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long row = noteCurrent;
	Long lineCurrent = this->textEditingForm->current->GetCurrent();
	Long index = this->textEditingForm->current->First();
	if (lineCurrent == index
		&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(row))) {
		row = this->textEditingForm->note->Previous();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		index = this->textEditingForm->current->First();
		lineCurrent = this->textEditingForm->current->GetLength();
	}

	Long i = lineCurrent;
	while (i > index) {
		character = this->textEditingForm->current->GetAt(i - 1);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		i--;
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftHomeCommand::GetType() {
	return "CNTShiftHome";
}

CNTCommand* CNTShiftHomeCommand::Clone() {
	return new CNTShiftHomeCommand(*this);
}

//CNTShiftEndCommand
CNTShiftEndCommand::CNTShiftEndCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftEndCommand::CNTShiftEndCommand(const CNTShiftEndCommand& source)
	: CNTCommand(source) {
}

CNTShiftEndCommand::~CNTShiftEndCommand() {

}

CNTShiftEndCommand& CNTShiftEndCommand::operator=(const CNTShiftEndCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftEndCommand::Execute() {
	Glyph* character;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long lineCurrent = this->textEditingForm->current->GetCurrent();
	Long index = this->textEditingForm->current->Last();
	Long row = this->textEditingForm->note->GetCurrent();
	if (lineCurrent == index
		&& row + 1 < this->textEditingForm->note->GetLength()
		&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(row + 1))) {
		row = this->textEditingForm->note->Next();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
		index = this->textEditingForm->current->Last();
		lineCurrent = 0;
	}

	Long i = lineCurrent;
	while (i < index) {
		character = this->textEditingForm->current->GetAt(i);
		(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
		i++;
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftEndCommand::GetType() {
	return "CNTShiftEnd";
}

CNTCommand* CNTShiftEndCommand::Clone() {
	return new CNTShiftEndCommand(*this);
}

//CNTShiftCtrlLeftCommand
CNTShiftCtrlLeftCommand::CNTShiftCtrlLeftCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftCtrlLeftCommand::CNTShiftCtrlLeftCommand(const CNTShiftCtrlLeftCommand& source)
	: CNTCommand(source) {
}

CNTShiftCtrlLeftCommand::~CNTShiftCtrlLeftCommand() {

}

CNTShiftCtrlLeftCommand& CNTShiftCtrlLeftCommand::operator=(const CNTShiftCtrlLeftCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftCtrlLeftCommand::Execute() {
	Glyph* character;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long row = noteCurrent;

	Long lineCurrent = this->textEditingForm->current->GetCurrent();

	row = this->textEditingForm->note->MovePreviousWord();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(row);

	while (row >= 0
		&& (this->textEditingForm->current->GetCurrent() <= 0
			&& dynamic_cast<DummyLine*>(this->textEditingForm->current))
		|| (this->textEditingForm->current->GetCurrent() >= this->textEditingForm->current->GetLength()
			&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(row + 1)))) {
		row = this->textEditingForm->note->MovePreviousWord();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
	}

	Long lineNext = this->textEditingForm->current->GetCurrent();
	Glyph* line;
	Long column;
	Long j;
	Long i = noteCurrent;
	while (i >= row) {
		line = this->textEditingForm->note->GetAt(i);
		column = 0;
		if (i <= row) {
			column = lineNext;
		}
		j = line->GetLength();
		if (i == noteCurrent) {
			j = lineCurrent;
		}
		while (j > column) {
			character = line->GetAt(j - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			j--;
		}
		i--;
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftCtrlLeftCommand::GetType() {
	return "CNTShiftCtrlLeft";
}

CNTCommand* CNTShiftCtrlLeftCommand::Clone() {
	return new CNTShiftCtrlLeftCommand(*this);
}

//CNTShiftCtrlRightCommand
CNTShiftCtrlRightCommand::CNTShiftCtrlRightCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftCtrlRightCommand::CNTShiftCtrlRightCommand(const CNTShiftCtrlRightCommand& source)
	: CNTCommand(source) {
}

CNTShiftCtrlRightCommand::~CNTShiftCtrlRightCommand() {

}

CNTShiftCtrlRightCommand& CNTShiftCtrlRightCommand::operator=(const CNTShiftCtrlRightCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftCtrlRightCommand::Execute() {
	Glyph* character;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long row = noteCurrent;

	Long lineCurrent = this->textEditingForm->current->GetCurrent();

	row = this->textEditingForm->note->MoveNextWord();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
	while (row + 1 < this->textEditingForm->note->GetLength()
		&& (this->textEditingForm->current->GetCurrent() >= this->textEditingForm->current->GetLength()
			&& dynamic_cast<DummyLine*>(this->textEditingForm->note->GetAt(row + 1)))
		|| (this->textEditingForm->current->GetCurrent() <= 0
			&& dynamic_cast<DummyLine*>(this->textEditingForm->current))) {
		row = this->textEditingForm->note->MoveNextWord();
		this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
	}

	Long lineNext = this->textEditingForm->current->GetCurrent();
	Glyph* line;
	Long column;
	Long j;
	Long i = noteCurrent;
	while (i <= row) {
		line = this->textEditingForm->note->GetAt(i);
		column = line->GetLength();
		if (i >= row) {
			column = lineNext;
		}
		j = 0;
		if (i == noteCurrent) {
			j = lineCurrent;
		}
		while (j < column) {
			character = line->GetAt(j);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			j++;
		}
		i++;
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftCtrlRightCommand::GetType() {
	return "CNTShiftCtrlRight";
}

CNTCommand* CNTShiftCtrlRightCommand::Clone() {
	return new CNTShiftCtrlRightCommand(*this);
}

//CNTShiftCtrlHomeCommand
CNTShiftCtrlHomeCommand::CNTShiftCtrlHomeCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftCtrlHomeCommand::CNTShiftCtrlHomeCommand(const CNTShiftCtrlHomeCommand& source)
	: CNTCommand(source) {
}

CNTShiftCtrlHomeCommand::~CNTShiftCtrlHomeCommand() {

}

CNTShiftCtrlHomeCommand& CNTShiftCtrlHomeCommand::operator=(const CNTShiftCtrlHomeCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftCtrlHomeCommand::Execute() {
	Glyph* character;
	Glyph* line;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long lineCurrent = this->textEditingForm->current->GetCurrent();

	Long row = this->textEditingForm->note->First();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
	this->textEditingForm->current->First();

	Long j;
	Long i = noteCurrent;
	while (i >= row) {
		line = this->textEditingForm->note->GetAt(i);
		j = line->GetLength();
		if (i == noteCurrent) {
			j = lineCurrent;
		}
		while (j > 0) {
			character = line->GetAt(j - 1);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			j--;
		}
		i--;
	}

	Long start = row;
	Long end = noteCurrent;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originStart == noteCurrent) { //선택할 때
			start = row;
			end = originEnd;
		}
		else if (originEnd == noteCurrent) { //선택 해제할 때
			end = row;
			start = originStart;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftCtrlHomeCommand::GetType() {
	return "CNTShiftCtrlHome";
}

CNTCommand* CNTShiftCtrlHomeCommand::Clone() {
	return new CNTShiftCtrlHomeCommand(*this);
}

//CNTShiftCtrlEndCommand
CNTShiftCtrlEndCommand::CNTShiftCtrlEndCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTShiftCtrlEndCommand::CNTShiftCtrlEndCommand(const CNTShiftCtrlEndCommand& source)
	: CNTCommand(source) {
}

CNTShiftCtrlEndCommand::~CNTShiftCtrlEndCommand() {

}

CNTShiftCtrlEndCommand& CNTShiftCtrlEndCommand::operator=(const CNTShiftCtrlEndCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTShiftCtrlEndCommand::Execute() {
	Glyph* character;
	Glyph* line;
	Long noteCurrent = this->textEditingForm->note->GetCurrent();
	Long lineCurrent = this->textEditingForm->current->GetCurrent();

	Long row = this->textEditingForm->note->Last();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(row);
	this->textEditingForm->current->Last();

	Long j;
	Long i = noteCurrent;
	while (i <= row) {
		line = this->textEditingForm->note->GetAt(i);
		j = 0;
		if (i == noteCurrent) {
			j = lineCurrent;
		}
		while (j < line->GetLength()) {
			character = line->GetAt(j);
			(!character->GetIsSelected()) ? (character->Select(true)) : (character->Select(false));
			j++;
		}
		i++;
	}

	Long start = noteCurrent;
	Long end = row;
	if (this->textEditingForm->selection != NULL) {
		Long originStart = this->textEditingForm->selection->GetStart();
		Long originEnd = this->textEditingForm->selection->GetEnd();
		if (originEnd == noteCurrent) { //선택할 때
			start = originStart;
			end = row;
		}
		else if (originStart == noteCurrent) { //선택 해제할 때
			end = originEnd;
			start = row;
		}
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
	this->textEditingForm->selection = new Selection(start, end);

	if (start == end && this->textEditingForm->note->IsSelecting() == false) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
	}
}

string CNTShiftCtrlEndCommand::GetType() {
	return "CNTShiftCtrlEnd";
}

CNTCommand* CNTShiftCtrlEndCommand::Clone() {
	return new CNTShiftCtrlEndCommand(*this);
}
//////////////////// Select ////////////////////

//////////////////// Flag ////////////////////
//CNTLockHScrollCommand
CNTLockHScrollCommand::CNTLockHScrollCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTLockHScrollCommand::CNTLockHScrollCommand(const CNTLockHScrollCommand& source)
	: CNTCommand(source) {
}

CNTLockHScrollCommand::~CNTLockHScrollCommand() {

}

CNTLockHScrollCommand& CNTLockHScrollCommand::operator=(const CNTLockHScrollCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTLockHScrollCommand::Execute() {
	CRect rect;
	this->textEditingForm->GetClientRect(rect);
	DummyManager dummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, rect.Width());

	Long i;
	if (this->textEditingForm->GetIsLockedHScroll() == FALSE) {
		this->textEditingForm->SetIsLockedHScroll(TRUE);
		i = 0;
		while (i < this->textEditingForm->note->GetLength()) {
			i = dummyManager.Fold(i);
			i++;
		}
	}
	else {
		this->textEditingForm->SetIsLockedHScroll(FALSE);
		i = 0;
		while (i < this->textEditingForm->note->GetLength()) {
			dummyManager.Unfold(i);
			i++;
		}
	}

	if (this->textEditingForm->selection != NULL) {
		delete this->textEditingForm->selection;
		this->textEditingForm->selection = NULL;
		this->textEditingForm->note->UnselectAll();
	}

	this->textEditingForm->undoHistoryBook->Empty();
	this->textEditingForm->redoHistoryBook->Empty();

	Long index = this->textEditingForm->note->First();
	this->textEditingForm->current = this->textEditingForm->note->GetAt(index);
	this->textEditingForm->current->First();
}

string CNTLockHScrollCommand::GetType() {
	return "CNTLockHScroll";
}

CNTCommand* CNTLockHScrollCommand::Clone() {
	return new CNTLockHScrollCommand(*this);
}

//CNTUnlockHistoryBookCommand
CNTUnlockHistoryBookCommand::CNTUnlockHistoryBookCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTUnlockHistoryBookCommand::CNTUnlockHistoryBookCommand(const CNTUnlockHistoryBookCommand& source)
	: CNTCommand(source) {
}

CNTUnlockHistoryBookCommand::~CNTUnlockHistoryBookCommand() {

}

CNTUnlockHistoryBookCommand& CNTUnlockHistoryBookCommand::operator=(const CNTUnlockHistoryBookCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTUnlockHistoryBookCommand::Execute() {
	BOOL isCNTUnlockedHistoryBook = this->textEditingForm->GetIsUnlockedHistoryBook();
	if (isCNTUnlockedHistoryBook == FALSE) {
		isCNTUnlockedHistoryBook = TRUE;
	}
	else {
		isCNTUnlockedHistoryBook = FALSE;
		this->textEditingForm->undoHistoryBook->Empty();
		this->textEditingForm->redoHistoryBook->Empty();
	}
	this->textEditingForm->SetIsUnlockedHistoryBook(isCNTUnlockedHistoryBook);
}

string CNTUnlockHistoryBookCommand::GetType() {
	return "CNTUnlockHistoryBook";
}

CNTCommand* CNTUnlockHistoryBookCommand::Clone() {
	return new CNTUnlockHistoryBookCommand(*this);
}

//CNTUnlockFindReplaceDialogCommand
CNTUnlockFindReplaceDialogCommand::CNTUnlockFindReplaceDialogCommand(TextEditingForm* textEditingForm)
	: CNTCommand(textEditingForm) {
}

CNTUnlockFindReplaceDialogCommand::CNTUnlockFindReplaceDialogCommand(const CNTUnlockFindReplaceDialogCommand& source)
	: CNTCommand(source) {
}

CNTUnlockFindReplaceDialogCommand::~CNTUnlockFindReplaceDialogCommand() {

}

CNTUnlockFindReplaceDialogCommand& CNTUnlockFindReplaceDialogCommand::operator=(const CNTUnlockFindReplaceDialogCommand& source) {
	CNTCommand::operator=(source);

	return *this;
}

void CNTUnlockFindReplaceDialogCommand::Execute() {
	BOOL isCNTUnlockedFindReplaceDialog = this->textEditingForm->GetIsUnlockedFindReplaceDialog();
	if (isCNTUnlockedFindReplaceDialog == FALSE) {
		isCNTUnlockedFindReplaceDialog = TRUE;
	}
	else {
		isCNTUnlockedFindReplaceDialog = FALSE;
	}
	this->textEditingForm->SetIsUnlockedFindReplaceDialog(isCNTUnlockedFindReplaceDialog);
}

string CNTUnlockFindReplaceDialogCommand::GetType() {
	return "CNTUnlockFindReplaceDialog";
}

CNTCommand* CNTUnlockFindReplaceDialogCommand::Clone() {
	return new CNTUnlockFindReplaceDialogCommand(*this);
}
//////////////////// Flag ////////////////////