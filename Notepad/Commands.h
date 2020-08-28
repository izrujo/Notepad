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

#include <afxwin.h> //TCHAR
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

private:
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

//AutoNewlineCommand
class AutoNewlineCommand : public Command {
public:
	AutoNewlineCommand(NotepadForm* notepadForm = 0);
	AutoNewlineCommand(const AutoNewlineCommand& source);
	~AutoNewlineCommand();
	AutoNewlineCommand& operator=(const AutoNewlineCommand& source);

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

//PageSetupCommand
class PageSetupCommand : public Command {
public:
	PageSetupCommand(NotepadForm* notepadForm = 0);
	PageSetupCommand(const PageSetupCommand& source);
	virtual ~PageSetupCommand();
	PageSetupCommand& operator=(const PageSetupCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PrintCommand
class PrintCommand : public Command {
public:
	PrintCommand(NotepadForm* notepadForm = 0);
	PrintCommand(const PrintCommand& source);
	virtual ~PrintCommand();
	PrintCommand& operator=(const PrintCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PreviewCommand
class PreviewCommand : public Command {
public:
	PreviewCommand(NotepadForm* notepadForm = 0);
	PreviewCommand(const PreviewCommand& source);
	virtual ~PreviewCommand();
	PreviewCommand& operator=(const PreviewCommand& source);

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
	virtual void Unexecute();
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

//CopyCommand
class CopyCommand : public Command {
public:
	CopyCommand(NotepadForm* notepadForm = 0);
	CopyCommand(const CopyCommand& source);
	virtual ~CopyCommand();
	CopyCommand& operator=(const CopyCommand& source);

	virtual void Execute();
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
	virtual void Unexecute();
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

//FindCommand
class FindCommand : public Command {
public:
	FindCommand(NotepadForm* notepadForm = 0);
	FindCommand(const FindCommand& source);
	virtual ~FindCommand();
	FindCommand& operator=(const FindCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//ReplaceCommand
class ReplaceCommand : public Command {
public:
	ReplaceCommand(NotepadForm* notepadForm = 0);
	ReplaceCommand(const ReplaceCommand& source);
	virtual ~ReplaceCommand();
	ReplaceCommand& operator=(const ReplaceCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//=============== Move Command ===============
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

//UpCommand
class UpCommand : public Command {
public:
	UpCommand(NotepadForm* notepadForm = 0);
	UpCommand(const UpCommand& source);
	virtual ~UpCommand();
	UpCommand& operator=(const UpCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//DownCommand
class DownCommand : public Command {
public:
	DownCommand(NotepadForm* notepadForm = 0);
	DownCommand(const DownCommand& source);
	virtual ~DownCommand();
	DownCommand& operator=(const DownCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//HomeCommand
class HomeCommand : public Command {
public:
	HomeCommand(NotepadForm* notepadForm = 0);
	HomeCommand(const HomeCommand& source);
	virtual ~HomeCommand();
	HomeCommand& operator=(const HomeCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//EndCommand
class EndCommand : public Command {
public:
	EndCommand(NotepadForm* notepadForm = 0);
	EndCommand(const EndCommand& source);
	virtual ~EndCommand();
	EndCommand& operator=(const EndCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CtrlLeftCommand
class CtrlLeftCommand : public Command {
public:
	CtrlLeftCommand(NotepadForm* notepadForm = 0);
	CtrlLeftCommand(const CtrlLeftCommand& source);
	virtual ~CtrlLeftCommand();
	CtrlLeftCommand& operator=(const CtrlLeftCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CtrlRightCommand
class CtrlRightCommand : public Command {
public:
	CtrlRightCommand(NotepadForm* notepadForm = 0);
	CtrlRightCommand(const CtrlRightCommand& source);
	virtual ~CtrlRightCommand();
	CtrlRightCommand& operator=(const CtrlRightCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CtrlHomeCommand
class CtrlHomeCommand : public Command {
public:
	CtrlHomeCommand(NotepadForm* notepadForm = 0);
	CtrlHomeCommand(const CtrlHomeCommand& source);
	virtual ~CtrlHomeCommand();
	CtrlHomeCommand& operator=(const CtrlHomeCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//CtrlEndCommand
class CtrlEndCommand : public Command {
public:
	CtrlEndCommand(NotepadForm* notepadForm = 0);
	CtrlEndCommand(const CtrlEndCommand& source);
	virtual ~CtrlEndCommand();
	CtrlEndCommand& operator=(const CtrlEndCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PageUpCommand
class PageUpCommand : public Command {
public:
	PageUpCommand(NotepadForm* notepadForm = 0);
	PageUpCommand(const PageUpCommand& source);
	virtual ~PageUpCommand();
	PageUpCommand& operator=(const PageUpCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//PageDownCommand
class PageDownCommand : public Command {
public:
	PageDownCommand(NotepadForm* notepadForm = 0);
	PageDownCommand(const PageDownCommand& source);
	virtual ~PageDownCommand();
	PageDownCommand& operator=(const PageDownCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//ShiftLeftCommand
class ShiftLeftCommand : public Command {
public:
	ShiftLeftCommand(NotepadForm* notepadForm = 0);
	ShiftLeftCommand(const ShiftLeftCommand& source);
	virtual ~ShiftLeftCommand();
	ShiftLeftCommand& operator=(const ShiftLeftCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};

//ShiftRightCommand
class ShiftRightCommand : public Command {
public:
	ShiftRightCommand(NotepadForm* notepadForm = 0);
	ShiftRightCommand(const ShiftRightCommand& source);
	virtual ~ShiftRightCommand();
	ShiftRightCommand& operator=(const ShiftRightCommand& source);

	virtual void Execute();
	virtual string GetType();
	virtual Command* Clone();
};
//=============== Move Command ===============

#endif //_COMMAND_H