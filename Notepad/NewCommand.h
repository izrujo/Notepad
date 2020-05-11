#ifndef _NEWCOMMAND_H
#define _NEWCOMMAND_H

#include "Command.h"
class NotepadForm;

class NewCommand : public Command {
public:
	NewCommand(NotepadForm *notepadFOrm = 0);
	NewCommand(const NewCommand& source);
	virtual ~NewCommand();
	virtual void Execute();

	NewCommand& operator =(const NewCommand& source);
};

#endif //_NEWCOMMAND_H