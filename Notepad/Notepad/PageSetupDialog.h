#ifndef _PAGESETUPDIALOG_H
#define _PAGESETUPDIALOG_H

#include "resource.h"
#include "../Utilities/String.h"
#include "afxwin.h"

class PageSetupDialog {
public:
	enum { IDD = IDD_PAGESETUPDIALOG };
public:
	PageSetupDialog(CWnd* parent = NULL);
	
	BOOL DoModal();
public:
	PAGESETUPDLG psd;
};

UINT PageSetupHook(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

#endif //_PAGESETUPDIALOG_H