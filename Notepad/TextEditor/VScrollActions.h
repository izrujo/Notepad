#ifndef _VSCROLLACTIONS_H
#define _VSCROLLACTIONS_H

#include <afxwin.h>

class TextEditingForm;

class VScrollAction {
public:
	VScrollAction(TextEditingForm *textEditingForm = 0);
	VScrollAction(const VScrollAction& source);
	virtual ~VScrollAction() = 0;
	VScrollAction& operator=(const VScrollAction& source);

	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) = 0;
protected:
	TextEditingForm *textEditingForm;
};


class LineUpAction : public VScrollAction {
public:
	LineUpAction(TextEditingForm *textEditingForm = 0);
	LineUpAction(const LineUpAction& source);
	virtual ~LineUpAction();
	LineUpAction& operator=(const LineUpAction& source);

	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class LineDownAction : public VScrollAction {
public:
	LineDownAction(TextEditingForm *textEditingForm = 0);
	LineDownAction(const LineDownAction& source);
	virtual ~LineDownAction();
	LineDownAction& operator=(const LineDownAction& source);

	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class PageUpAction : public VScrollAction {
public:
	PageUpAction(TextEditingForm *textEditingForm = 0);
	PageUpAction(const PageUpAction& source);
	virtual ~PageUpAction();
	PageUpAction& operator=(const PageUpAction& source);

	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class PageDownAction : public VScrollAction {
public:
	PageDownAction(TextEditingForm *textEditingForm = 0);
	PageDownAction(const PageDownAction& source);
	virtual ~PageDownAction();
	PageDownAction& operator=(const PageDownAction& source);

	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class ThumbTrackVScrollAction : public VScrollAction {
public:
	ThumbTrackVScrollAction(TextEditingForm *textEditingForm = 0);
	ThumbTrackVScrollAction(const ThumbTrackVScrollAction& source);
	virtual ~ThumbTrackVScrollAction();
	ThumbTrackVScrollAction& operator=(const ThumbTrackVScrollAction& source);

	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

#endif // !_VScrollActions_H