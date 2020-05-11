#ifndef _DUMMYLINE_H
#define _DUMMYLINE_H

#include "Line.h"
#include <string>

class DummyLine : public Line {
public:
	DummyLine(Long capacity = 256);
	//DummyLine(Glyph *line);
	DummyLine(const DummyLine& source);
	~DummyLine();
	DummyLine& operator =(const DummyLine& source);

	virtual string GetContent();
	virtual Glyph* Clone();
};

#endif //_DUMMYLINE_H