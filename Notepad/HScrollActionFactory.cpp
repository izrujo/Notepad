#include "HScrollActionFactory.h"
#include "NotepadForm.h"
#include "HScrollActions.h"

HScrollActionFactory::HScrollActionFactory(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

HScrollActionFactory::HScrollActionFactory(const HScrollActionFactory& source) {
	this->notepadForm = source.notepadForm;
}

HScrollActionFactory::~HScrollActionFactory() {

}

HScrollAction* HScrollActionFactory::Make(UINT nSBCode) {
	HScrollAction *hScrollAction = 0;

	switch (nSBCode) {
	case SB_LINELEFT:
		hScrollAction = new LineLeftAction(this->notepadForm);
		break;
	case SB_LINERIGHT:
		hScrollAction = new LineRightAction(this->notepadForm);
		break;
	case SB_PAGELEFT:
		hScrollAction = new PageLeftAction(this->notepadForm);
		break;
	case SB_PAGERIGHT:
		hScrollAction = new PageRightAction(this->notepadForm);
		break;
	case SB_THUMBTRACK:
		hScrollAction = new ThumbTrackHScrollAction(this->notepadForm);
		break;
	default:
		break;
	}

	return hScrollAction;
}

HScrollActionFactory& HScrollActionFactory::operator=(const HScrollActionFactory& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}