#include "CaretController.h"
#include "Caret.h"
#include "CharacterMetrics.h"
#include "TextEditingForm.h"
#include "Glyph.h"
#include "Observer.h"
#include "ScrollController.h"
#include "Scroll.h"

CaretController::CaretController(TextEditingForm* textEditingForm) {
	this->textEditingForm = textEditingForm;
	this->caret = 0;
	this->textEditingForm->AttachObserver(this);
}

CaretController::CaretController(const CaretController& source) {
	this->textEditingForm = source.textEditingForm;
	this->caret = new Caret(*source.caret);
}

CaretController::~CaretController() {
	if (this->caret != 0) {
		delete this->caret;
	}
	this->textEditingForm->DetachObserver(this);
}

CaretController& CaretController::operator=(const CaretController& source) {
	this->textEditingForm = source.textEditingForm;
	this->caret = new Caret(*source.caret);

	return *this;
}

void CaretController::Update() {
	if (this->caret == 0) {
		this->caret = new Caret(this->textEditingForm);
	}
	Long height = this->textEditingForm->characterMetrics->GetHeight();
	BOOL isComposing = this->textEditingForm->GetIsComposing();
	Long x;
	this->textEditingForm->current->GetLength();
	if (isComposing == TRUE) {
		this->caret->Create(this->textEditingForm->characterMetrics->GetDoubleByteWidth(), height);
		x = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current, this->textEditingForm->current->GetCurrent() - 1);

	}
	else {
		this->caret->Create(2, height);
		x = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current, this->textEditingForm->current->GetCurrent()); //??
	}

	Long index = this->textEditingForm->note->GetCurrent();
	Long y = this->textEditingForm->characterMetrics->GetY(index);

	Long hScrollPosition = 0;
	Long vScrollPosition = 0;
	if (this->textEditingForm->scrollController != NULL) {
		hScrollPosition = this->textEditingForm->scrollController->GetHorizontalScroll()->GetPosition();
		vScrollPosition = this->textEditingForm->scrollController->GetVerticalScroll()->GetPosition();
	}

	// 스크롤 위치를 반영해서 캐럿의 좌표를 조정해서 설정한다.
	this->caret->Move(x - hScrollPosition, y - vScrollPosition);
	this->caret->Show(true);
}

Long CaretController::GetCaretX() {
	return this->caret->GetX();
}

Long CaretController::GetCaretY() {
	return this->caret->GetY();
}