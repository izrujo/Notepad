#include "VScrollActionFactory.h"
#include "TextEditingForm.h"
#include "VScrollActions.h"

VScrollActionFactory::VScrollActionFactory(TextEditingForm *textEditingForm) {
	this->textEditingForm = textEditingForm;
}

VScrollActionFactory::VScrollActionFactory(const VScrollActionFactory& source) {
	this->textEditingForm = source.textEditingForm;
}

VScrollActionFactory::~VScrollActionFactory() {

}

VScrollActionFactory& VScrollActionFactory::operator=(const VScrollActionFactory& source) {
	this->textEditingForm = source.textEditingForm;

	return *this;
}

VScrollAction* VScrollActionFactory::Make(UINT nSBCode) {
	VScrollAction *vScrollAction = 0;

	switch (nSBCode) {
	case SB_LINEUP:
		vScrollAction = new LineUpAction(this->textEditingForm);
		break;
	case SB_LINEDOWN:
		vScrollAction = new LineDownAction(this->textEditingForm);
		break;
	case SB_PAGEUP:
		vScrollAction = new PageUpAction(this->textEditingForm);
		break;
	case SB_PAGEDOWN:
		vScrollAction = new PageDownAction(this->textEditingForm);
		break;
	case SB_THUMBTRACK:
		vScrollAction = new ThumbTrackVScrollAction(this->textEditingForm);
		break;
	default:
		break;
	}

	return vScrollAction;
}