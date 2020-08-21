#ifndef _FINDREPLACEDIALOG_H
#define _FINDREPLACEDIALOG_H

#include <afxdlgs.h>
#include "resource.h"
#include "String.h"

class FindReplaceDialog : public CFindReplaceDialog {
public:
	enum { IDF = IDD_FINDDIALOG, IDR = IDD_REPLACEDIALOG };
public:
	FindReplaceDialog(BOOL findOnly = TRUE, string findWhat = "", CWnd* parent = NULL);
	virtual BOOL OnInitDialog();

	BOOL WrapAround();

	BOOL Find();
	void Replace();
};

#endif //_FINDREPLACEDIALOG_H