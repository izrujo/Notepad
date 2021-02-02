//CharacterMetrics.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : Manage text size data.
MVC : ?
Author : Chan Hyung Kim
Written Date : 2019.05
Recently Updated : 2019.05
**************************************************/

#ifndef CHARACTERMETRICS_H
#define CHARACTERMETRICS_H

#include <string>

#include <afxwin.h>

using namespace std;
typedef signed long int Long;
class Glyph;
class Font;

class CharacterMetrics {
public:
	CharacterMetrics(CWnd* window = 0, Font *font = 0);
	CharacterMetrics(const CharacterMetrics& source);
	~CharacterMetrics();
	Long GetX(Glyph* line);
	Long GetX(Glyph* line, Long index);
	Long GetX(const string& buffer);
	Long GetY(Long index);
	Long GetColumn(Glyph* line, Long x);
	Long GetColumnForDrawing(Glyph* line, Long x);
	Long GetRow(Long y);
	Long GetNoteWidth(Glyph* note);
	Long GetWidthAverage();

	CharacterMetrics& operator=(const CharacterMetrics& source);

	Long GetWidth(Long index) const;
	Long GetDoubleByteWidth() const;
	Long GetHeight() const;
private:
	CWnd* window;
	Font* font;
	Long widths[129];
	Long height;
};

inline Long CharacterMetrics::GetWidth(Long index) const {
	return this->widths[index];
}

inline Long CharacterMetrics::GetDoubleByteWidth() const {
	return this->widths[128];
}

inline Long CharacterMetrics::GetHeight() const {
	return this->height;
}

#endif // !CHARACTERMETRICS_H
