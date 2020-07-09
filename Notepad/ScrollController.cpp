#include "ScrollController.h"
#include "Scroll.h"
#include "NotepadForm.h"
#include "CharacterMetrics.h"
#include "Subject.h"
#include "Line.h"

ScrollController::ScrollController(NotepadForm* notepadForm)
	: Observer() {
	this->notepadForm = notepadForm;
	this->noteWidth = this->notepadForm->characterMetrics->GetNoteWidth(this->notepadForm->note);

	Long index = this->notepadForm->note->GetLength();
	this->noteHeight = this->notepadForm->characterMetrics->GetY(index);

	RECT rect;
	this->notepadForm->GetClientRect(&rect);
	Long clientWidth = rect.right - rect.left;
	Long clientHeight = rect.bottom - rect.top;

	Long maximum = this->noteWidth - clientWidth;
	Long lineSize = this->notepadForm->characterMetrics->GetWidthAverage();
	Long pageSize = clientWidth;

	this->horizontalScroll = new HorizontalScroll(0, maximum, pageSize, lineSize, 0);

	maximum = this->noteHeight - clientHeight;
	lineSize = this->notepadForm->characterMetrics->GetHeight();
	pageSize = clientHeight;

	this->verticalScroll = new VerticalScroll(0, maximum, pageSize, lineSize, 0);

	this->notepadForm->AttachObserver(this);
}

ScrollController::ScrollController(const ScrollController& source)
	: Observer(source) {
	this->notepadForm = source.notepadForm;
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

	this->notepadForm->DetachObserver(this);
}

ScrollController& ScrollController::operator=(const ScrollController& source) {
	Observer::operator=(source);

	this->notepadForm = source.notepadForm;
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

	this->notepadForm->GetClientRect(&rect);

	// ���� ��ũ�� ���� �κ�

	// ��Ʈ ���λ�����, Ŭ���̾�Ʈ ���λ����� ���ϴ� �κ�
	noteLength = this->notepadForm->note->GetLength();
	if (noteLength <= 1 && this->notepadForm->current->GetLength() <= 0) { // ��Ʈ�� �ƹ��͵� ������
		this->noteWidth = 0;
	}
	else {
		index = this->notepadForm->current->GetLength();
		lineWidth = this->notepadForm->characterMetrics->GetX(this->notepadForm->current, index);
		if (lineWidth > this->noteWidth) {
			this->noteWidth = lineWidth;
		}
	}
	clientWidth = rect.right - rect.left;
	style = ::GetWindowLong(this->notepadForm->m_hWnd, GWL_STYLE);
	// 

	if (clientWidth < this->noteWidth) { // Ŭ���̾�Ʈ ������ �� ������ ��ũ�� ������ ����
		style = style | WS_HSCROLL;
		minimum = this->horizontalScroll->GetMinimum();
		maximum = this->noteWidth;//
		position = this->horizontalScroll->GetPosition();
		lineSize = this->notepadForm->characterMetrics->GetWidthAverage();
		pageSize = clientWidth;//

		if (this->horizontalScroll != 0) {
			delete this->horizontalScroll;
		}
		this->horizontalScroll = new HorizontalScroll(minimum, maximum, pageSize, lineSize, position);
		scrollInfo = this->horizontalScroll->GetScrollInfo();
	}
	else {
		style = style & ~WS_HSCROLL;
		if (this->horizontalScroll != 0) {
			delete this->horizontalScroll;
			this->horizontalScroll = new HorizontalScroll(0, 0, 0, 0, 0);
			scrollInfo = this->horizontalScroll->GetScrollInfo();
		}
	}

	this->notepadForm->SetScrollInfo(SB_HORZ, &scrollInfo, TRUE);
	// ���� ��ũ�� ���� �κ�

	// ���� ��ũ�� ���� �κ�
	index = this->notepadForm->note->GetLength();
	this->noteHeight = this->notepadForm->characterMetrics->GetY(index);
	clientHeight = rect.bottom - rect.top;
	if (clientHeight < this->noteHeight) {
		style = style | WS_VSCROLL;
		lineSize = this->notepadForm->characterMetrics->GetHeight();
		minimum = this->verticalScroll->GetMinimum();
		maximum = this->noteHeight;
		position = this->verticalScroll->GetPosition();
		pageSize = clientHeight / lineSize * lineSize;

		if (this->verticalScroll != 0) {
			delete this->verticalScroll;
		}
		this->verticalScroll = new VerticalScroll(minimum, maximum, pageSize, lineSize, position);
		scrollInfo = this->verticalScroll->GetScrollInfo();
		this->notepadForm->SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
	}
	else {
		style = style & ~WS_VSCROLL;
		if (this->verticalScroll != 0) {
			delete this->verticalScroll;
			this->verticalScroll = new VerticalScroll(0, 0, 0, 0, 0);
			scrollInfo = this->verticalScroll->GetScrollInfo();

		}

	}
	this->notepadForm->SetScrollInfo(SB_VERT, &scrollInfo, TRUE);
	::SetWindowLong(this->notepadForm->m_hWnd, GWL_STYLE, style);
	
	this->notepadForm->RedrawWindow();
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
	this->notepadForm->GetClientRect(&rect);
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
	previousX = this->notepadForm->SetScrollPos(SB_HORZ, positionX, TRUE);
	positionX = this->notepadForm->GetScrollPos(SB_HORZ);
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
	previousY = this->notepadForm->SetScrollPos(SB_VERT, positionY, TRUE);
	positionY = this->notepadForm->GetScrollPos(SB_VERT);
	this->verticalScroll->Move(positionY);

	//�޸��� �����츦 ��ũ���Ѵ�.
	this->notepadForm->ScrollWindow(previousX - positionX, previousY - positionY);
}