#ifndef _PRINTER_H
#define _PRINTER_H

#include <afxwin.h>

typedef signed long int Long;

class NotepadForm;
class PrintInformation;

class Printer {
public:
	Printer(NotepadForm* notepadForm = 0, PrintInformation *printInformation = 0);
	Printer(const Printer& source);
	~Printer();
	Printer& operator=(const Printer& source);

	void Print();

private:
	NotepadForm* notepadForm;
	PrintInformation* printInformation;
};


#endif //_PRINTER_H