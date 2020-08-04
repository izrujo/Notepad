#include "Selection.h"

Selection::Selection(Long start, Long end) {
	this->start = start;
	this->end = end;
}

Selection::Selection(const Selection& source) {
	this->start = source.start;
	this->end = source.end;
}

Selection::~Selection() {

}

Selection& Selection::operator=(const Selection& source) {
	this->start = source.start;
	this->end = source.end;
	
	return *this;
}