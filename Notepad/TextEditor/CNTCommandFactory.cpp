#include "CNTCommandFactory.h"
#include "CNTCommands.h"
#include "TextEditingForm.h"
#include "resource.h"

CNTCommandFactory::CNTCommandFactory(TextEditingForm* textEditingForm) {
	this->textEditingForm = textEditingForm;
}

CNTCommandFactory::CNTCommandFactory(const CNTCommandFactory& source) {
	this->textEditingForm = source.textEditingForm;
}

CNTCommandFactory::~CNTCommandFactory() {

}

CNTCommandFactory& CNTCommandFactory::operator=(const CNTCommandFactory& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

CNTCommand* CNTCommandFactory::Make(int uID) {
	CNTCommand* command = 0;

	//////////////////// Main ////////////////////
	if (uID == IDCNT_EDIT_WRITE) {
		command = new CNTWriteCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_IMECOMPOSITION) {
		command = new CNTImeCompositionCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_IMECHAR) {
		command = new CNTImeCharCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_DELETE) {
		command = new CNTDeleteCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_COPY) {
		command = new CNTCopyCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_DELETESELECTION) {
		command = new CNTDeleteSelectionCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_CUT) {
		command = new CNTCutCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_PASTE) {
		command = new CNTPasteCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_SELECTALL) {
		command = new CNTSelectAllCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_UNDO) {
		command = new CNTUndoCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_REDO) {
		command = new CNTRedoCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_FIND) {
		command = new CNTFindCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_EDIT_REPLACE) {
		command = new CNTReplaceCommand(this->textEditingForm);
	}
	//////////////////// Main ////////////////////
	//////////////////// Basic ////////////////////
	else if (uID == IDCNT_BASIC_WRITE) {
		command = new CNTWriteBasicCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_BASIC_IMECOMPOSITION) {
		command = new CNTImeCompositionBasicCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_BASIC_IMECHAR) {
		command = new CNTImeCharBasicCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_BASIC_DELETE) {
		command = new CNTDeleteBasicCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_BASIC_COPY) {
		command = new CNTCopyBasicCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_BASIC_DELETESELECTION) {
		command = new CNTDeleteSelectionBasicCommand(this->textEditingForm);
	}
	//////////////////// Basic ////////////////////
	//////////////////// Move ////////////////////
	else if (uID == IDCNT_MOVE_LEFT) {
		command = new CNTLeftCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_RIGHT) {
		command = new CNTRightCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_UP) {
		command = new CNTUpCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_DOWN) {
		command = new CNTDownCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_HOME) {
		command = new CNTHomeCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_END) {
		command = new CNTEndCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_CTRLLEFT) {
		command = new CNTCtrlLeftCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_CTRLRIGHT) {
		command = new CNTCtrlRightCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_CTRLHOME) {
		command = new CNTCtrlHomeCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_CTRLEND) {
		command = new CNTCtrlEndCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_PAGEUP) {
		command = new CNTPageUpCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_MOVE_PAGEDOWN) {
		command = new CNTPageDownCommand(this->textEditingForm);
	}
	//////////////////// Move ////////////////////
	//////////////////// Select ////////////////////
	else if (uID == IDCNT_SELECTMOVE_LEFT) {
		command = new CNTShiftLeftCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_RIGHT) {
		command = new CNTShiftRightCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_UP) {
		command = new CNTShiftUpCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_DOWN) {
		command = new CNTShiftDownCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_HOME) {
		command = new CNTShiftHomeCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_END) {
		command = new CNTShiftEndCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_CTRLLEFT) {
		command = new CNTShiftCtrlLeftCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_CTRLRIGHT) {
		command = new CNTShiftCtrlRightCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_CTRLHOME) {
		command = new CNTShiftCtrlHomeCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_SELECTMOVE_CTRLEND) {
		command = new CNTShiftCtrlEndCommand(this->textEditingForm);
	}
	//////////////////// Select ////////////////////
	//////////////////// Flag ////////////////////
	else if (uID == IDCNT_FLAG_LOCKHSCROLL) {
		command = new CNTLockHScrollCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_FLAG_UNLOCKHISTORYBOOK) {
		command = new CNTUnlockHistoryBookCommand(this->textEditingForm);
	}
	else if (uID == IDCNT_FLAG_UNLOCKFINDREPLACEDIALOG) {
		command = new CNTUnlockFindReplaceDialogCommand(this->textEditingForm);
	}
	//////////////////// Flag ////////////////////
	else if (uID == IDCNT_ETC_SIZE) {
		command = new CNTSizeCommand(this->textEditingForm);
	}

	return command;
}