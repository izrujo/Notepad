#ifndef _LINEDIVIDER_H
#define _LINEDIVIDER_H

typedef signed long int Long;
class CharacterMetrics;
class Glyph;

class LineDivider {
public:
	LineDivider(CharacterMetrics* characterMetrics);
	LineDivider(const LineDivider& source);
	~LineDivider();
	LineDivider& operator=(const LineDivider& source);

	//returned Glyph : remained note from note until clientHeight
	Glyph* Divide(Long clientWidth, Long clientHeight, Glyph* note, Long row, Long column);
	void Combine(Glyph* note, Long* row, Long* column);
private:
	CharacterMetrics* characterMetrics;
};

#endif //_LINEDIVIDER_H