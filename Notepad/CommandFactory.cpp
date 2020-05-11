#include "CommandFactory.h"
#include "Command.h"
#include "FontCommand.h"
#include "NewCommand.h"
#include "OpenCommand.h"
#include "SaveCommand.h"
#include "SaveAsCommand.h"
#include "NotepadForm.h"
#include "resource.h"
#include "WordWrapCheckedCommand.h"
#include "WordWrapUnCheckedCommand.h"
#include "WordWrapController.h"

CommandFactory::CommandFactory(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

CommandFactory::CommandFactory(const CommandFactory& source) {
	this->notepadForm = source.notepadForm;
}

CommandFactory::~CommandFactory() {
}

Command* CommandFactory::Make(int uID) {
	Command *command = 0;
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
	else if (uID == IDM_FORMAT_WORDWRAP) {
		CMenu *menu = this->notepadForm->GetMenu();
		UINT menuState = menu->GetMenuState(uID, MF_BYCOMMAND);
		if (menuState == MF_UNCHECKED) {
			menu->CheckMenuItem(uID, MF_CHECKED | MF_BYCOMMAND);
			if (this->notepadForm->wordWrapController == NULL) {
				this->notepadForm->wordWrapController = new WordWrapController(this->notepadForm);
			}
			command = new WordWrapCheckedCommand(this->notepadForm);
		}
		else if (menuState == MF_CHECKED) {
			menu->CheckMenuItem(uID, MF_UNCHECKED | MF_BYCOMMAND);
			if (this->notepadForm->wordWrapController != NULL) {
				delete this->notepadForm->wordWrapController;
				this->notepadForm->wordWrapController = NULL;
			}
			command = new WordWrapUnCheckedCommand(this->notepadForm);
		}
	}

	return command;
}

CommandFactory& CommandFactory::operator =(const CommandFactory& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}