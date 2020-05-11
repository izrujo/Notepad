#include "HScrollActions.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "Scroll.h"

HScrollAction::HScrollAction(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

HScrollAction::HScrollAction(const HScrollAction& source) {
	this->notepadForm = source.notepadForm;
}

HScrollAction::~HScrollAction() {

}

HScrollAction& HScrollAction::operator=(const HScrollAction& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

// LineLeftAction

LineLeftAction::LineLeftAction(NotepadForm *notepadForm)
	:HScrollAction(notepadForm) {

}

LineLeftAction::LineLeftAction(const LineLeftAction& source)
	: HScrollAction(source) {

}

LineLeftAction::~LineLeftAction() {

}

void LineLeftAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Left();
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);//
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

LineLeftAction& LineLeftAction::operator=(const LineLeftAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

// LineRightAction

LineRightAction::LineRightAction(NotepadForm *notepadForm)
	:HScrollAction(notepadForm) {

}

LineRightAction::LineRightAction(const LineRightAction& source)
	: HScrollAction(source) {

}

LineRightAction::~LineRightAction() {

}

void LineRightAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Right();
	Long previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous-position, 0);

}

LineRightAction& LineRightAction::operator=(const LineRightAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

// PageLeftAction

PageLeftAction::PageLeftAction(NotepadForm *notepadForm)
	:HScrollAction(notepadForm) {

}

PageLeftAction::PageLeftAction(const PageLeftAction& source)
	: HScrollAction(source) {

}

PageLeftAction::~PageLeftAction() {

}

void PageLeftAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageLeft();
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

PageLeftAction& PageLeftAction::operator=(const PageLeftAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

// PageRightAction

PageRightAction::PageRightAction(NotepadForm *notepadForm)
	:HScrollAction(notepadForm) {

}

PageRightAction::PageRightAction(const PageRightAction& source)
	: HScrollAction(source) {

}

PageRightAction::~PageRightAction() {

}

void PageRightAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageRight();
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

PageRightAction& PageRightAction::operator=(const PageRightAction& source) {
	HScrollAction::operator=(source);

	return *this;
}

// ThumbTrackHScrollAction

ThumbTrackHScrollAction::ThumbTrackHScrollAction(NotepadForm *notepadForm)
	:HScrollAction(notepadForm) {

}

ThumbTrackHScrollAction::ThumbTrackHScrollAction(const ThumbTrackHScrollAction& source)
	: HScrollAction(source) {

}

ThumbTrackHScrollAction::~ThumbTrackHScrollAction() {

}

void ThumbTrackHScrollAction::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->MoveHorizontalScroll(nPos);
	int previous = this->notepadForm->SetScrollPos(SB_HORZ, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_HORZ);
	this->notepadForm->scrollController->MoveHorizontalScroll(position);
	this->notepadForm->ScrollWindow(previous - position, 0);
}

ThumbTrackHScrollAction& ThumbTrackHScrollAction::operator=(const ThumbTrackHScrollAction& source) {
	HScrollAction::operator=(source);

	return *this;
}
