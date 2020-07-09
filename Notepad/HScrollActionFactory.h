#ifndef _HSCROLLFACTORY_H
#define _HSCROLLFACTORY_H

#include <afxwin.h>

class HScrollAction;
class NotepadForm;

class HScrollActionFactory {
public:
	HScrollActionFactory(NotepadForm *notepadForm = 0);
	HScrollActionFactory(const HScrollActionFactory& source);
	~HScrollActionFactory();
	HScrollActionFactory& operator=(const HScrollActionFactory& source);

	HScrollAction* Make(UINT nSBCode);
private:
	NotepadForm *notepadForm;
};

#endif // !_HSCROLLFACTORY_H
