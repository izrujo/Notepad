//Commands.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : 
MVC : ?
Author : Chan Hyung Kim
Written Date : 2019.05
Recently Updated : 2020.07.17
**************************************************/

#ifndef _COMMAND_H
#define _COMMAND_H

#include <afxwin.h>
#include <iostream>
#include "Array.h"
using namespace std;

class NotepadForm;
class Glyph;
class Note;

//Command
class Command {
public:
	Command(NotepadForm* notepadForm = 0);
	Command(const Command& source);
	virtual ~Command() = 0;
	Command& operator=(const Command& source);

	virtual void Execute() = 0;
	virtual void Unexecute();

	virtual string GetType() = 0;
	virtual Command* Clone() = 0;

	//MacroCommand
	virtual Long Add(Command* command);
	virtual Long Remove(Long index);
	virtual Command* GetAt(Long index);

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;

protected:
	NotepadForm* notepadForm;
};

/////////////////Composite////////////////////////
//MacroCommand
class MacroCommand : public Command {
public:
	MacroCommand(NotepadForm* notepadForm = 0, Long capacity = 10);
	MacroCommand(const MacroCommand& source);
	virtual ~MacroCommand();
	MacroCommand& operator=(const MacroCommand& source);

	virtual void Execute();
	virtual void Unexecute();

	virtual Long Add(Command* command);
	virtual Long Remove(Long index);
	virtual Command* GetAt(Long index);

	virtual string GetType();
	virtual Command* Clone();

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;

protected:
	Array<Command*> commands;
	Long capacity;
	Long length;
};

inline Long MacroCommand::GetCapacity() const {
	return this->capacity;
}

inline Long MacroCommand::GetLength() const {
	return this->length;
}
/////////////////Composite////////////////////////

