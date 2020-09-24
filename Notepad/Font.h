#ifndef _Font_H
#define _Font_H

#include "CHWindowForm.h"

class Font {
public:
	Font(CHWindowForm *window = 0);
	Font(LOGFONT Font, COLORREF color, CWnd* window = 0);
	Font(const Font& source);
	~Font();
	Font& operator=(const Font& source);

	void Create(CFont& Font);

	LOGFONT& GetFont() const;
	COLORREF& GetColor() const;
private:
	LOGFONT font;
	COLORREF color;
	CWnd* window;
};

inline LOGFONT& Font::GetFont() const {
	return const_cast<LOGFONT&>(this->font);
}

inline COLORREF& Font::GetColor() const {
	return const_cast<COLORREF&>(this->color);
}

#endif //_Font_H