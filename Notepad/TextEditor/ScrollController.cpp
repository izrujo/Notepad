#include "ScrollController.h"
#include "Scroll.h"
#include "TextEditingForm.h"
#include "CharacterMetrics.h"
#include "Subject.h"
#include "Line.h"

ScrollController::ScrollController(TextEditingForm* textEditingForm)
	: Observer() {
	this->textEditingForm = textEditingForm;
	this->noteWidth = this->textEditingForm->characterMetrics->GetNoteWidth(this->textEditingForm->note);

	Long index = this->textEditingForm->note->GetLength();
	this->noteHeight = this->textEditingForm->characterMetrics->GetY(index);

	RECT rect;
	this->textEditingForm->GetClientRect(&rect);
	Long clientWidth = rect.right - rect.left;
	Long clientHeight = rect.bottom - rect.top;

	Long maximum = this->noteWidth - clientWidth;
	Long lineSize = this->textEditingForm->characterMetrics->GetWidthAverage();
	Long pageSize = clientWidth;

	this->horizontalScroll = new HorizontalScroll(0, maximum, pageSize, lineSize, 0);

	maximum = this->noteHeight - clientHeight;
	lineSize = this->textEditingForm->characterMetrics->GetHeight();
	pageSize = clientHeight;

	this->verticalScroll = new VerticalScroll(0, maximum, pageSize, lineSize, 0);

	this->textEditingForm->AttachObserver(this);
}

ScrollController::ScrollController(const ScrollController& source)
	: Observer(source) {
	this->textEditingForm = source.textEditingForm;
	this->noteWidth = source.noteWidth;
	this->noteHeight = source.noteHeight;

	this->horizontalScroll = new HorizontalScroll(*(dynamic_cast<HorizontalScroll*>(source.horizontalScroll)));
	this->verticalScroll = new VerticalScroll(*(dynamic_cast<VerticalScroll*>(source.horizontalScroll)));
}

ScrollController::~ScrollController() {
	if (this->horizontalScroll != 0) {
		delete this->horizontalScroll;
	}
	if (this->verticalScroll != 0) {
		delete this->verticalScroll;
	}

	this->textEditingForm->DetachObserver(this);
}

ScrollController& ScrollController::operator=(const ScrollController& source) {
	Observer::operator=(source);

	this->textEditingForm = source.textEditingForm;
	this->noteWidth = source.noteWidth;
	this->noteHeight = source.noteHeight;

	if (this->horizontalScroll != 0) {
		delete this->horizontalScroll;
		this->horizontalScroll = new HorizontalScroll(*(dynamic_cast<HorizontalScroll*>(source.horizontalScroll)));
	}
	if (this->verticalScroll != 0) {
		delete this->verticalScroll;
		this->verticalScroll = new VerticalScroll(*(dynamic_cast<VerticalScroll*>(source.horizontalScroll)));
	}

	return *this;
}

void ScrollController::Update() {
	RECT rect;
	Long lineWidth;
	Long clientWidth;
	Long clientHeight;
	Long index;
	Long maximum;
	Long minimum;
	Long position;
	Long lineSize;
	Long pageSize;
	SCROLLINFO scrollInfo;
	LONG style;
	Long noteLength;

	this->textEditingForm->GetClientRect(&rect);
	style = ::GetWindowLong(this->textEditingForm->m_hWnd, GWL_STYLE);
	UINT disable;

	// 수평 스크롤 생성 부분

	// 노트 가로사이즈, 클라이언트 가로사이즈 구하는 부분
	noteLength = this->textEditingForm->note->GetLength();
	if (noteLength <= 1 && this->textEditingForm->current->GetLength() <= 0) { // 노트의 아무것도 없으면
		this->noteWidth = 0;
	}
	else {
		index = this->textEditingForm->current->GetLength();
		lineWidth = this->textEditingForm->characterMetrics->GetX(this->textEditingForm->current, index);
		if (lineWidth > this->noteWidth) {
			this->noteWidth = lineWidth;
		}
	}
	clientWidth = rect.right - rect.left;
	// 

	if (clientWidth < this->noteWidth && this->textEditingForm->GetIsLockedHScroll() == FALSE) { // 클라이언트 영역이 더 작으면 스크롤 설정및 생성
		style = style | WS_HSCROLL;
		minimum = this->horizontalScroll->GetMinimum();
		maximum = this->noteWidth;//
		position = this->horizontalScroll->GetPosition();
		lineSize = this->textEditingForm->characterMetrics->GetWidthAverage();
		pageSize = clientWidth;//

		if (this->horizontalScroll != 0) {
			delete this->horizontalScroll;
		}
		this->horizontalScroll = new HorizontalScroll(minimum, maximum, pageSize, lineSize, position);
		scrollInfo = this->horizontalScroll->GetScrollInfo();
		disable = ESB_ENABLE_BOTH;
		this->textEditingForm->SetScrollInfo(SB_HORZ, &scrollInfo, TRUE);
	}
	else {
		style = style | WS_HSCROLL;
		if (this->horizontalScroll != 0) {
			delete this->horizontalScroll;
			this->horizontalScroll = new HorizontalScroll(0, 0, 0, 0, 0);
			scrollInfo = this->horizontalScroll->GetScrollInfo();
		}
		disable = ESB_DISABLE_BOTH;
	}
	this->textEditingForm->EnableScrollBar(SB_HORZ, disable);
	::SetWindowLong(this->textEditingForm->m_hWnd, GWL_STYLE, style);

	// 수평 스크롤 생성 부분

	// 수직 스크롤 생성 부분
	index = this->textEditingForm->note->GetLength();
	this->noteHeight = this->textEditingForm->characterMetrics->GetY(index);
	clientHeight = rect.bottom - rect.top;
	if (clientHeight < this->noteHeight) {
		style = style | WS_VSCROLL;
		lineSize = this->textEditingForm->characterMetrics->GetHeight();
		minimum = this->verticalScroll->GetMinimum();
		maximum = this->noteHeight;
		position = this->verticalScroll->GetPosition();
		pageSize = clientHeight / lineSize * lineSize;

		if (this->verticalScroll != 0) {
			delete this->verticalScroll;
		}
		this->verticalScroll = new VerticalScroll(minimum, maximum, pageSize, lineSize, position);
		scrollInfo = this->verticalScroll->GetScrollInfo();
		disable = ESB_ENABLE_BOTH;
		this->textEditingForm->SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
	}
	else {
		style = style | WS_VSCROLL;
		if (this->verticalScroll != 0) {
			delete this->verticalScroll;
			this->verticalScroll = new VerticalScroll(0, 0, 0, 0, 0);
			scrollInfo = this->verticalScroll->GetScrollInfo();
		}
		disable = ESB_DISABLE_BOTH;
	}
	this->textEditingForm->EnableScrollBar(SB_VERT, disable);
	::SetWindowLong(this->textEditingForm->m_hWnd, GWL_STYLE, style);

	this->textEditingForm->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}

