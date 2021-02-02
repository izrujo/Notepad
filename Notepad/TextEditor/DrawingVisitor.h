#ifndef _DRAWINGVISITOR_H
#define _DRAWINGVISITOR_H

#include "Visitor.h"
#include <afxwin.h>

typedef signed long int Long;

class CharacterMetrics;
class ScrollController;
//
class Selection;

class DrawingVisitor : public Visitor {
public:
	DrawingVisitor(CDC *dc = 0, CharacterMetrics* characterMetrics = 0, ScrollController* scrollController = 0, Selection* selection = 0);
	virtual ~DrawingVisitor();

	virtual void Visit(Note* note);
	virtual void Visit(Line* line);
	virtual void Visit(Character* character);
	virtual void Visit(Book* book); //Not implement

private:
	CDC* dc;
	CharacterMetrics* characterMetrics;
	ScrollController* scrollController;
	//
	Selection* selection;
	Long x;
	Long y;
};

#endif //_DRAWINGVISITOR_H