#include "CHWindowForm.h"
#include "Font.h"
#include "CharacterMetrics.h"

CHWindowForm::CHWindowForm() {
	this->font = NULL;
	this->characterMetrics = NULL;
}

CHWindowForm::~CHWindowForm() {
	if (this->font != NULL) {
		delete this->font;
	}
	if (this->characterMetrics != NULL) {
		delete this->characterMetrics;
	}
}

int CHWindowForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	return 0;
}