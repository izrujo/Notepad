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

	// ���� ��ũ�� ���� �κ�

	// ��Ʈ ���λ�����, Ŭ���̾�Ʈ ���λ����� ���ϴ� �κ�
	noteLength = this->textEditingForm->note->GetLength();
	if (noteLength <= 1 && this->textEditingForm->current->GetLength() <= 0) { // ��Ʈ�� �ƹ��͵� ������
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

	if (clientWidth < this->noteWidth && this->textEditingForm->GetIsLockedHScroll() == FALSE) { // Ŭ���̾�Ʈ ������ �� ������ ��ũ�� ������ ����
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

	// ���� ��ũ�� ���� �κ�

	// ���� ��ũ�� ���� �κ�
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
	//Ŭ���̾�Ʈ ���� ��ǥ�� ���Ѵ�.
	Long clientLeft = this->horizontalScroll->GetPosition();
	Long clientRight = this->horizontalScroll->GetPosition() + rect.right - rect.left;
	Long clientTop = this->verticalScroll->GetPosition() + this->verticalScroll->GetLineSize(); // Ŭ���̾�Ʈ ���� ���
	Long clientBottom = this->verticalScroll->GetPosition() + rect.bottom - rect.top;

	//x�� Ŭ���̾�Ʈ ���� �ȿ� ���� �ʰ� Ŭ���̾�Ʈ �������� ���ʿ� ������
	if (x < clientLeft) {
		//	1.x��ǥ�� �Ÿ��� Ŭ���̾�Ʈ �������� ����
		distanceOfX = x - clientLeft;
	}//x�� Ŭ���̾�Ʈ ���� �ȿ� ���� �ʰ� Ŭ���̾�Ʈ �������� �����ʿ� ������
	else if (x > clientRight) {
		//	1. x��ǥ�� Ŭ���̾�Ʈ ������ ����
		distanceOfX = x - clientRight;
	}

	positionX = this->horizontalScroll->GetPosition();//���� ��ġ�� ���Ѵ�.
	positionX += distanceOfX;//���� ��ġ���� ���Ѱ��� ���ϴ�.
	positionX = this->horizontalScroll->Move(positionX);//���� ��Ʈ�ѿ��� �̵��ϴ�.
	// �� ��ġ�� ���ϴ�.
	// ��ũ���� �����ϴ�.
	previousX = this->textEditingForm->SetScrollPos(SB_HORZ, positionX, TRUE);
	positionX = this->textEditingForm->GetScrollPos(SB_HORZ);
	this->horizontalScroll->Move(positionX);

	//Ŭ���̾�Ʈ ���� ��ǥ�� ���Ѵ�.
	if (y < clientTop) {//y�� Ŭ���̾�Ʈ ���� �ȿ� ���� �ʰ� Ŭ���̾�Ʈ �������� ���� ������
		distanceOfY = y - clientTop;//	1.y��ǥ�� �Ÿ��� Ŭ���̾�Ʈ �������� ����
	}
	else if (y > clientBottom) {//y�� Ŭ���̾�Ʈ ���� �ȿ� ���� �ʰ� Ŭ���̾�Ʈ �������� ���� ������
		distanceOfY = y - clientBottom;//	1. Ŭ���̾�Ʈ ������ y��ǥ�� �Ÿ��� ����.
	}

	positionY = this->verticalScroll->GetPosition();//���� ��ġ�� ���Ѵ�.
	positionY += distanceOfY;//���� ��ġ���� ���Ѱ��� ���ϴ�.
	positionY = this->verticalScroll->Move(positionY);//��ũ�� ��Ʈ�ѷ����� �̵��ϴ�.
	// �� ��ġ�� ���ϴ�.
	// ��ũ���� �����ϴ�.
	previousY = this->textEditingForm->SetScrollPos(SB_VERT, positionY, TRUE);
	positionY = this->textEditingForm->GetScrollPos(SB_VERT);
	this->verticalScroll->Move(positionY);

	//�޸��� �����츦 ��ũ���Ѵ�.
	this->textEditingForm->ScrollWindow(previousX - positionX, previousY - positionY);
}