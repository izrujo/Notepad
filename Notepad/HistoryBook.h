#ifndef _HISTORYBOOK_H
#define _HISTORYBOOK_H

#include "Stack.h"

class Command;

class HistoryBook {
public:
	HistoryBook(Long capacity = 64);
	HistoryBook(const HistoryBook& source);
	~HistoryBook();
	HistoryBook& operator=(const HistoryBook& source);

	Long Write(Command* command);
	Long Erase();
	Command* OpenAt();
	void Empty();
	bool IsEmpty();

	Long GetCapacity() const;
	Long GetLength() const;
	Long GetTop() const;

private:
	Stack<Command*> commands;
	Long capacity;
	Long length;
	Long top;
};

inline Long HistoryBook::GetCapacity() const {
	return this->capacity;
}

inline Long HistoryBook::GetLength() const {
	return this->length;
}

inline Long HistoryBook::GetTop() const {
	return this->top;
}

#endif //_HISTORYBOOK_H