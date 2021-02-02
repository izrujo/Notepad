#ifndef _VSCROLLFACTORY_H
#define _VSCROLLFACTORY_H

#include <afxwin.h>

class VScrollAction;
class TextEditingForm;

class VScrollActionFactory {
public:
	VScrollActionFactory(TextEditingForm *textEditingForm = 0);
	VScrollActionFactory(const VScrollActionFactory& source);
	~VScrollActionFactory();
	VScrollActionFactory& operator=(const VScrollActionFactory& source);

	VScrollAction* Make(UINT nSBCode);
private:
	TextEditingForm *textEditingForm;
};

#endif // !_VSCROLLFACTORY_H
