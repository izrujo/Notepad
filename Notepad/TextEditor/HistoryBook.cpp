#include "HistoryBook.h"
#include "CNTCommands.h"

HistoryBook::HistoryBook(Long capacity)
	: commands(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->top = 0;
}

HistoryBook::HistoryBook(const HistoryBook& source)
	: commands(source.capacity) {
	
	Stack<CNTCommand*> tempForSource(source.commands);
	Stack<CNTCommand*> temp(source.capacity);
	Long i = 0;
	while (i < source.length) {
		temp.Push(tempForSource.Top()->Clone());
		tempForSource.Pop();
		i++;
	}

	i = 0;
	while (i < source.length) {
		this->commands.Push(temp.Top());
		temp.Pop();
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->top = source.top;
}

HistoryBook::~HistoryBook() {
	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}
}

HistoryBook& HistoryBook::operator=(const HistoryBook& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}

	this->commands = source.commands;

	Stack<CNTCommand*> temp(source.capacity);
	i = 0;
	while (i < source.length) {
		temp.Push(this->commands.Top()->Clone());
		this->commands.Pop();
		i++;
	}

	i = 0;
	while (i < source.length) {
		this->commands.Push(temp.Top());
		temp.Pop();
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->top = source.top;

	return *this;
}

Long HistoryBook::Write(CNTCommand* command) {
	this->top = this->commands.Push(command);
	if (this->length >= this->capacity) {
		this->capacity++;
	}
	this->length++;

	return this->top;
}

Long HistoryBook::Erase() {
	if (this->commands.Top() != 0) {
		delete this->commands.Top();
	}

	this->commands.Pop();
	this->length--;
	this->top--;

	return -1;
}

CNTCommand* HistoryBook::OpenAt() {
	return this->commands.Top();
}

void HistoryBook::Empty() {
	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}
	this->length = 0;
	this->top = 0;
}

bool HistoryBook::IsEmpty() {
	return this->commands.IsEmpty();
}