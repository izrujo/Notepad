#include "History.h"
#include "Commands.h"

History::History(Long capacity)
	: commands(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->top = 0;
}

History::History(const History& source)
	: commands(source.capacity) {

	Stack<Command*> tempForSource(source.commands);
	Stack<Command*> temp(source.capacity);
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

History::~History() {
	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}
}

History& History::operator=(const History& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->commands.Top() != 0) {
			delete this->commands.Top();
			this->commands.Pop();
		}
		i++;
	}

	this->commands = source.commands;

	Stack<Command*> temp(source.capacity);
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

Long History::Happen(Command* command) {
	this->top = this->commands.Push(command);
	if (this->length >= this->capacity) {
		this->capacity++;
	}
	this->length++;

	return this->top;
}

Long History::Forget() {
	if (this->commands.Top() != 0) {
		delete this->commands.Top();
	}

	this->commands.Pop();
	this->length--;
	this->top--;

	return -1;
}

Command* History::Reveal() {
	return this->commands.Top();
}

bool History::IsEmpty() {
	return this->commands.IsEmpty();
}

History* History::Clone() {
	return new History(*this);
}