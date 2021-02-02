#ifndef _HSCROLLACTIONS_H
#define _HSCROLLACTIONS_H

#include <afxwin.h>

class TextEditingForm;

class HScrollAction {
public:
	HScrollAction(TextEditingForm *textEditingForm = 0);
	HScrollAction(const HScrollAction& source);
	virtual ~HScrollAction() = 0;
	HScrollAction& operator=(const HScrollAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) = 0;
protected:
	TextEditingForm *textEditingForm;
};

class LineLeftAction : public HScrollAction {
public:
	LineLeftAction(TextEditingForm *textEditingForm = 0);
	LineLeftAction(const LineLeftAction& source);
	virtual ~LineLeftAction();
	LineLeftAction& operator=(const LineLeftAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class LineRightAction : public HScrollAction {
public:
	LineRightAction(TextEditingForm *textEditingForm = 0);
	LineRightAction(const LineRightAction& source);
	virtual ~LineRightAction();
	LineRightAction& operator=(const LineRightAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class PageLeftAction : public HScrollAction {
public:
	PageLeftAction(TextEditingForm *textEditingForm = 0);
	PageLeftAction(const PageLeftAction& source);
	virtual ~PageLeftAction();
	PageLeftAction& operator=(const PageLeftAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class PageRightAction : public HScrollAction {
public:
	PageRightAction(TextEditingForm *textEditingForm = 0);
	PageRightAction(const PageRightAction& source);
	virtual ~PageRightAction();
	PageRightAction& operator=(const PageRightAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class ThumbTrackHScrollAction : public HScrollAction {
public:
	ThumbTrackHScrollAction(TextEditingForm *textEditingForm = 0);
	ThumbTrackHScrollAction(const ThumbTrackHScrollAction& source);
	virtual ~ThumbTrackHScrollAction();
	ThumbTrackHScrollAction& operator=(const ThumbTrackHScrollAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

#endif // !_HSCROLLACTIONS_H
