#ifndef _PRINTSTATEDIALOG_H
#define _PRINTSTATEDIALOG_H

#include <afxdlgs.h>
#include "resource.h"

class PrintStateDialog : public CDialog {
public:
	enum { IDD = IDD_PRINTSTATEDIALOG };
public:
	PrintStateDialog(CWnd* parent = NULL);
	virtual BOOL OnInitDialog();
	void OnCancel();
};

#endif //_PRINTSTATEDIALOG_H