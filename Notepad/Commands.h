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

#endif //_COMMAND_H