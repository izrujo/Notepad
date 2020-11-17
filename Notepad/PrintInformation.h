#ifndef _PRINTINFORMATION_H
#define _PRINTINFORMATION_H

#include <afxwin.h>

class NotepadForm;
class Glyph;
class CharacterMetrics;
class Font;

class PrintInformation {
public:
	PrintInformation(NotepadForm* notepadForm = 0, Glyph* note = 0);
	~PrintInformation();

public:
	Glyph* book;
	CharacterMetrics* characterMetrics;
	Font* font;
	CDC printerDC;
};

#endif //_PRINTINFORMATION_H