#ifndef _HISTORY_H
#define _HISTORY_H

#include "Stack.h"

class Command;

class History {
public:
	History(Long capacity = 64);
	History(const History& source);
	~History();
	History& operator=(const History& source);

	Long Happen(Command* command);
	Long Forget();
	Command* Reveal();
	bool IsEmpty();

	History* Clone();

	Long GetCapacity() const;
	Long GetLength() const;
	Long GetTop() const;

private:
	Stack<Command*> commands;
	Long capacity;
	Long length;
	Long top;
};

inline Long History::GetCapacity() const {
	return this->capacity;
}

inline Long History::GetLength() const {
	return this->length;
}

inline Long History::GetTop() const {
	return this->top;
}

#endif //_HISTORY_H