#ifndef _VSCROLLFACTORY_H
#define _VSCROLLFACTORY_H

#include <afxwin.h>

class VScrollAction;
class NotepadForm;

class VScrollActionFactory {
public:
	VScrollActionFactory(NotepadForm *notepadForm = 0);
	VScrollActionFactory(const VScrollActionFactory& source);
	~VScrollActionFactory();
	VScrollActionFactory& operator=(const VScrollActionFactory& source);

	VScrollAction* Make(UINT nSBCode);
private:
	NotepadForm *notepadForm;
};

#endif // !_VSCROLLFACTORY_H
