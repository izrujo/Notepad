#ifndef _PREVIEWFORM_H
#define _PREVIEWFORM_H

#include <afxwin.h>
#include <afxext.h>

class PrintInformation;

class PreviewForm : public CFrameWnd { ///600 dpi 기준으로 디폴트를 잡자.
public:
	PreviewForm(CWnd* parent = NULL);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnCommandRange(UINT uID);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* parent;
	CToolBar toolBar;
	PrintInformation* printInformation;
};

#endif //_PREVIEWFORM_H