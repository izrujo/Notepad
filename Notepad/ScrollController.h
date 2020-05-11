#ifndef _SCROLLCONTROLLER
#define _SCROLLCONTROLLER

#include "Observer.h"

class NotepadForm;
typedef signed long int Long;

class Scroll;
class ScrollController : public Observer {
public:
	ScrollController(NotepadForm *notepadForm = 0);
	ScrollController(const ScrollController& source);
	virtual ~ScrollController();
	virtual void Update();
	virtual Observer* GetClone();
	Long Left();
	Long Right();
	Long PageLeft();
	Long PageRight();
	Long Up();
	Long Down();
	Long PageUp();
	Long PageDown();
	Long Rotate(short delta);
	Long MoveHorizontalScroll(Long position);
	Long MoveVerticalScroll(Long position);
	void SmartScrollToPoint(Long x, Long y);

	Long GetNoteWidth() const;
	Long GetNoteHeight() const;
	Scroll* GetHorizontalScroll() const;
	Scroll* GetVerticalScroll() const;
private:
	NotepadForm *notepadForm;
	Long noteWidth;
	Long noteHeight;
	Scroll *horizontalScroll;
	Scroll *verticalScroll;
};
inline Long ScrollController::GetNoteWidth() const {
	return this->noteWidth;
}
inline Long ScrollController::GetNoteHeight() const {
	return this->noteHeight;
}
inline Scroll* ScrollController::GetHorizontalScroll() const {
	return const_cast<Scroll*>(this->horizontalScroll);
}
inline Scroll* ScrollController::GetVerticalScroll() const {
	return const_cast<Scroll*>(this->verticalScroll);
}

#endif //_SCROLLCONTROLLER