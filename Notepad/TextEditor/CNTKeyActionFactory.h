#ifndef _CNTKEYACTIONFACTORY_H
#define _CNTKEYACTIONFACTORY_H

#include <afxwin.h>

class CNTKeyAction;
class TextEditingForm;

class CNTKeyActionFactory {
public:
	CNTKeyActionFactory(TextEditingForm *textEditingForm = 0);
	CNTKeyActionFactory(const CNTKeyActionFactory& source);
	~CNTKeyActionFactory();
	CNTKeyActionFactory& operator=(const CNTKeyActionFactory& source);

	CNTKeyAction* Make(UINT nChar);
private:
	TextEditingForm *textEditingForm;
};

#endif // _CNTKEYACTIONFACTORY_H