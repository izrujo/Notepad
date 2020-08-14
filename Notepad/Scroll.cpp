//Scroll.cpp
#include "Scroll.h"

Scroll::Scroll() {
	this->minimum = 0;
	this->maximum = 0;
	this->pageSize = 0;
	this->lineSize = 0;
	this->position = 0;
}

Scroll::Scroll(Long minimum, Long maximum, Long pageSize, Long lineSize, Long position) {
	this->minimum = minimum;
	this->maximum = maximum;
	this->pageSize = pageSize;
	this->lineSize = lineSize;
	this->position = position;
}

Scroll::Scroll(const Scroll& source) {
	this->minimum = source.minimum;
	this->maximum = source.maximum;
	this->pageSize = source.pageSize;
	this->lineSize = source.lineSize;
	this->position = source.position;
}

Scroll::~Scroll() {

}

Scroll& Scroll::operator=(const Scroll& source) {
	this->minimum = source.minimum;
	this->maximum = source.maximum;
	this->pageSize = source.pageSize;
	this->lineSize = source.lineSize;
	this->position = source.position;

	return *this;
}

Long Scroll::Up() {
	this->position += this->lineSize;
	if (this->position > (this->maximum-this->pageSize + 1)) { 
		this->position = this->maximum - this->pageSize + 1; // 1�� ���ϴ� ������ ������� ȣȯ�� ����
	}
	return this->position;
}

Long Scroll::Down() {
	this->position -= this->lineSize;
	if (this->position < this->minimum) {
		this->position = this->minimum;
	}
	return this->position;
}
Long Scroll::PageUp() {
	this->position += this->pageSize;
	if (this->position > (this->maximum - this->pageSize + 1)) {
		this->position = (this->maximum - this->pageSize + 1);
	}
	return this->position;
}

Long Scroll::PageDown() {
	this->position -= this->pageSize;
	if (this->position < this->minimum) {
		this->position = this->minimum;
	}
	return this->position;
}

Long Scroll::Move(Long position) {
	this->position = position;
	return this->position;
}

Long Scroll::Rotate(short delta) {
	this->position += -delta / WHEEL_DELTA * this->lineSize;
	if (this->position > (this->maximum - this->pageSize + 1)) {
		this->position = this->maximum - this->pageSize + 1;
	}
	else if (this->position < this->minimum) {
		this->position = this->minimum;
	}
	
	return this->position;
}

SCROLLINFO Scroll::GetScrollInfo() {
	SCROLLINFO scrollInfo = { 0, };
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL & ~SIF_POS;
	scrollInfo.nMin = this->minimum;
	scrollInfo.nMax = this->maximum;
	scrollInfo.nPage = this->pageSize;
	scrollInfo.nPos = this->position;

	return scrollInfo;
}

//VerticalScroll
HorizontalScroll::HorizontalScroll() 
	: Scroll() {

}

HorizontalScroll::HorizontalScroll(Long minimum, Long maximum, Long pageSize, Long lineSize, Long position)
	: Scroll(minimum, maximum, pageSize, lineSize, position){

}

HorizontalScroll::HorizontalScroll(const HorizontalScroll& source)
	: Scroll(source){

}

HorizontalScroll::~HorizontalScroll() {

}

HorizontalScroll& HorizontalScroll::operator=(const HorizontalScroll& source) {
	Scroll::operator=(source);
	
	return *this;
}

//VerticalScroll
VerticalScroll::VerticalScroll() 
	: Scroll() {

}

VerticalScroll::VerticalScroll(Long minimum, Long maximum, Long pageSize, Long lineSize, Long position)
	: Scroll(minimum, maximum, pageSize, lineSize, position){

}

VerticalScroll::VerticalScroll(const VerticalScroll& source)
	: Scroll(source) {

}

VerticalScroll::~VerticalScroll() {

}

VerticalScroll& VerticalScroll::operator=(const VerticalScroll& source) {
	Scroll::operator=(source);

	return *this;
}