#ifndef _CHWINDOWFORM_H
#define _CHWINDOWFORM_H

#include "afxwin.h"

class Font;
class CharacterMetrics;

class CHWindowForm : public CFrameWnd {
public:
	CHWindowForm();
	virtual ~CHWindowForm() = 0;
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	Font* font;
	CharacterMetrics* characterMetrics;
};

#endif //_CHWINDOWFORM_H