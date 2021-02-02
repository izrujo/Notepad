#include "VScrollActions.h"
#include "TextEditingForm.h"
#include "ScrollController.h"
#include "Scroll.h"

VScrollAction::VScrollAction(TextEditingForm *textEditingForm) {
	this->textEditingForm = textEditingForm;
}

VScrollAction::VScrollAction(const VScrollAction& source) {
	this->textEditingForm = source.textEditingForm;
}

VScrollAction::~VScrollAction() {

}

VScrollAction& VScrollAction::operator=(const VScrollAction& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

// LineUpAction
LineUpAction::LineUpAction(TextEditingForm *textEditingForm)
	: VScrollAction(textEditingForm) {

}

LineUpAction::LineUpAction(const LineUpAction& source)
	: VScrollAction(source) {

}

LineUpAction::~LineUpAction() {

}

LineUpAction& LineUpAction::operator=(const LineUpAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

void LineUpAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->Up();
	int previous = this->textEditingForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_VERT);
	this->textEditingForm->scrollController->MoveVerticalScroll(position);
	this->textEditingForm->ScrollWindow(0, previous - position);
}

// LineDownAction
LineDownAction::LineDownAction(TextEditingForm *textEditingForm)
	: VScrollAction(textEditingForm) {

}

LineDownAction::LineDownAction(const LineDownAction& source)
	: VScrollAction(source) {

}

LineDownAction::~LineDownAction() {

}

LineDownAction& LineDownAction::operator=(const LineDownAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

void LineDownAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->Down();
	int previous = this->textEditingForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_VERT);
	this->textEditingForm->scrollController->MoveVerticalScroll(position);
	this->textEditingForm->ScrollWindow(0, previous - position);
}

// PageUpAction
PageUpAction::PageUpAction(TextEditingForm *textEditingForm)
	: VScrollAction(textEditingForm) {

}

PageUpAction::PageUpAction(const PageUpAction& source)
	: VScrollAction(source) {

}

PageUpAction::~PageUpAction() {

}

PageUpAction& PageUpAction::operator=(const PageUpAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

void PageUpAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->PageUp();
	int previous = this->textEditingForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_VERT);
	this->textEditingForm->scrollController->MoveVerticalScroll(position);
	this->textEditingForm->ScrollWindow(0, previous - position);
}

// PageDownAction
PageDownAction::PageDownAction(TextEditingForm *textEditingForm)
	: VScrollAction(textEditingForm) {

}

PageDownAction::PageDownAction(const PageDownAction& source)
	: VScrollAction(source) {

}

PageDownAction::~PageDownAction() {

}

PageDownAction& PageDownAction::operator=(const PageDownAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

void PageDownAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->PageDown();
	int previous = this->textEditingForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_VERT);
	this->textEditingForm->scrollController->MoveVerticalScroll(position);
	this->textEditingForm->ScrollWindow(0, previous - position);
}

// ThumbTrackVScrollAction
ThumbTrackVScrollAction::ThumbTrackVScrollAction(TextEditingForm *textEditingForm)
	: VScrollAction(textEditingForm) {

}

ThumbTrackVScrollAction::ThumbTrackVScrollAction(const ThumbTrackVScrollAction& source)
	: VScrollAction(source) {

}

ThumbTrackVScrollAction::~ThumbTrackVScrollAction() {

}

ThumbTrackVScrollAction& ThumbTrackVScrollAction::operator=(const ThumbTrackVScrollAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

void ThumbTrackVScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->textEditingForm->scrollController->MoveVerticalScroll(nPos);
	int previous = this->textEditingForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->textEditingForm->GetScrollPos(SB_VERT);
	this->textEditingForm->scrollController->MoveVerticalScroll(position);
	this->textEditingForm->ScrollWindow(0, previous - position);
}