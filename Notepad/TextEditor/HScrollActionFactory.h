#ifndef _HSCROLLFACTORY_H
#define _HSCROLLFACTORY_H

#include <afxwin.h>

class HScrollAction;
class TextEditingForm;

class HScrollActionFactory {
public:
	HScrollActionFactory(TextEditingForm *textEditingForm = 0);
	HScrollActionFactory(const HScrollActionFactory& source);
	~HScrollActionFactory();
	HScrollActionFactory& operator=(const HScrollActionFactory& source);

	HScrollAction* Make(UINT nSBCode);
private:
	TextEditingForm *textEditingForm;
};

#endif // !_HSCROLLFACTORY_H