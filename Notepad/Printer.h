#ifndef _PRINTER_H
#define _PRINTER_H

#include <afxwin.h>

typedef signed long int Long;

class NotepadForm;
class PrintInformation;
class PrintStateDialog;

class Printer {
public:
	Printer(NotepadForm* notepadForm = 0);
	Printer(const Printer& source);
	~Printer();
	Printer& operator=(const Printer& source);

	void Print(PrintInformation *printInformation, PrintStateDialog* printStateDialog);
	void Pause();
	void Resume();
	void End();

	BOOL GetIsPrinting() const;
	BOOL GetIsPausing() const;

private:
	static UINT PrintThread(LPVOID pParam);

private:
	NotepadForm* notepadForm;
	CWinThread* thread;
	PrintInformation* printInformation;
	PrintStateDialog* printStateDialog;
	BOOL isPrinting;
	BOOL isPausing;
};

inline BOOL Printer::GetIsPrinting() const {
	return this->isPrinting;
}

inline BOOL Printer::GetIsPausing() const {
	return this->isPausing;
}

#endif //_PRINTER_H