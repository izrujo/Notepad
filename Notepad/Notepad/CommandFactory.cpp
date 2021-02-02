#include "CommandFactory.h"
#include "Commands.h"
#include "NotepadForm.h"
#include "resource.h"

CommandFactory::CommandFactory(NotepadForm* notepadForm) {
	this->notepadForm = notepadForm;
}

CommandFactory::CommandFactory(const CommandFactory& source) {
	this->notepadForm = source.notepadForm;
}

CommandFactory::~CommandFactory() {

}

CommandFactory& CommandFactory::operator=(const CommandFactory& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

Command* CommandFactory::Make(int uID) {
	Command* command = 0;
	if (uID == IDM_FORMAT_FONT) {
		command = new FontCommand(this->notepadForm);
	}
	else if (uID == IDM_FORMAT_AUTONEWLINE) {
		command = new AutoNewlineCommand(this->notepadForm);
	}
	else if (uID == IDM_FILE_NEW) {
		command = new NewCommand(this->notepadForm);
	}
	else if (uID == IDM_FILE_OPEN) {
		command = new OpenCommand(this->notepadForm);
	}
	else if (uID == IDM_FILE_SAVE) {
		command = new SaveCommand(this->notepadForm);
	}
	else if (uID == IDM_FILE_SAVEAS) {
		command = new SaveAsCommand(this->notepadForm);
	}
	else if (uID == IDM_FILE_PAGESETUP) {
		command = new PageSetupCommand(this->notepadForm);
	}
	else if (uID == IDM_FILE_PRINT) {
		command = new PrintCommand(this->notepadForm);
	}
	else if (uID == IDM_FILE_PREVIEW) {
		command = new PreviewCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_COPY) {
		command = new CopyCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_PASTE) {
		command = new PasteCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_CUT) {
		command = new CutCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_SELECTALL) {
		command = new SelectAllCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_DELETE) {
		command = new DeleteSelectionCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_UNDO) {
		command = new UndoCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_REDO) {
		command = new RedoCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_FIND) {
		command = new FindCommand(this->notepadForm);
	}
	else if (uID == IDM_EDIT_REPLACE) {
		command = new ReplaceCommand(this->notepadForm);
	}

	return command;
}