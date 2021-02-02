//Stack.h

#ifndef _STACK_H
#define _STACK_H

typedef signed long int Long;
template <typename T>

class Stack {
public:
	Stack(Long capacity = 64);
	Stack(const Stack& source);
	~Stack();
	Stack& operator=(const Stack& source);

	Long Push(T object);
	Long Pop();
	T& Top();
	void Empty();
	bool IsEmpty();

	Long GetCapacity() const;
	Long GetLength() const;
	Long GetTop() const;

private:
	T(*front);
	Long capacity;
	Long length;
	Long top;
};

template <typename T>
inline Long Stack<T>::GetCapacity() const {
	return this->capacity;
}

template <typename T>
inline Long Stack<T>::GetLength() const {
	return this->length;
}

template <typename T>
inline Long Stack<T>::GetTop() const {
	return this->top;
}

template <typename T>
Stack<T>::Stack(Long capacity) {
	this->front = new T[capacity];
	this->capacity = capacity;
	this->length = 0;
	this->top = 0;
}

template <typename T>
Stack<T>::Stack(const Stack& source) {
	this->front = new T[source.capacity];
	this->capacity = source.capacity;
	
	Long i = 0;
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->length = source.length;
	this->top = source.top;
}

template <typename T>
Stack<T>::~Stack() {
	if (this->front != 0) {
		delete[] this->front;
	}
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& source) {
	if (this->front != 0) {
		delete[] this->front;
	}
	this->front = new T[source.capacity];
	this->capacity = source.capacity;
	Long i = 0;
	while (i < source.length) {
		this->front[i] = source.front[i];
		i++;
	}
	this->length = source.length;
	this->top = source.top;

	return *this;
}

template <typename T>
Long Stack<T>::Push(T object) {
	T(*objects);
	Long i;

	if (this->length >= this->capacity) {
		objects = new T[this->capacity + 1];
		i = 0;
		while (i < this->length) {
			objects[i] = this->front[i];
			//memcpy(((char(*))objects) + (i * size), ((char(*))stack->front) + (i * size), size);
			i++;
		}
		if (this->front != 0) {
			delete[] this->front;
		}
		this->front = objects;
		this->capacity++;
		this->top = this->length;
	}
	this->front[this->top] = object;
	//memcpy(((char(*))stack->front) + ((stack->top) * size), object, size);
	this->length++;
	this->top++;

	return this->top;
}

template <typename T>
Long Stack<T>::Pop() {
	this->top--;
	this->length--;

	return -1;
}

template <typename T>
T& Stack<T>::Top() {
	return this->front[this->top - 1];
	//memcpy(object, ((char(*))stack->front) + ((stack->top - 1) * size), size);
}

template <typename T>
void Stack<T>::Empty() {
	this->top = 0;
	this->length = 0;
}

template <typename T>
bool Stack<T>::IsEmpty() {
	bool ret;
	if (this->length == 0) {
		ret = true;
	}
	else {
		ret = false;
	}
	return ret;
}

#endif //_STACK_H