#ifndef _SAVEASCOMMAND_H
#define _SAVEASCOMMAND_H

#include "Command.h"
class NotepadForm;

class SaveAsCommand : public Command {
public:
	SaveAsCommand(NotepadForm *notepadForm = 0);
	SaveAsCommand(const SaveAsCommand& source);
	virtual ~SaveAsCommand();
	virtual void Execute();

	SaveAsCommand& operator =(const SaveAsCommand& source);
};

#endif //_SAVEASCOMMAND_H