#ifndef _SAVECOMMAND_H
#define _SAVECOMMAND_H

#include "Command.h"
class NotepadForm;

class SaveCommand : public Command {
public:
	SaveCommand(NotepadForm *notepadForm = 0);
	SaveCommand(const SaveCommand& source);
	virtual ~SaveCommand();
	virtual void Execute();

	SaveCommand& operator =(const SaveCommand& source);
};

#endif //_SAVECOMMAND_H