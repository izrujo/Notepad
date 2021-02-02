#ifndef _NOTE_H
#define _NOTE_H

#include "../TextEditor/Composite.h"
#include <string>

typedef signed long int Long;
using namespace std;

class Glyph;
class Visitor;

class Book : public Composite {
public:
	Book(Long capacity = 256);
	Book(const Book& source);
	virtual ~Book();
	Book& operator=(const Book& source);

	virtual Long Add(Glyph* glyph);
	virtual Long Add(Long index, Glyph* glyph);
	virtual Long Remove(Long index);
	virtual Long Next();

	virtual Glyph* Clone();
	virtual string GetContent();
	virtual void Accept(Visitor* visitor);
};

#endif //_NOTE_H