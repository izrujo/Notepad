#ifndef _WORDWRAPUNCHECKEDCOMMAND_H
#define _WORDWRAPUNCHECKEDCOMMAND_H

#include "Command.h"
class NotepadForm;

class WordWrapUnCheckedCommand : public Command {
public:
	WordWrapUnCheckedCommand(NotepadForm *notepadForm = 0);
	WordWrapUnCheckedCommand(const WordWrapUnCheckedCommand& source);
	virtual ~WordWrapUnCheckedCommand();
	virtual void Execute();

	WordWrapUnCheckedCommand& operator =(const WordWrapUnCheckedCommand& source);
};

#endif //_WORDWRAPUNCHECKEDCOMMAND_H