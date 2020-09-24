#ifndef _PREVIEWFORM_H
#define _PREVIEWFORM_H

#include "CHWindowForm.h"
#include <afxext.h>

class Glyph;

class PreviewForm : public CHWindowForm { ///600 dpi �������� ����Ʈ�� ����.
public:
	PreviewForm(CWnd* parent = NULL, Glyph* note = 0);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnCommandRange(UINT uID);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* parent;
	Glyph* note;
	Glyph* book;
	CToolBar toolBar;
	CDC printerDC;
};

#endif //_PREVIEWFORM_H