//FontCommand
class FontCommand : public Command {
public:
	FontCommand(NotepadForm *notepadForm = 0);
	FontCommand(const FontCommand& source);
	~FontCommand();
	FontCommand& operator=(const FontCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//NewCommand
class NewCommand : public Command {
public:
	NewCommand(NotepadForm *notepadFOrm = 0);
	NewCommand(const NewCommand& source);
	virtual ~NewCommand();
	NewCommand& operator=(const NewCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//OpenCommand
class OpenCommand : public Command {
public:
	OpenCommand(NotepadForm *notepadForm = 0);
	OpenCommand(const OpenCommand& source);
	virtual ~OpenCommand();
	OpenCommand& operator=(const OpenCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//SaveCommand
class SaveCommand : public Command {
public:
	SaveCommand(NotepadForm* notepadForm = 0);
	SaveCommand(const SaveCommand& source);
	virtual ~SaveCommand();
	SaveCommand& operator=(const SaveCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//SaveAsCommand
class SaveAsCommand : public Command {
public:
	SaveAsCommand(NotepadForm* notepadForm = 0);
	SaveAsCommand(const SaveAsCommand& source);
	virtual ~SaveAsCommand();
	SaveAsCommand& operator=(const SaveAsCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CloseCommand
class CloseCommand : public Command {
public:
	CloseCommand(NotepadForm* notepadForm = 0);
	CloseCommand(const CloseCommand& source);
	virtual ~CloseCommand();
	CloseCommand& operator=(const CloseCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//WriteCommand
class WriteCommand : public Command {
public:
	WriteCommand(NotepadForm* notepadForm = 0);
	WriteCommand(const WriteCommand& source);
	virtual ~WriteCommand();
	WriteCommand& operator=(const WriteCommand& source);

	virtual void Execute();
	virtual void Unexecute();
	virtual string GetType();
	virtual Command* Clone();

private:
	int nChar;
	Long row;
	Long column;
};

//ImeCompositionCommand
class ImeCompositionCommand : public Command {
public:
	ImeCompositionCommand(NotepadForm* notepadForm = 0);
	ImeCompositionCommand(const ImeCompositionCommand& source);
	virtual ~ImeCompositionCommand();
	ImeCompositionCommand& operator=(const ImeCompositionCommand& source);

	virtual void Execute();
	virtual void Unexecute(); //임시?
	virtual string GetType();
	virtual Command* Clone();
};

//ImeCharCommand
class ImeCharCommand : public Command {
public:
	ImeCharCommand(NotepadForm* notepadForm = 0);
	ImeCharCommand(const ImeCharCommand& source);
	virtual ~ImeCharCommand();
	ImeCharCommand& operator=(const ImeCharCommand& source);

	virtual void Execute();
	virtual void Unexecute(); //임시?
	virtual string GetType();
	virtual Command* Clone();

private:
	TCHAR(*buffer);
	Long row;
	Long column;
};

//DeleteCommand
class DeleteCommand : public Command {
public:
	DeleteCommand(NotepadForm* notepadForm = 0);
	DeleteCommand(const DeleteCommand& source);
	virtual ~DeleteCommand();
	DeleteCommand& operator=(const DeleteCommand& source);

	virtual void Execute();
	virtual void Unexecute();
	virtual string GetType();
	virtual Command* Clone();

private:
	Long row;
	Long noteLength;
	Long column;
	Long lineLength;
	Glyph* character;
};
#if 0
//BackspaceCommand
class BackspaceCommand : public Command {
public:
	BackspaceCommand(NotepadForm* notepadForm = 0);
	BackspaceCommand(const BackspaceCommand& source);
	virtual ~BackspaceCommand();
	BackspaceCommand& operator=(const BackspaceCommand& source);

	virtual void Execute();
	virtual void Unexecute();
	virtual string GetType();
	virtual Command* Clone();

private:
	Long row;
	Long column;
	Long preLineLength;
	Glyph* character;
};
#endif
//CopyCommand
class CopyCommand : public Command {
public:
	CopyCommand(NotepadForm* notepadForm = 0);
	CopyCommand(const CopyCommand& source);
	virtual ~CopyCommand();
	CopyCommand& operator=(const CopyCommand& source);

	virtual void Execute();
	virtual void Unexecute(); //임시?
	virtual string GetType();
	virtual Command* Clone();
};

//PasteCommand
class PasteCommand : public Command {
public:
	PasteCommand(NotepadForm* notepadForm = 0);
	PasteCommand(const PasteCommand& source);
	virtual ~PasteCommand();
	PasteCommand& operator=(const PasteCommand& source);

	virtual void Execute();
	virtual void Unexecute(); //임시?
	virtual string GetType();
	virtual Command* Clone();

private:
	MacroCommand* macroCommand;
};

//CutCommand
class CutCommand : public Command {
public:
	CutCommand(NotepadForm* notepadForm = 0);
	CutCommand(const CutCommand& source);
	virtual ~CutCommand();
	CutCommand& operator=(const CutCommand& source);

	virtual void Execute();
	virtual void Unexecute();
	virtual string GetType();
	virtual Command* Clone();

private:
	MacroCommand* macroCommand;
};

//SelectAllCommand
class SelectAllCommand : public Command {
public:
	SelectAllCommand(NotepadForm* notepadForm = 0);
	SelectAllCommand(const SelectAllCommand& source);
	virtual ~SelectAllCommand();
	SelectAllCommand& operator=(const SelectAllCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//DeleteSelectionCommand
class DeleteSelectionCommand : public Command {
public:
	DeleteSelectionCommand(NotepadForm* notepadForm = 0);
	DeleteSelectionCommand(const DeleteSelectionCommand& source);
	virtual ~DeleteSelectionCommand();
	DeleteSelectionCommand& operator=(const DeleteSelectionCommand& source);

	virtual void Execute();
	virtual void Unexecute();
	virtual string GetType();
	virtual Command* Clone();

private:
	MacroCommand* macroCommand;
};

//UndoCommand
class UndoCommand : public Command {
public:
	UndoCommand(NotepadForm* notepadForm = 0);
	UndoCommand(const UndoCommand& source);
	virtual ~UndoCommand();
	UndoCommand& operator=(const UndoCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//RedoCommand
class RedoCommand : public Command {
public:
	RedoCommand(NotepadForm* notepadForm = 0);
	RedoCommand(const RedoCommand& source);
	virtual ~RedoCommand();
	RedoCommand& operator=(const RedoCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//LeftCommand
class LeftCommand : public Command {
public:
	LeftCommand(NotepadForm* notepadForm = 0);
	LeftCommand(const LeftCommand& source);
	virtual ~LeftCommand();
	LeftCommand& operator=(const LeftCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//RightCommand
class RightCommand : public Command {
public:
	RightCommand(NotepadForm* notepadForm = 0);
	RightCommand(const RightCommand& source);
	virtual ~RightCommand();
	RightCommand& operator=(const RightCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

#endif //_COMMAND_H