#include "VScrollActions.h"
#include "NotepadForm.h"
#include "ScrollController.h"
#include "Scroll.h"

VScrollAction::VScrollAction(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

VScrollAction::VScrollAction(const VScrollAction& source) {
	this->notepadForm = source.notepadForm;
}

VScrollAction::~VScrollAction() {

}

VScrollAction& VScrollAction::operator=(const VScrollAction& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

// LineUpAction

LineUpAction::LineUpAction(NotepadForm *notepadForm)
	:VScrollAction(notepadForm) {

}

LineUpAction::LineUpAction(const LineUpAction& source)
	: VScrollAction(source) {

}

LineUpAction::~LineUpAction() {

}

void LineUpAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Up();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

LineUpAction& LineUpAction::operator=(const LineUpAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

// LineDownAction

LineDownAction::LineDownAction(NotepadForm *notepadForm)
	:VScrollAction(notepadForm) {

}

LineDownAction::LineDownAction(const LineDownAction& source)
	: VScrollAction(source) {

}

LineDownAction::~LineDownAction() {

}

void LineDownAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->Down();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

LineDownAction& LineDownAction::operator=(const LineDownAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

// PageUpAction

PageUpAction::PageUpAction(NotepadForm *notepadForm)
	:VScrollAction(notepadForm) {

}

PageUpAction::PageUpAction(const PageUpAction& source)
	: VScrollAction(source) {

}

PageUpAction::~PageUpAction() {

}

void PageUpAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageUp();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

PageUpAction& PageUpAction::operator=(const PageUpAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

// PageDownAction

PageDownAction::PageDownAction(NotepadForm *notepadForm)
	:VScrollAction(notepadForm) {

}

PageDownAction::PageDownAction(const PageDownAction& source)
	: VScrollAction(source) {

}

PageDownAction::~PageDownAction() {

}

void PageDownAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->PageDown();
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

PageDownAction& PageDownAction::operator=(const PageDownAction& source) {
	VScrollAction::operator=(source);

	return *this;
}

// ThumbTrackVScrollAction

ThumbTrackVScrollAction::ThumbTrackVScrollAction(NotepadForm *notepadForm)
	:VScrollAction(notepadForm) {

}

ThumbTrackVScrollAction::ThumbTrackVScrollAction(const ThumbTrackVScrollAction& source)
	: VScrollAction(source) {

}

ThumbTrackVScrollAction::~ThumbTrackVScrollAction() {

}

void ThumbTrackVScrollAction::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) {
	Long position = this->notepadForm->scrollController->MoveVerticalScroll(nPos);
	int previous = this->notepadForm->SetScrollPos(SB_VERT, position, TRUE);
	position = this->notepadForm->GetScrollPos(SB_VERT);
	this->notepadForm->scrollController->MoveVerticalScroll(position);
	this->notepadForm->ScrollWindow(0, previous - position);
}

ThumbTrackVScrollAction& ThumbTrackVScrollAction::operator=(const ThumbTrackVScrollAction& source) {
	VScrollAction::operator=(source);

	return *this;
}