Long ScrollController::Left() {
	return this->horizontalScroll->Down();
}

Long ScrollController::Right() {
	return this->horizontalScroll->Up();
}

Long ScrollController::PageLeft() {
	return this->horizontalScroll->PageDown();
}

Long ScrollController::PageRight() {
	return this->horizontalScroll->PageUp();
}

Long ScrollController::Up() {
	return this->verticalScroll->Down();
}

Long ScrollController::Down() {
	return this->verticalScroll->Up();
}

Long ScrollController::PageUp() {
	return this->verticalScroll->PageDown();
}

Long ScrollController::PageDown() {
	return this->verticalScroll->PageUp();
}

Long ScrollController::Rotate(short delta) {
	return this->verticalScroll->Rotate(delta);
}

Long ScrollController::MoveHorizontalScroll(Long position) {
	return this->horizontalScroll->Move(position);
}

Long ScrollController::MoveVerticalScroll(Long position) {
	return this->verticalScroll->Move(position);
}

void ScrollController::SmartScrollToPoint(Long x, Long y) {
	Long distanceOfX = 0;
	Long distanceOfY = 0;
	Long positionX;
	Long previousX;
	Long positionY;
	Long previousY;
	RECT rect;
	this->textEditingForm->GetClientRect(&rect);
	//클라이언트 영역 좌표를 구한다.
	Long clientLeft = this->horizontalScroll->GetPosition();
	Long clientRight = this->horizontalScroll->GetPosition() + rect.right - rect.left;
	Long clientTop = this->verticalScroll->GetPosition() + this->verticalScroll->GetLineSize(); // 클라이언트 영역 축소
	Long clientBottom = this->verticalScroll->GetPosition() + rect.bottom - rect.top;

	//x가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 왼쪽에 있으면
	if (x < clientLeft) {
		//	1.x좌표의 거리와 클라이언트 시작점을 빼다
		distanceOfX = x - clientLeft;
	}//x가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 오른쪽에 있으면
	else if (x > clientRight) {
		//	1. x좌표와 클라이언트 끝점을 빼다
		distanceOfX = x - clientRight;
	}

	positionX = this->horizontalScroll->GetPosition();//현재 위치를 구한다.
	positionX += distanceOfX;//현재 위치에서 구한값을 더하다.
	positionX = this->horizontalScroll->Move(positionX);//수직 컨트롤에서 이동하다.
	// 전 위치를 구하다.
	// 스크롤을 설정하다.
	previousX = this->textEditingForm->SetScrollPos(SB_HORZ, positionX, TRUE);
	positionX = this->textEditingForm->GetScrollPos(SB_HORZ);
	this->horizontalScroll->Move(positionX);

	//클라이언트 영역 좌표를 구한다.
	if (y < clientTop) {//y가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 위에 있으면
		distanceOfY = y - clientTop;//	1.y좌표의 거리와 클라이언트 시작점을 빼다
	}
	else if (y > clientBottom) {//y가 클라이언트 영역 안에 있지 않고 클라이언트 영역보다 위에 있으면
		distanceOfY = y - clientBottom;//	1. 클라이언트 끝점과 y좌표의 거리를 빼다.
	}

	positionY = this->verticalScroll->GetPosition();//현재 위치를 구한다.
	positionY += distanceOfY;//현재 위치에서 구한값을 더하다.
	positionY = this->verticalScroll->Move(positionY);//스크롤 컨트롤러에서 이동하다.
	// 전 위치를 구하다.
	// 스크롤을 설정하다.
	previousY = this->textEditingForm->SetScrollPos(SB_VERT, positionY, TRUE);
	positionY = this->textEditingForm->GetScrollPos(SB_VERT);
	this->verticalScroll->Move(positionY);

	//메모장 윈도우를 스크롤한다.
	this->textEditingForm->ScrollWindow(previousX - positionX, previousY - positionY);
}