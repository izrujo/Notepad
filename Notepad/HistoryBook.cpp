#include "HistoryBook.h"
#include "History.h"

HistoryBook::HistoryBook(Long capacity)
	: historys(capacity) {
	this->capacity = capacity;
	this->length = 0;
	this->top = 0;
}

HistoryBook::HistoryBook(const HistoryBook& source)
	: historys(source.capacity) {
	
	Stack<History*> tempForSource(source.historys);
	Stack<History*> temp(source.capacity);
	Long i = 0;
	while (i < source.length) {
		temp.Push(tempForSource.Top()->Clone());
		tempForSource.Pop();
		i++;
	}

	i = 0;
	while (i < source.length) {
		this->historys.Push(temp.Top());
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
		if (this->historys.Top() != 0) {
			delete this->historys.Top();
			this->historys.Pop();
		}
		i++;
	}
}

HistoryBook& HistoryBook::operator=(const HistoryBook& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->historys.Top() != 0) {
			delete this->historys.Top();
			this->historys.Pop();
		}
		i++;
	}

	this->historys = source.historys;

	Stack<History*> temp(source.capacity);
	i = 0;
	while (i < source.length) {
		temp.Push(this->historys.Top()->Clone());
		this->historys.Pop();
		i++;
	}

	i = 0;
	while (i < source.length) {
		this->historys.Push(temp.Top());
		temp.Pop();
		i++;
	}

	this->capacity = source.capacity;
	this->length = source.length;
	this->top = source.top;

	return *this;
}

Long HistoryBook::Write(History* command) {
	this->top = this->historys.Push(command);
	if (this->length >= this->capacity) {
		this->capacity++;
	}
	this->length++;

	return this->top;
}

Long HistoryBook::Erase() {
	if (this->historys.Top() != 0) {
		delete this->historys.Top();
	}

	this->historys.Pop();
	this->length--;
	this->top--;

	return -1;
}

History* HistoryBook::OpenAt() {
	return this->historys.Top();
}

void HistoryBook::Empty() {
	Long i = 0;
	while (i < this->length) {
		if (this->historys.Top() != 0) {
			delete this->historys.Top();
			this->historys.Pop();
		}
		i++;
	}
	this->length = 0;
	this->top = 0;
}

bool HistoryBook::IsEmpty() {
	return this->historys.IsEmpty();
}