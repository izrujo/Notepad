//CaretController.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : Control caret object.
MVC : ?
Author : Chan Hyung Kim
Written Date : 2019.05
Recently Updated : 2019.05
**************************************************/

#ifndef _CARETCONTROLLER_H
#define _CARETCONTROLLER_H

#include "Observer.h"

typedef signed long int Long;
class NotepadForm;
class Caret;

class CaretController : public Observer {
public:
	CaretController(NotepadForm *notepadForm = 0);
	CaretController(const CaretController& source);
	virtual ~CaretController();
	CaretController& operator=(const CaretController& source);

	virtual void Update();

	Long GetCaretX();
	Long GetCaretY();
private:
	NotepadForm *notepadForm;
	Caret *caret;
};

#endif //_CARETCONTROLLER_H