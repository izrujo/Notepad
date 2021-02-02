#ifndef _PRINTINGVISITOR_H
#define _PRINTINGVISITOR_H

#include "../TextEditor/Visitor.h"
#include <afxwin.h>

typedef signed long int Long;

class CharacterMetrics;

class PrintingVisitor : public Visitor {
public:
	PrintingVisitor(CDC* dc = 0, CharacterMetrics* characterMetrics = 0, Long x = 0, Long y = 0);
	virtual ~PrintingVisitor();

	virtual void Visit(Note* note);
	virtual void Visit(Line* line);
	virtual void Visit(Character* character);
	virtual void Visit(Book* book); //not implemented

private:
	CDC* dc;
	CharacterMetrics* characterMetrics;
	Long x;
	Long y;
};

#endif //_PRINTINGVISITOR_H