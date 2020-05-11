#ifndef _WORDWRAPCONTROLLER_H
#define _WORDWRAPCONTROLLER_H

#include "Observer.h"

class NotepadForm;

class WordWrapController : public Observer {
public:
	WordWrapController(NotepadForm *notepadForm = 0);
	WordWrapController(const WordWrapController& source);
	virtual ~WordWrapController();
	virtual void Update();

	WordWrapController& operator =(const WordWrapController& source);
private:
	NotepadForm *notepadForm;
};

#endif //_WORDWRAPCONTROLLER_H