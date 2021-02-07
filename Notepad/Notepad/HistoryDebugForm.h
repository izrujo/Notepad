#ifndef _HISTORYDEBUGFORM_H
#define _HISTORYDEBUGFORM_H

#include <afxwin.h>
#include <afxext.h>

#include "../Utilities/Array.h"

class HistoryBook;

class HistoryDebugForm : public CFrameWnd { ///600 dpi 기준으로 디폴트를 잡자.
public:
	HistoryDebugForm(CWnd* parent = NULL);
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* parent;
	HistoryBook* undo;
	HistoryBook* redo;
	Array<CString> *undos;
	Long undosCapacity;
	Long undosLength;
	Array<CString> *redos;
	Long redosCapacity;
	Long redosLength;
};

#endif //_HISTORYDEBUGFORM_H