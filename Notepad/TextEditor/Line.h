#ifndef _LINE_H
#define _LINE_H

#include "Composite.h"
#include <string>

typedef signed long int Long;
using namespace std;

class Glyph;
class Visitor;

class Line : public Composite {
public:
	Line(Long capacity = 256);
	Line(const Line& source);
	virtual ~Line();
	Line& operator=(const Line& source);

	virtual Glyph* Divide(Long index);
	virtual Glyph* Combine(Glyph* other);

	virtual Long MovePreviousWord();
	virtual Long MoveNextWord();

	virtual bool IsFirst();
	virtual bool IsLast();

	virtual void UnselectAll();
	virtual bool IsSelecting();
	virtual Long Select(Long start, Long end);

	virtual Glyph* Clone();
	virtual string GetContent();
	virtual void Accept(Visitor* visitor);
};

#endif //_LINE_H