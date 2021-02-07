#include "DrawingVisitor.h"
#include "Glyph.h"
#include "Note.h"
#include "Line.h"
#include "Characters.h"
#include "CharacterMetrics.h"
#include "Scroll.h"
#include "ScrollController.h"
#include "Selection.h"

//#include <sstream>
#pragma warning(disable:4996)

DrawingVisitor::DrawingVisitor(CDC* dc, CharacterMetrics* characterMetrics, ScrollController* scrollController, Selection* selection) {
	this->dc = dc;
	this->characterMetrics = characterMetrics;
	this->scrollController = scrollController;
	this->selection = selection; //임시
	this->x = 0;
	this->y = 0;
}

DrawingVisitor::~DrawingVisitor() {

}

void DrawingVisitor::Visit(Note* note) {
	Long y = 0;
	if (this->scrollController != NULL) {
		y = this->scrollController->GetVerticalScroll()->GetPosition();
	}
	Long begin = this->characterMetrics->GetRow(y);

	//임시
	Long start = -1;
	Long end = -1;
	if (this->selection != NULL) {
		start = this->selection->GetStart();
		end = this->selection->GetEnd();
	}
	//임시

	COLORREF oldColor = this->dc->GetTextColor();
	Long i = begin;
	while (i < note->GetLength()) {
		this->y = i * this->characterMetrics->GetHeight() - y;
		if (i == start || i == end) { //임시
			//this->dc->MoveTo(this->x + 10, this->y);
			//this->dc->LineTo(this->x + 10, this->y + this->characterMetrics->GetHeight()); //임시
			oldColor = this->dc->SetTextColor(RGB(255, 0, 0));
		}
		note->GetAt(i)->Accept(this);
		this->dc->SetTextColor(oldColor);
		i++;
	}
}

void DrawingVisitor::Visit(Line* line) {
	Long x = 0;
	if (this->scrollController != NULL) {
		x = this->scrollController->GetHorizontalScroll()->GetPosition();
	}
	Long begin = this->characterMetrics->GetColumnForDrawing(line, x);
	this->x = this->characterMetrics->GetX(line, begin);

	Long i = begin;
	while (i < line->GetLength()) {
		line->GetAt(i)->Accept(this);
		i++;
	}
}

void DrawingVisitor::Visit(Character* character) {
	COLORREF oldBkColor = this->dc->GetBkColor();
	COLORREF oldColor = this->dc->GetTextColor();

	string content = character->GetContent();
	bool isSelected = character->GetIsSelected();

	Long x = 0;
	if (this->scrollController != NULL) {
		x = this->scrollController->GetHorizontalScroll()->GetPosition();
	}

	Long width = 0;
	if (dynamic_cast<SingleByteCharacter*>(character)) {
		width = this->characterMetrics->GetWidth(dynamic_cast<SingleByteCharacter*>(character)->GetChar());
	}
	else if (dynamic_cast<DoubleByteCharacter*>(character)) {
		width = this->characterMetrics->GetDoubleByteWidth();
	}

	if (isSelected == true) {
		oldBkColor = this->dc->SetBkColor(RGB(0, 0, 235));
		oldColor = this->dc->SetTextColor(RGB(255, 255, 255));
	}

	this->dc->TextOutA(this->x - x, this->y, content.c_str());
	this->x += width;

	this->dc->SetTextColor(oldColor);
	this->dc->SetBkColor(oldBkColor);
}

void DrawingVisitor::Visit(Book* book) {

}