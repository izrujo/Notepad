#include "LineDivider.h"
#include "Glyph.h"
#include "DummyLine.h"
#include "CharacterMetrics.h"

LineDivider::LineDivider(CharacterMetrics* characterMetrics) {
	this->characterMetrics = characterMetrics;
}

LineDivider::LineDivider(const LineDivider& source) {
	this->characterMetrics = characterMetrics;
}

LineDivider::~LineDivider() {

}

LineDivider& LineDivider::operator=(const LineDivider& source) {
	this->characterMetrics = source.characterMetrics;

	return *this;
}

Glyph* LineDivider::Divide(Long clientWidth, Long clientHeight, Glyph* note, Long row, Long column) {
	Long characterHeight = this->characterMetrics->GetHeight();
	clientHeight -= characterHeight;

	Glyph* line;
	Glyph* next;
	Glyph* dummy;
	Long cutColumn;
	Long width;
	Long height = 0;
	Long j;

	Long i = 0;
	while (i < note->GetLength() && height < clientHeight) {
		line = note->GetAt(i);
		width = this->characterMetrics->GetX(line, line->GetLength());
		if (width >= clientWidth) {
			dummy = new DummyLine;
			cutColumn = this->characterMetrics->GetColumnForDrawing(line, clientWidth);
			next = line->Divide(cutColumn);
			j = 0;
			while (j < next->GetLength()) {
				dummy->Add(next->GetAt(j)->Clone());
				j++;
			}
			note->Add(i + 1, dummy);

			if (row > i) {
				row++;
			}
			else if (row == i && column > cutColumn) {
				row++;
				column -= cutColumn;
			}

			if (next != 0) {
				delete next;
			}
		}
		height += characterHeight;
		i++;
	}

	note->Move(row);
	note->GetAt(row)->Move(column);

	Glyph* remainNote = 0;
	if (i < note->GetLength()) {
		remainNote = note->Divide(i);
	}

	return remainNote;
}

void LineDivider::Combine(Glyph* note, Long* row, Long* column) {
	*row = note->GetCurrent();
	*column = note->GetAt(*row)->GetCurrent();

	Glyph* line;
	Glyph* previous;
	Long i = note->GetLength() - 1;
	while (i > 0) {
		previous = note->GetAt(i - 1);
		line = note->GetAt(i);
		if (dynamic_cast<DummyLine*>(line)) {
			if (*row >= i) {
				if (*row == i) {
					*column += previous->GetLength();
				}
				*row--;
			}
			previous->Combine(line);
			note->Remove(i);
		}
		i--;
	}
}