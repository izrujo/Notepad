#ifndef _DUMMYLINE_H
#define _DUMMYLINE_H

#include "Line.h"
#include <string>

class DummyLine : public Line {
public:
	DummyLine(Long capacity = 256);
	DummyLine(const DummyLine& source);
	~DummyLine();
	DummyLine& operator=(const DummyLine& source);

	virtual Glyph* Clone();
	virtual string GetContent();
};

#endif //_DUMMYLINE_H