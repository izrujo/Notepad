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

class NotepadForm;

//Command
class Command {
public:
	Command(NotepadForm* notepadForm = 0);
	Command(const Command& source);
	virtual ~Command() = 0;
	Command& operator=(const Command& source);

	virtual void Execute() = 0;
protected:
	NotepadForm* notepadForm;
};

//FontCommand
class FontCommand : public Command {
public:
	FontCommand(NotepadForm *notepadForm = 0);
	FontCommand(const FontCommand& source);
	~FontCommand();
	FontCommand& operator=(const FontCommand& source);

	virtual void Execute();
};

//NewCommand
class NewCommand : public Command {
public:
	NewCommand(NotepadForm *notepadFOrm = 0);
	NewCommand(const NewCommand& source);
	virtual ~NewCommand();
	NewCommand& operator=(const NewCommand& source);

	virtual void Execute();

};

//OpenCommand
class OpenCommand : public Command {
public:
	OpenCommand(NotepadForm *notepadForm = 0);
	OpenCommand(const OpenCommand& source);
	virtual ~OpenCommand();
	OpenCommand& operator=(const OpenCommand& source);

	virtual void Execute();
};

//SaveCommand
class SaveCommand : public Command {
public:
	SaveCommand(NotepadForm* notepadForm = 0);
	SaveCommand(const SaveCommand& source);
	virtual ~SaveCommand();
	SaveCommand& operator=(const SaveCommand& source);

	virtual void Execute();
};

//SaveAsCommand
class SaveAsCommand : public Command {
public:
	SaveAsCommand(NotepadForm* notepadForm = 0);
	SaveAsCommand(const SaveAsCommand& source);
	virtual ~SaveAsCommand();
	SaveAsCommand& operator=(const SaveAsCommand& source);

	virtual void Execute();
};

//CloseCommand
class CloseCommand : public Command {
public:
	CloseCommand(NotepadForm* notepadForm = 0);
	CloseCommand(const CloseCommand& source);
	virtual ~CloseCommand();
	CloseCommand& operator=(const CloseCommand& source);

	virtual void Execute();
};

//WriteCommand
class WriteCommand : public Command {
public:
	WriteCommand(NotepadForm* notepadForm = 0);
	WriteCommand(const WriteCommand& source);
	virtual ~WriteCommand();
	WriteCommand& operator=(const WriteCommand& source);

	virtual void Execute();
};

//ImeCompositionCommand
class ImeCompositionCommand : public Command {
public:
	ImeCompositionCommand(NotepadForm* notepadForm = 0);
	ImeCompositionCommand(const ImeCompositionCommand& source);
	virtual ~ImeCompositionCommand();
	ImeCompositionCommand& operator=(const ImeCompositionCommand& source);

	virtual void Execute();
};

//ImeCharCommand
class ImeCharCommand : public Command {
public:
	ImeCharCommand(NotepadForm* notepadForm = 0);
	ImeCharCommand(const ImeCharCommand& source);
	virtual ~ImeCharCommand();
	ImeCharCommand& operator=(const ImeCharCommand& source);

	virtual void Execute();
};

//DeleteCommand
class DeleteCommand : public Command {
public:
	DeleteCommand(NotepadForm* notepadForm = 0);
	DeleteCommand(const DeleteCommand& source);
	virtual ~DeleteCommand();
	DeleteCommand& operator=(const DeleteCommand& source);

	virtual void Execute();
};

//BackspaceCommand
class BackspaceCommand : public Command {
public:
	BackspaceCommand(NotepadForm* notepadForm = 0);
	BackspaceCommand(const BackspaceCommand& source);
	virtual ~BackspaceCommand();
	BackspaceCommand& operator=(const BackspaceCommand& source);

	virtual void Execute();
};

//CopyCommand
class CopyCommand : public Command {
public:
	CopyCommand(NotepadForm* notepadForm = 0);
	CopyCommand(const CopyCommand& source);
	virtual ~CopyCommand();
	CopyCommand& operator=(const CopyCommand& source);

	virtual void Execute();
};

//PasteCommand
class PasteCommand : public Command {
public:
	PasteCommand(NotepadForm* notepadForm = 0);
	PasteCommand(const PasteCommand& source);
	virtual ~PasteCommand();
	PasteCommand& operator=(const PasteCommand& source);

	virtual void Execute();
};

//CutCommand
class CutCommand : public Command {
public:
	CutCommand(NotepadForm* notepadForm = 0);
	CutCommand(const CutCommand& source);
	virtual ~CutCommand();
	CutCommand& operator=(const CutCommand& source);

	virtual void Execute();
};

//SelectAllCommand
class SelectAllCommand : public Command {
public:
	SelectAllCommand(NotepadForm* notepadForm = 0);
	SelectAllCommand(const SelectAllCommand& source);
	virtual ~SelectAllCommand();
	SelectAllCommand& operator=(const SelectAllCommand& source);

	virtual void Execute();
};

//DeleteSelectionCommand
class DeleteSelectionCommand : public Command {
public:
	DeleteSelectionCommand(NotepadForm* notepadForm = 0);
	DeleteSelectionCommand(const DeleteSelectionCommand& source);
	virtual ~DeleteSelectionCommand();
	DeleteSelectionCommand& operator=(const DeleteSelectionCommand& source);

	virtual void Execute();
};

#endif //_COMMAND_H