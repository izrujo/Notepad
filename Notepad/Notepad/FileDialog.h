//FileDialog.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : Open or save file.
MVC : ?
Author : Chan Hyung Kim
Written Date : 2020.07.16
Recently Updated : 2020.07.17
**************************************************/

#ifndef _FILEDIALOG_H
#define _FILEDIALOG_H

#include <afxdlgs.h>
#include <iostream>

using namespace std;

class FileDialog : public CFileDialog {
public:
    FileDialog(BOOL bOpenFileDialog,
        LPCTSTR lpszDefExt = NULL,
        LPCTSTR lpszFileName = NULL,
        DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        LPCTSTR lpszFilter = NULL,
        CWnd* pParentWnd = NULL,
        DWORD dwSize = 0,
        BOOL bVistaStyle = TRUE);
protected:
    //virtual void OnOK();
    DECLARE_MESSAGE_MAP()  

public:
    string GetEncodingType() const;
private:
    IFileDialogCustomize* customizeDlg;
    string encodingType;
};

inline string FileDialog::GetEncodingType() const {
    return const_cast<string&>(this->encodingType);
}

#endif //_FILEDIALOG_H