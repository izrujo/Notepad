#include "HScrollActionFactory.h"
#include "TextEditingForm.h"
#include "HScrollActions.h"

HScrollActionFactory::HScrollActionFactory(TextEditingForm *textEditingForm) {
	this->textEditingForm = textEditingForm;
}

HScrollActionFactory::HScrollActionFactory(const HScrollActionFactory& source) {
	this->textEditingForm = source.textEditingForm;
}

HScrollActionFactory::~HScrollActionFactory() {

}

HScrollActionFactory& HScrollActionFactory::operator=(const HScrollActionFactory& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

HScrollAction* HScrollActionFactory::Make(UINT nSBCode) {
	HScrollAction *hScrollAction = 0;

	switch (nSBCode) {
	case SB_LINELEFT:
		hScrollAction = new LineLeftAction(this->textEditingForm);
		break;
	case SB_LINERIGHT:
		hScrollAction = new LineRightAction(this->textEditingForm);
		break;
	case SB_PAGELEFT:
		hScrollAction = new PageLeftAction(this->textEditingForm);
		break;
	case SB_PAGERIGHT:
		hScrollAction = new PageRightAction(this->textEditingForm);
		break;
	case SB_THUMBTRACK:
		hScrollAction = new ThumbTrackHScrollAction(this->textEditingForm);
		break;
	default:
		break;
	}

	return hScrollAction;
}