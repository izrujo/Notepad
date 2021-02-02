#ifndef _PRINTJOBMANAGER_H
#define _PRINTJOBMANAGER_H

#include <afxwin.h>

typedef signed long int Long;

class NotepadForm;
class PrintStateDialog;

class PrintJobManager {
public:
	PrintJobManager(NotepadForm* notepadForm = 0);
	PrintJobManager(const PrintJobManager& source);
	~PrintJobManager();
	PrintJobManager& operator=(const PrintJobManager& source);

	void Check(PrintStateDialog* printStateDialog);
	void Pause();
	void Resume();
	void End();

	BOOL GetIsChecking() const;
	BOOL GetIsPausing() const;

private:
	static UINT CheckThread(LPVOID pParam);

private:
	NotepadForm* notepadForm;
	CWinThread* thread;
	PrintStateDialog* printStateDialog;
	BOOL isChecking;
	BOOL isPausing;
};

inline BOOL PrintJobManager::GetIsChecking() const {
	return this->isChecking;
}

inline BOOL PrintJobManager::GetIsPausing() const {
	return this->isPausing;
}

#endif //_PRINTJOBMANAGER_H