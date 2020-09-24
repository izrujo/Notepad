#include "Font.h"

Font::Font(CHWindowForm* window) {
	this->window = window;
	CDC* dc = this->window->GetDC();
	CFont* dcFont = dc->GetCurrentFont();
	dcFont->GetLogFont(&this->font);
	strcpy_s(this->font.lfFaceName, "±¼¸²");
	this->color = 0x00000000;
}

Font::Font(LOGFONT Font, COLORREF color, CWnd* window) {
	this->font = Font;
	this->color = color;
	this->window = window;
}

Font::Font(const Font& source) {
	this->font = source.font;
	this->color = source.color;
	this->window = source.window;
}

Font::~Font() {

}

Font& Font::operator= (const Font& source) {
	this->font = source.font;
	this->color = source.color;
	this->window = source.window;

	return *this;
}

void Font::Create(CFont& font) {
	font.CreateFontIndirectA(&this->font);
}