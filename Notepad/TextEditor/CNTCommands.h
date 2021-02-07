#ifndef _COMMAND_H
#define _COMMAND_H

#include "../Utilities/Array.h"
#include "../Utilities/Stack.h"
#include <iostream>
#include <afxwin.h> //TCHAR

using namespace std;
typedef signed long int Long;

class TextEditingForm;
class Glyph;

class CNTCommand {
public:
	CNTCommand(TextEditingForm* textEditingForm = 0);
	CNTCommand(const CNTCommand& source);
	virtual ~CNTCommand() = 0;
	CNTCommand& operator=(const CNTCommand& source);

	virtual void Execute() = 0;
	virtual void Unexecute();

	virtual string GetType() = 0;
	virtual CNTCommand* Clone() = 0;

	//MacroCommand
	virtual Long Add(CNTCommand* command);
	virtual Long Remove(Long index);
	virtual CNTCommand* GetAt(Long index);
	virtual Long GetCapacity() const;
	virtual Long GetLength() const;

	//SizeCommand
	virtual Long Write(CNTCommand* command);
	virtual Long Erase();
	virtual CNTCommand* OpenAt();
	virtual void Empty();
	virtual bool IsEmpty();
	virtual Long GetTop() const;
	virtual Long GetWidth() const;

protected:
	TextEditingForm* textEditingForm;
};

