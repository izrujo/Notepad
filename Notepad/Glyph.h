#ifndef _GLYPH_H
#define _GLYPH_H

#include <string>
using namespace std;
typedef signed long int Long;

class Visitor;

class Glyph {
public:
	Glyph();
	Glyph(const Glyph& source);
	virtual ~Glyph() = 0; //abstract
	Glyph& operator=(const Glyph& source);

	virtual Long Add(Glyph* glyph); //virtual
	virtual Long Add(Long index, Glyph* glyph);
	virtual Long Remove(Long index); //virtual
	virtual Glyph* GetAt(Long index); //virtual
	virtual Glyph* Divide(Long index);
	virtual Glyph* Combine(Glyph* index);

	virtual Long First();
	virtual Long Last();
	virtual Long Previous();
	virtual Long Next();
	virtual Long Move(Long index);
	virtual Long MovePreviousWord();
	virtual Long MoveNextWord();

	virtual void UnselectAll();
	virtual bool IsSelecting();

	virtual void Select(bool isSelected); //virtual

	virtual Glyph* Clone() = 0; //abstract
	virtual string GetContent() = 0; //abstract
	virtual void Accept(Visitor* visitor) = 0;

	virtual Long GetCapacity() const; //virtual
	virtual Long GetLength() const; //virtual
	virtual Long GetCurrent() const; //virtual
	virtual bool GetIsSelected() const; //virtual 
};

#endif //_GLYPH_H