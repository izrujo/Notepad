#include "FileManager.h"
#include "File.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "Scanner.h"
#include "ScrollController.h"

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

void FileManager::Save(string filePathName) {
	File file(filePathName);
	
	string content = this->notepadForm->note->GetContent();

	file.Save(content);
}

void FileManager::Load(string filePathName) {
	File file(filePathName);
	string content = file.Load();

	if (this->notepadForm->note != NULL) {
		delete this->notepadForm->note;
	}
	GlyphFactory glyphFactory;
	this->notepadForm->note = glyphFactory.Make("");
	this->notepadForm->current = glyphFactory.Make("\r\n");
	this->notepadForm->note->Add(this->notepadForm->current);
	Scanner scanner(content);
	while (scanner.IsEnd() == FALSE) {
		string token = scanner.GetToken();
		Glyph* glyph = glyphFactory.Make(token.c_str());
		if (token != "\r\n") {
			this->notepadForm->current->Add(glyph);
		}
		else {
			Long index = this->notepadForm->note->Add(glyph);
			this->notepadForm->current = this->notepadForm->note->GetAt(index);
		}
		scanner.Next();
	}

	Long index = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();

	if (this->notepadForm->scrollController != NULL) {
		delete this->notepadForm->scrollController;
		this->notepadForm->scrollController = new ScrollController(this->notepadForm);
	}
	this->notepadForm->Notify();
}

void FileManager::New() {
	if (this->notepadForm->note != NULL) {
		delete this->notepadForm->note;
	}
	GlyphFactory glyphFactory;
	this->notepadForm->note = glyphFactory.Make("");
	Glyph* line = glyphFactory.Make("\r\n");
	this->notepadForm->note->Add(line);

	Long index = this->notepadForm->note->First();
	this->notepadForm->current = this->notepadForm->note->GetAt(index);
	this->notepadForm->current->First();

	if (this->notepadForm->scrollController != NULL) {
		delete this->notepadForm->scrollController;
		this->notepadForm->scrollController = new ScrollController(this->notepadForm);
	}
	this->notepadForm->Notify();
}