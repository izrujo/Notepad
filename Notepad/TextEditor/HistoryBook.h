#ifndef _HISTORYBOOK_H
#define _HISTORYBOOK_H

#include "../Utilities/Stack.h"

class CNTCommand;

class HistoryBook {
public:
	HistoryBook(Long capacity = 64);
	HistoryBook(const HistoryBook& source);
	~HistoryBook();
	HistoryBook& operator=(const HistoryBook& source);

	Long Write(CNTCommand* command);
	Long Erase();
	CNTCommand* OpenAt();
	void Empty();
	bool IsEmpty();

	Long GetCapacity() const;
	Long GetLength() const;
	Long GetTop() const;

private:
	Stack<CNTCommand*> commands;
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