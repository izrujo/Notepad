#ifndef _HSCROLLACTIONS_H
#define _HSCROLLACTIONS_H

#include <afxwin.h>

class NotepadForm;

class HScrollAction {
public:
	HScrollAction(NotepadForm *notepadForm = 0);
	HScrollAction(const HScrollAction& source);
	virtual ~HScrollAction() = 0;
	HScrollAction& operator=(const HScrollAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar) = 0;
protected:
	NotepadForm *notepadForm;
};

class LineLeftAction : public HScrollAction {
public:
	LineLeftAction(NotepadForm *notepadForm = 0);
	LineLeftAction(const LineLeftAction& source);
	virtual ~LineLeftAction();
	LineLeftAction& operator=(const LineLeftAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class LineRightAction : public HScrollAction {
public:
	LineRightAction(NotepadForm *notepadForm = 0);
	LineRightAction(const LineRightAction& source);
	virtual ~LineRightAction();
	LineRightAction& operator=(const LineRightAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class PageLeftAction : public HScrollAction {
public:
	PageLeftAction(NotepadForm *notepadForm = 0);
	PageLeftAction(const PageLeftAction& source);
	virtual ~PageLeftAction();
	PageLeftAction& operator=(const PageLeftAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class PageRightAction : public HScrollAction {
public:
	PageRightAction(NotepadForm *notepadForm = 0);
	PageRightAction(const PageRightAction& source);
	virtual ~PageRightAction();
	PageRightAction& operator=(const PageRightAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

class ThumbTrackHScrollAction : public HScrollAction {
public:
	ThumbTrackHScrollAction(NotepadForm *notepadForm = 0);
	ThumbTrackHScrollAction(const ThumbTrackHScrollAction& source);
	virtual ~ThumbTrackHScrollAction();
	ThumbTrackHScrollAction& operator=(const ThumbTrackHScrollAction& source);

	virtual void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};

#endif // !_HSCROLLACTIONS_H
