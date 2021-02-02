#include "HScrollActions.h"
#include "TextEditingForm.h"
#include "ScrollController.h"
#include "Scroll.h"

HScrollAction:: HScrollAction(TextEditingForm *textEditingForm) {
	this->textEditingForm = textEditingForm;
}

HScrollAction:: HScrollAction(const HScrollAction& source) {
	this->textEditingForm = source.textEditingForm;
}

HScrollAction::~HScrollAction() {

}

HScrollAction& HScrollAction::operator=(const HScrollAction& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

// LineLeftAction
LineLeftAction::LineLeftAction(TextEditingForm *textEditingForm)
	: HScrollAction(textEditingForm) {

}

LineLeftAction::LineLeftAction(const LineLeftAction& source)
	: HScrollAction(source) {

}

LineLeftAction::~LineLeftAction() {

}

LineLeftAction& LineLeftAction::operator=(const LineLeftAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

void LineLeftAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->Left();
	int previous = this->textEditingForm->SetScrollPos(SB_HORZ, position, TRUE);//
	position = this->textEditingForm->GetScrollPos(SB_HORZ);
	this->textEditingForm->scrollController->MoveHorizontalScroll(position);
	this->textEditingForm->ScrollWindow(previous - position, 0);
}

// LineRightAction
LineRightAction::LineRightAction(TextEditingForm *textEditingForm)
	: HScrollAction(textEditingForm) {

}

LineRightAction::LineRightAction(const LineRightAction& source)
	: HScrollAction(source) {

}

LineRightAction::~LineRightAction() {

}

LineRightAction& LineRightAction::operator=(const LineRightAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

void LineRightAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->Right();
	Long previous = this->textEditingForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_HORZ);
	this->textEditingForm->scrollController->MoveHorizontalScroll(position);
	this->textEditingForm->ScrollWindow(previous-position, 0);
}

// PageLeftAction
PageLeftAction::PageLeftAction(TextEditingForm *textEditingForm)
	: HScrollAction(textEditingForm) {

}

PageLeftAction::PageLeftAction(const PageLeftAction& source)
	: HScrollAction(source) {

}

PageLeftAction::~PageLeftAction() {

}

PageLeftAction& PageLeftAction::operator=(const PageLeftAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

void PageLeftAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->PageLeft();
	int previous = this->textEditingForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_HORZ);
	this->textEditingForm->scrollController->MoveHorizontalScroll(position);
	this->textEditingForm->ScrollWindow(previous - position, 0);
}

// PageRightAction
PageRightAction::PageRightAction(TextEditingForm *textEditingForm)
	: HScrollAction(textEditingForm) {

}

PageRightAction::PageRightAction(const PageRightAction& source)
	: HScrollAction(source) {

}

PageRightAction::~PageRightAction() {

}

PageRightAction& PageRightAction::operator=(const PageRightAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

void PageRightAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->PageRight();
	int previous = this->textEditingForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_HORZ);
	this->textEditingForm->scrollController->MoveHorizontalScroll(position);
	this->textEditingForm->ScrollWindow(previous - position, 0);
}

// ThumbTrackHScrollAction
ThumbTrackHScrollAction::ThumbTrackHScrollAction(TextEditingForm *textEditingForm)
	: HScrollAction(textEditingForm) {

}

ThumbTrackHScrollAction::ThumbTrackHScrollAction(const ThumbTrackHScrollAction& source)
	: HScrollAction(source) {

}

ThumbTrackHScrollAction::~ThumbTrackHScrollAction() {

}

ThumbTrackHScrollAction& ThumbTrackHScrollAction::operator=(const ThumbTrackHScrollAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

void ThumbTrackHScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->MoveHorizontalScroll(nPos);
	int previous = this->textEditingForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_HORZ);
	this->textEditingForm->scrollController->MoveHorizontalScroll(position);
	this->textEditingForm->ScrollWindow(previous - position, 0);
}