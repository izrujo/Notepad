#include "VScrollActionFactory.h"
#include "NotepadForm.h"
#include "VScrollActions.h"

VScrollActionFactory::VScrollActionFactory(NotepadForm *notepadForm) {
	this->notepadForm = notepadForm;
}

VScrollActionFactory::VScrollActionFactory(const VScrollActionFactory& source) {
	this->notepadForm = source.notepadForm;
}

VScrollActionFactory::~VScrollActionFactory() {

}

VScrollActionFactory& VScrollActionFactory::operator=(const VScrollActionFactory& source) {
	this->notepadForm = source.notepadForm;

	return *this;
}

VScrollAction* VScrollActionFactory::Make(UINT nSBCode) {
	VScrollAction *vScrollAction = 0;

	switch (nSBCode) {
	case SB_LINEUP:
		vScrollAction = new LineUpAction(this->notepadForm);
		break;
	case SB_LINEDOWN:
		vScrollAction = new LineDownAction(this->notepadForm);
		break;
	case SB_PAGEUP:
		vScrollAction = new PageUpAction(this->notepadForm);
		break;
	case SB_PAGEDOWN:
		vScrollAction = new PageDownAction(this->notepadForm);
		break;
	case SB_THUMBTRACK:
		vScrollAction = new ThumbTrackVScrollAction(this->notepadForm);
		break;
	default:
		break;
	}

	return vScrollAction;
}