#ifndef _WORDWRAPCHECKEDCOMMAND_H
#define _WORDWRAPCHECKEDCOMMAND_H

#include "Command.h"
class NotepadForm;

class WordWrapCheckedCommand : public Command {
public:
	WordWrapCheckedCommand(NotepadForm *notepadForm = 0);
	WordWrapCheckedCommand(const WordWrapCheckedCommand& source);
	virtual ~WordWrapCheckedCommand();
	virtual void Execute();

	WordWrapCheckedCommand& operator =(const WordWrapCheckedCommand& source);
};

#endif //_WORDWRAPCHECKEDCOMMAND_H