#include "PrintingVisitor.h"
#include "../TextEditor/Visitor.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/Note.h"
#include "../TextEditor/Line.h"
#include "../TextEditor/Characters.h"
#include "../TextEditor/CharacterMetrics.h"

//#include <sstream>
#pragma warning(disable:4996)

PrintingVisitor::PrintingVisitor(CDC* dc, CharacterMetrics* characterMetrics, Long x, Long y) {
	this->dc = dc;
	this->characterMetrics = characterMetrics;
	this->x = x;
	this->y = y;
}

PrintingVisitor::~PrintingVisitor() {

}

void PrintingVisitor::Visit(Note* note) {
	Long x = this->x;

	Long i = 0;
	while (i < note->GetLength()) {
		note->GetAt(i)->Accept(this);
		this->x = x;
		this->y += this->characterMetrics->GetHeight();
		i++;
	}
}

void PrintingVisitor::Visit(Line* line) {
	Long i = 0;
	while (i < line->GetLength()) {
		line->GetAt(i)->Accept(this);
		i++;
	}
}

void PrintingVisitor::Visit(Character* character) {
	string content = character->GetContent();

	Long width = 0;
	if (dynamic_cast<SingleByteCharacter*>(character)) {
		width = this->characterMetrics->GetWidth(dynamic_cast<SingleByteCharacter*>(character)->GetChar());
	}
	else if (dynamic_cast<DoubleByteCharacter*>(character)) {
		width = this->characterMetrics->GetDoubleByteWidth();
	}

	this->dc->TextOutA(this->x, this->y, content.c_str());
	this->x += width;
}

void PrintingVisitor::Visit(Book* book) {

}