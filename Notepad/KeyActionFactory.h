#ifndef KEYACTIONFACTORY_H
#define KEYACTIONFACTORY_H

#include <afxwin.h>

class KeyAction;
class NotepadForm;

class KeyActionFactory {
public:
	KeyActionFactory(NotepadForm *notepadForm = 0);
	KeyActionFactory(const KeyActionFactory& source);
	~KeyActionFactory();
	KeyActionFactory& operator=(const KeyActionFactory& source);

	KeyAction* Make(UINT nChar);
private:
	NotepadForm *notepadForm;
};

#endif // !KEYACTIONFACTORY_H