//////////////////// Composite ////////////////////
//CNTMacroCommand
class CNTMacroCommand : public CNTCommand {
public:
	CNTMacroCommand(TextEditingForm* textEditingForm = 0, Long capacity = 10);
	CNTMacroCommand(const CNTMacroCommand& source);
	virtual ~CNTMacroCommand();
	CNTMacroCommand& operator=(const CNTMacroCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual Long Add(CNTCommand* command);
	virtual Long Remove(Long index);
	virtual CNTCommand* GetAt(Long index);

	virtual string GetType();
	virtual CNTCommand* Clone();

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;

private:
	Array<CNTCommand*> commands;
	Long capacity;
	Long length;
};

inline Long CNTMacroCommand::GetCapacity() const {
	return this->capacity;
}

inline Long CNTMacroCommand::GetLength() const {
	return this->length;
}

//CNTSizeCommand
class CNTSizeCommand : public CNTCommand {
public:
	CNTSizeCommand(TextEditingForm* textEditingForm = 0, Long capacity = 64);
	CNTSizeCommand(const CNTSizeCommand& source);
	virtual ~CNTSizeCommand();
	CNTSizeCommand& operator=(const CNTSizeCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual Long Write(CNTCommand* command);
	virtual Long Erase();
	virtual CNTCommand* OpenAt();
	virtual void Empty();
	virtual bool IsEmpty();

	virtual string GetType();
	virtual CNTCommand* Clone();

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetTop() const;
	virtual Long GetWidth() const;

private:
	Stack<CNTCommand*> commands;
	Long capacity;
	Long length;
	Long top;
	Long width;
};

inline Long CNTSizeCommand::GetCapacity() const {
	return this->capacity;
}

inline Long CNTSizeCommand::GetLength() const {
	return this->length;
}

inline Long CNTSizeCommand::GetTop() const {
	return this->top;
}

inline Long CNTSizeCommand::GetWidth() const {
	return this->width;
}
//////////////////// Composite ////////////////////

//////////////////// Basic ////////////////////
//CNTWriteBasicCommand
class CNTWriteBasicCommand : public CNTCommand {
public:
	CNTWriteBasicCommand(TextEditingForm* textEditingForm = 0);
	CNTWriteBasicCommand(const CNTWriteBasicCommand& source);
	virtual ~CNTWriteBasicCommand();
	CNTWriteBasicCommand& operator=(const CNTWriteBasicCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTImeCompositionBasicCommand
class CNTImeCompositionBasicCommand : public CNTCommand {
public:
	CNTImeCompositionBasicCommand(TextEditingForm* textEditingForm = 0);
	CNTImeCompositionBasicCommand(const CNTImeCompositionBasicCommand& source);
	virtual ~CNTImeCompositionBasicCommand();
	CNTImeCompositionBasicCommand& operator=(const CNTImeCompositionBasicCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTImeCharBasicCommand
class CNTImeCharBasicCommand : public CNTCommand {
public:
	CNTImeCharBasicCommand(TextEditingForm* textEditingForm = 0);
	CNTImeCharBasicCommand(const CNTImeCharBasicCommand& source);
	virtual ~CNTImeCharBasicCommand();
	CNTImeCharBasicCommand& operator=(const CNTImeCharBasicCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTDeleteBasicCommand
class CNTDeleteBasicCommand : public CNTCommand {
public:
	CNTDeleteBasicCommand(TextEditingForm* textEditingForm = 0);
	CNTDeleteBasicCommand(const CNTDeleteBasicCommand& source);
	virtual ~CNTDeleteBasicCommand();
	CNTDeleteBasicCommand& operator=(const CNTDeleteBasicCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTCopyBasicCommand
class CNTCopyBasicCommand : public CNTCommand {
public:
	CNTCopyBasicCommand(TextEditingForm* textEditingForm = 0);
	CNTCopyBasicCommand(const CNTCopyBasicCommand& source);
	virtual ~CNTCopyBasicCommand();
	CNTCopyBasicCommand& operator=(const CNTCopyBasicCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTDeleteSelectionBasicCommand
class CNTDeleteSelectionBasicCommand : public CNTCommand {
public:
	CNTDeleteSelectionBasicCommand(TextEditingForm* textEditingForm = 0);
	CNTDeleteSelectionBasicCommand(const CNTDeleteSelectionBasicCommand& source);
	virtual ~CNTDeleteSelectionBasicCommand();
	CNTDeleteSelectionBasicCommand& operator=(const CNTDeleteSelectionBasicCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};
//////////////////// Basic ////////////////////

//////////////////// Main ////////////////////
//CNTWriteCommand
class CNTWriteCommand : public CNTCommand {
public:
	CNTWriteCommand(TextEditingForm* textEditingForm = 0);
	CNTWriteCommand(const CNTWriteCommand& source);
	virtual ~CNTWriteCommand();
	CNTWriteCommand& operator=(const CNTWriteCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual string GetType();
	virtual CNTCommand* Clone();

private:
	int nChar;
	Long row;
	Long column;
};

//CNTImeCompositionCommand
class CNTImeCompositionCommand : public CNTCommand {
public:
	CNTImeCompositionCommand(TextEditingForm* textEditingForm = 0);
	CNTImeCompositionCommand(const CNTImeCompositionCommand& source);
	virtual ~CNTImeCompositionCommand();
	CNTImeCompositionCommand& operator=(const CNTImeCompositionCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTImeCharCommand
class CNTImeCharCommand : public CNTCommand {
public:
	CNTImeCharCommand(TextEditingForm* textEditingForm = 0);
	CNTImeCharCommand(const CNTImeCharCommand& source);
	virtual ~CNTImeCharCommand();
	CNTImeCharCommand& operator=(const CNTImeCharCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual string GetType();
	virtual CNTCommand* Clone();

private:
	TCHAR(*buffer);
	Long row;
	Long column;
};

//CNTDeleteCommand
class CNTDeleteCommand : public CNTCommand {
public:
	CNTDeleteCommand(TextEditingForm* textEditingForm = 0);
	CNTDeleteCommand(const CNTDeleteCommand& source);
	virtual ~CNTDeleteCommand();
	CNTDeleteCommand& operator=(const CNTDeleteCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual string GetType();
	virtual CNTCommand* Clone();

private:
	Long row;
	Long noteLength;
	Long column;
	Long lineLength;
	Glyph* character;
};

//CNTCopyCommand
class CNTCopyCommand : public CNTCommand {
public:
	CNTCopyCommand(TextEditingForm* textEditingForm = 0);
	CNTCopyCommand(const CNTCopyCommand& source);
	virtual ~CNTCopyCommand();
	CNTCopyCommand& operator=(const CNTCopyCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTDeleteSelectionCommand
class CNTDeleteSelectionCommand : public CNTCommand {
public:
	CNTDeleteSelectionCommand(TextEditingForm* textEditingForm = 0);
	CNTDeleteSelectionCommand(const CNTDeleteSelectionCommand& source);
	virtual ~CNTDeleteSelectionCommand();
	CNTDeleteSelectionCommand& operator=(const CNTDeleteSelectionCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual string GetType();
	virtual CNTCommand* Clone();

private:
	Long startRow;
	Long startColumn;
	Long endRow;
	Long endColumn;
	string selecteds;
};

//CNTCutCommand
class CNTCutCommand : public CNTCommand {
public:
	CNTCutCommand(TextEditingForm* textEditingForm = 0);
	CNTCutCommand(const CNTCutCommand& source);
	virtual ~CNTCutCommand();
	CNTCutCommand& operator=(const CNTCutCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTPasteCommand
class CNTPasteCommand : public CNTCommand {
public:
	CNTPasteCommand(TextEditingForm* textEditingForm = 0);
	CNTPasteCommand(const CNTPasteCommand& source);
	virtual ~CNTPasteCommand();
	CNTPasteCommand& operator=(const CNTPasteCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual string GetType();
	virtual CNTCommand* Clone();

private:
	Long startRow;
	Long startColumn;
	Long endRow;
	Long endColumn;
	string pasteds;
};

//CNTSelectAllCommand
class CNTSelectAllCommand : public CNTCommand {
public:
	CNTSelectAllCommand(TextEditingForm* textEditingForm = 0);
	CNTSelectAllCommand(const CNTSelectAllCommand& source);
	virtual ~CNTSelectAllCommand();
	CNTSelectAllCommand& operator=(const CNTSelectAllCommand& source);

	virtual void Execute();

	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTUndoCommand
class CNTUndoCommand : public CNTCommand {
public:
	CNTUndoCommand(TextEditingForm* textEditingForm = 0);
	CNTUndoCommand(const CNTUndoCommand& source);
	virtual ~CNTUndoCommand();
	CNTUndoCommand& operator=(const CNTUndoCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTRedoCommand
class CNTRedoCommand : public CNTCommand {
public:
	CNTRedoCommand(TextEditingForm* textEditingForm = 0);
	CNTRedoCommand(const CNTRedoCommand& source);
	virtual ~CNTRedoCommand();
	CNTRedoCommand& operator=(const CNTRedoCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTFindCommand
class CNTFindCommand : public CNTCommand {
public:
	CNTFindCommand(TextEditingForm* textEditingForm = 0);
	CNTFindCommand(const CNTFindCommand& source);
	virtual ~CNTFindCommand();
	CNTFindCommand& operator=(const CNTFindCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTReplaceCommand
class CNTReplaceCommand : public CNTCommand {
public:
	CNTReplaceCommand(TextEditingForm* textEditingForm = 0);
	CNTReplaceCommand(const CNTReplaceCommand& source);
	virtual ~CNTReplaceCommand();
	CNTReplaceCommand& operator=(const CNTReplaceCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};
//////////////////// Main ////////////////////

//////////////////// Move ////////////////////
//CNTLeftCommand
class CNTLeftCommand : public CNTCommand {
public:
	CNTLeftCommand(TextEditingForm* textEditingForm = 0);
	CNTLeftCommand(const CNTLeftCommand& source);
	virtual ~CNTLeftCommand();
	CNTLeftCommand& operator=(const CNTLeftCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTRightCommand
class CNTRightCommand : public CNTCommand {
public:
	CNTRightCommand(TextEditingForm* textEditingForm = 0);
	CNTRightCommand(const CNTRightCommand& source);
	virtual ~CNTRightCommand();
	CNTRightCommand& operator=(const CNTRightCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTUpCommand
class CNTUpCommand : public CNTCommand {
public:
	CNTUpCommand(TextEditingForm* textEditingForm = 0);
	CNTUpCommand(const CNTUpCommand& source);
	virtual ~CNTUpCommand();
	CNTUpCommand& operator=(const CNTUpCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTDownCommand
class CNTDownCommand : public CNTCommand {
public:
	CNTDownCommand(TextEditingForm* textEditingForm = 0);
	CNTDownCommand(const CNTDownCommand& source);
	virtual ~CNTDownCommand();
	CNTDownCommand& operator=(const CNTDownCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTHomeCommand
class CNTHomeCommand : public CNTCommand {
public:
	CNTHomeCommand(TextEditingForm* textEditingForm = 0);
	CNTHomeCommand(const CNTHomeCommand& source);
	virtual ~CNTHomeCommand();
	CNTHomeCommand& operator=(const CNTHomeCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTEndCommand
class CNTEndCommand : public CNTCommand {
public:
	CNTEndCommand(TextEditingForm* textEditingForm = 0);
	CNTEndCommand(const CNTEndCommand& source);
	virtual ~CNTEndCommand();
	CNTEndCommand& operator=(const CNTEndCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTCtrlLeftCommand
class CNTCtrlLeftCommand : public CNTCommand {
public:
	CNTCtrlLeftCommand(TextEditingForm* textEditingForm = 0);
	CNTCtrlLeftCommand(const CNTCtrlLeftCommand& source);
	virtual ~CNTCtrlLeftCommand();
	CNTCtrlLeftCommand& operator=(const CNTCtrlLeftCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTCtrlRightCommand
class CNTCtrlRightCommand : public CNTCommand {
public:
	CNTCtrlRightCommand(TextEditingForm* textEditingForm = 0);
	CNTCtrlRightCommand(const CNTCtrlRightCommand& source);
	virtual ~CNTCtrlRightCommand();
	CNTCtrlRightCommand& operator=(const CNTCtrlRightCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTCtrlHomeCommand
class CNTCtrlHomeCommand : public CNTCommand {
public:
	CNTCtrlHomeCommand(TextEditingForm* textEditingForm = 0);
	CNTCtrlHomeCommand(const CNTCtrlHomeCommand& source);
	virtual ~CNTCtrlHomeCommand();
	CNTCtrlHomeCommand& operator=(const CNTCtrlHomeCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTCtrlEndCommand
class CNTCtrlEndCommand : public CNTCommand {
public:
	CNTCtrlEndCommand(TextEditingForm* textEditingForm = 0);
	CNTCtrlEndCommand(const CNTCtrlEndCommand& source);
	virtual ~CNTCtrlEndCommand();
	CNTCtrlEndCommand& operator=(const CNTCtrlEndCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTPageUpCommand
class CNTPageUpCommand : public CNTCommand {
public:
	CNTPageUpCommand(TextEditingForm* textEditingForm = 0);
	CNTPageUpCommand(const CNTPageUpCommand& source);
	virtual ~CNTPageUpCommand();
	CNTPageUpCommand& operator=(const CNTPageUpCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTPageDownCommand
class CNTPageDownCommand : public CNTCommand {
public:
	CNTPageDownCommand(TextEditingForm* textEditingForm = 0);
	CNTPageDownCommand(const CNTPageDownCommand& source);
	virtual ~CNTPageDownCommand();
	CNTPageDownCommand& operator=(const CNTPageDownCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};
//////////////////// Move ////////////////////

//////////////////// Select ////////////////////
//CNTShiftLeftCommand
class CNTShiftLeftCommand : public CNTCommand {
public:
	CNTShiftLeftCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftLeftCommand(const CNTShiftLeftCommand& source);
	virtual ~CNTShiftLeftCommand();
	CNTShiftLeftCommand& operator=(const CNTShiftLeftCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftRightCommand
class CNTShiftRightCommand : public CNTCommand {
public:
	CNTShiftRightCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftRightCommand(const CNTShiftRightCommand& source);
	virtual ~CNTShiftRightCommand();
	CNTShiftRightCommand& operator=(const CNTShiftRightCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftUpCommand
class CNTShiftUpCommand : public CNTCommand {
public:
	CNTShiftUpCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftUpCommand(const CNTShiftUpCommand& source);
	virtual ~CNTShiftUpCommand();
	CNTShiftUpCommand& operator=(const CNTShiftUpCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftDownCommand
class CNTShiftDownCommand : public CNTCommand {
public:
	CNTShiftDownCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftDownCommand(const CNTShiftDownCommand& source);
	virtual ~CNTShiftDownCommand();
	CNTShiftDownCommand& operator=(const CNTShiftDownCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftHomeCommand
class CNTShiftHomeCommand : public CNTCommand {
public:
	CNTShiftHomeCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftHomeCommand(const CNTShiftHomeCommand& source);
	virtual ~CNTShiftHomeCommand();
	CNTShiftHomeCommand& operator=(const CNTShiftHomeCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftEndCommand
class CNTShiftEndCommand : public CNTCommand {
public:
	CNTShiftEndCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftEndCommand(const CNTShiftEndCommand& source);
	virtual ~CNTShiftEndCommand();
	CNTShiftEndCommand& operator=(const CNTShiftEndCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftCtrlLeftCommand
class CNTShiftCtrlLeftCommand : public CNTCommand {
public:
	CNTShiftCtrlLeftCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlLeftCommand(const CNTShiftCtrlLeftCommand& source);
	virtual ~CNTShiftCtrlLeftCommand();
	CNTShiftCtrlLeftCommand& operator=(const CNTShiftCtrlLeftCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftCtrlRightCommand
class CNTShiftCtrlRightCommand : public CNTCommand {
public:
	CNTShiftCtrlRightCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlRightCommand(const CNTShiftCtrlRightCommand& source);
	virtual ~CNTShiftCtrlRightCommand();
	CNTShiftCtrlRightCommand& operator=(const CNTShiftCtrlRightCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftCtrlHomeCommand
class CNTShiftCtrlHomeCommand : public CNTCommand {
public:
	CNTShiftCtrlHomeCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlHomeCommand(const CNTShiftCtrlHomeCommand& source);
	virtual ~CNTShiftCtrlHomeCommand();
	CNTShiftCtrlHomeCommand& operator=(const CNTShiftCtrlHomeCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTShiftCtrlEndCommand
class CNTShiftCtrlEndCommand : public CNTCommand {
public:
	CNTShiftCtrlEndCommand(TextEditingForm* textEditingForm = 0);
	CNTShiftCtrlEndCommand(const CNTShiftCtrlEndCommand& source);
	virtual ~CNTShiftCtrlEndCommand();
	CNTShiftCtrlEndCommand& operator=(const CNTShiftCtrlEndCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};
//////////////////// Select ////////////////////

//////////////////// Flag ////////////////////
//CNTLockHScrollCommand
class CNTLockHScrollCommand : public CNTCommand {
public:
	CNTLockHScrollCommand(TextEditingForm* textEditingForm = 0);
	CNTLockHScrollCommand(const CNTLockHScrollCommand& source);
	virtual ~CNTLockHScrollCommand();
	CNTLockHScrollCommand& operator=(const CNTLockHScrollCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTUnlockHistoryBookCommand
class CNTUnlockHistoryBookCommand : public CNTCommand {
public:
	CNTUnlockHistoryBookCommand(TextEditingForm* textEditingForm = 0);
	CNTUnlockHistoryBookCommand(const CNTUnlockHistoryBookCommand& source);
	virtual ~CNTUnlockHistoryBookCommand();
	CNTUnlockHistoryBookCommand& operator=(const CNTUnlockHistoryBookCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};

//CNTUnlockFindReplaceDialogCommand
class CNTUnlockFindReplaceDialogCommand : public CNTCommand {
public:
	CNTUnlockFindReplaceDialogCommand(TextEditingForm* textEditingForm = 0);
	CNTUnlockFindReplaceDialogCommand(const CNTUnlockFindReplaceDialogCommand& source);
	virtual ~CNTUnlockFindReplaceDialogCommand();
	CNTUnlockFindReplaceDialogCommand& operator=(const CNTUnlockFindReplaceDialogCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual CNTCommand* Clone();
};
//////////////////// Flag ////////////////////

#endif //_COMMAND_H