#ifndef _NOTE_H
#define _NOTE_H

#include "Composite.h"
#include <string>

typedef signed long int Long;
using namespace std;

class Glyph;
class Visitor;

class Note : public Composite {
public:
	Note(Long capacity = 256);
	Note(const Note& source);
	virtual ~Note();
	Note& operator=(const Note& source);

	virtual Long Add(Glyph* glyph);
	virtual Long Add(Long index, Glyph* glyph);
	virtual Long Remove(Long index);
	virtual Glyph* Divide(Long index);

	virtual Long Next();
	virtual Long Last();
	virtual Long MovePreviousWord();
	virtual Long MoveNextWord();
	
	virtual bool IsFirst();
	virtual bool IsLast();

	virtual void UnselectAll();
	virtual bool IsSelecting();
	virtual string GetSelectedContent(Long start, Long end);
	virtual Long Select(Long start, Long startColumn, Long end, Long endColumn);
	virtual Long GetSelectedStartColumn(Long start);
	virtual Long GetSelectedEndColumn(Long end);

	virtual Glyph* Clone();
	virtual string GetContent();
	virtual string GetContent(Long startRow, Long startColumnn, Long endRow, Long endColumn);
	virtual void Accept(Visitor* visitor);
};

#endif //_NOTE_H