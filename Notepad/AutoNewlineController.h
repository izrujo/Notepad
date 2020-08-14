#ifndef _AUTONEWLINECONTROLLER_H
#define _AUTONEWLINECONTROLLER_H

#include "Observer.h"
class NotepadForm;

class AutoNewlineController : public Observer {
public:
	AutoNewlineController(NotepadForm* notepadForm = 0);
	AutoNewlineController(const AutoNewlineController& source);
	virtual ~AutoNewlineController();
	AutoNewlineController& operator=(const AutoNewlineController& source);

	virtual void Update();
	void Release();

private:
	NotepadForm* notepadForm;
};

#endif //_AUTONEWLINECONTROLLER_H