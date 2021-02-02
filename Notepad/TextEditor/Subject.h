#ifndef _SUBJECT_H
#define _SUBJECT_H

#include "../Utilities/Array.h"

typedef signed long int Long;
class Observer;

class Subject {
public:
	Subject(Long capacity = 256);
	Subject(const Subject& source);
	virtual ~Subject();
	Subject& operator=(const Subject& source);

	virtual Long AttachObserver(Observer *observer);
	virtual Long DetachObserver(Observer *observer);
	virtual void Notify();
	Observer* GetAt(Long index);

	Long GetCapacity() const;
	Long GetLength() const;
private:
	Array<Observer*> observers;
	Long capacity;
	Long length;
};

inline Long Subject::GetCapacity() const {
	return this->capacity;
}

inline Long Subject::GetLength() const {
	return this->length;
}

int CompareObserverLinks(void *one, void *other);

#endif //_SUBJECT_H