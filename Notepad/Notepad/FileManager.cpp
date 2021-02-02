#include "FileManager.h"
#include "Files.h"
#include "FileFactory.h"
#include "NotepadForm.h"
#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Scanner.h"
#include "../TextEditor/ScrollController.h"
#include "Document.h"

FileManager::FileManager(NotepadForm* notepadForm) {
	this->notepadForm = notepadForm;
}

FileManager::FileManager(const FileManager& source) {
	this->notepadForm = source.notepadForm;
}

FileManager::~FileManager() {

}

FileManager& FileManager::operator=(const FileManager& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

void FileManager::Save() {
	string filePathName = this->notepadForm->document->GetPathName();
	string encodingType = this->notepadForm->document->GetEncodingType();

	FileFactory fileFactory;
	File* file = fileFactory.MakeSaveFile(filePathName, encodingType);

	string content = this->notepadForm->textEditingForm->note->GetContent();

	file->Save(content);

	if (file != 0) {
		delete file;
	}
}

void FileManager::Load() {
	string filePathName = this->notepadForm->document->GetPathName();

	FileFactory fileFactory;
	File *file = fileFactory.MakeOpenFile(filePathName);
	this->notepadForm->document->SetEncodingType(file->GetType());
	string content = file->Load();

	if (file != 0) {
		delete file;
	}

	if (this->notepadForm->textEditingForm->note != NULL) {
		delete this->notepadForm->textEditingForm->note;
	}
	GlyphFactory glyphFactory;
	this->notepadForm->textEditingForm->note = glyphFactory.Make("");
	this->notepadForm->textEditingForm->current = glyphFactory.Make("\r\n");
	this->notepadForm->textEditingForm->note->Add(this->notepadForm->textEditingForm->current);
	Scanner scanner(content);
	while (scanner.IsEnd() == FALSE) {
		string token = scanner.GetToken();
		Glyph* glyph = glyphFactory.Make(token.c_str());
		if (token != "\r\n") {
			this->notepadForm->textEditingForm->current->Add(glyph);
		}
		else {
			Long index = this->notepadForm->textEditingForm->note->Add(glyph);
			this->notepadForm->textEditingForm->current = this->notepadForm->textEditingForm->note->GetAt(index);
		}
		scanner.Next();
	}

	Long index = this->notepadForm->textEditingForm->note->First();
	this->notepadForm->textEditingForm->current = this->notepadForm->textEditingForm->note->GetAt(index);
	this->notepadForm->textEditingForm->current->First();

	if (this->notepadForm->textEditingForm->scrollController != NULL) {
		delete this->notepadForm->textEditingForm->scrollController;
		this->notepadForm->textEditingForm->scrollController = new ScrollController(this->notepadForm->textEditingForm);
	}
	this->notepadForm->textEditingForm->Notify();
}

void FileManager::New() {
	if (this->notepadForm->textEditingForm->note != NULL) {
		delete this->notepadForm->textEditingForm->note;
	}
	GlyphFactory glyphFactory;
	this->notepadForm->textEditingForm->note = glyphFactory.Make("");
	Glyph* line = glyphFactory.Make("\r\n");
	this->notepadForm->textEditingForm->note->Add(line);

	Long index = this->notepadForm->textEditingForm->note->First();
	this->notepadForm->textEditingForm->current = this->notepadForm->textEditingForm->note->GetAt(index);
	this->notepadForm->textEditingForm->current->First();

	if (this->notepadForm->textEditingForm->scrollController != NULL) {
		delete this->notepadForm->textEditingForm->scrollController;
		this->notepadForm->textEditingForm->scrollController = new ScrollController(this->notepadForm->textEditingForm);
	}
	this->notepadForm->textEditingForm->Notify();
}