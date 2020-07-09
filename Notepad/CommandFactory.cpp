#include "CommandFactory.h"
#include "Command.h"
#include "FontCommand.h"
#include "NewCommand.h"
#include "OpenCommand.h"
#include "SaveCommand.h"
#include "SaveAsCommand.h"
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

	return command;
}