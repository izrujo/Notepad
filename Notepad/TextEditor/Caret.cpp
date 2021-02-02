#include "Caret.h"
#include "TextEditingForm.h"

Caret::Caret(TextEditingForm *textEditingForm) {
	this->textEditingForm = textEditingForm;
	this->x = 0;
	this->y = 0;
}

Caret::Caret(const Caret& source) {
	this->textEditingForm = source.textEditingForm;
	this->x = source.x;
	this->y = source.y;
}

Caret::~Caret() {
	::DestroyCaret();
}

Caret& Caret::operator=(const Caret& source) {
	this->textEditingForm = source.textEditingForm;
	this->x = source.x;
	this->y = source.y;

	return *this;
}

void Caret::Create(Long width, Long height) {
	this->textEditingForm->CreateSolidCaret(width, height);
}

void Caret::Move(Long x, Long y) {
	this->x = x;
	this->y = y;
	CPoint point(x, y);
	this->textEditingForm->SetCaretPos(point);
}

void Caret::Show(bool isShow) {
	if (isShow == TRUE) {
		this->textEditingForm->ShowCaret();
	}
	else {
		this->textEditingForm->HideCaret();
	}
}