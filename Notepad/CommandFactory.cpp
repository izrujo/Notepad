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
	else if (uID == IDC_WRITE_CHAR) {
		command = new WriteCommand(this->notepadForm);
	}
	else if (uID == IDC_IME_COMPOSITION) {
		command = new ImeCompositionCommand(this->notepadForm);
	}
	else if (uID == IDC_IME_CHAR) {
		command = new ImeCharCommand(this->notepadForm);
	}
	else if (uID == IDC_DELETE_CHAR) {
		command = new DeleteCommand(this->notepadForm);
	}
	else if (uID == IDC_WRITE_AFTER_DELETE) {
		command = new WriteAfterDeleteCommand(this->notepadForm);
	}
	else if (uID == IDC_IME_AFTER_DELETE) {
		command = new ImeAfterDeleteCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_LEFT) {
		command = new LeftCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_RIGHT) {
		command = new RightCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_UP) {
		command = new UpCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_DOWN) {
		command = new DownCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_HOME) {
		command = new HomeCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_END) {
		command = new EndCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_CTRLLEFT) {
		command = new CtrlLeftCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_CTRLRIGHT) {
		command = new CtrlRightCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_CTRLHOME) {
		command = new CtrlHomeCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_CTRLEND) {
		command = new CtrlEndCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_PAGEUP) {
		command = new PageUpCommand(this->notepadForm);
	}
	else if (uID == IDC_MOVE_PAGEDOWN) {
		command = new PageDownCommand(this->notepadForm);
	}

	return command;
}