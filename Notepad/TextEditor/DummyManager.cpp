#include "DummyManager.h"
#include "Glyph.h"
#include "DummyLine.h"
#include "CharacterMetrics.h"

DummyManager::DummyManager(Glyph* note, CharacterMetrics* characterMetrics, Long width) {
	this->note = note;
	this->characterMetrics = characterMetrics;
	this->width = width;
}

DummyManager::DummyManager(const DummyManager& source) {
	this->note = source.note;
	this->characterMetrics = source.characterMetrics;
	this->width = source.width;
}

DummyManager::~DummyManager() {

}

DummyManager& DummyManager::operator=(const DummyManager& source) {
	this->note = source.note;
	this->characterMetrics = source.characterMetrics;
	this->width = source.width;

	return *this;
}

Long DummyManager::Unfold(Long row) {
	Glyph* line = this->note->GetAt(row);
	while (dynamic_cast<DummyLine*>(line)) {
		row--;
		line = this->note->GetAt(row);
	}
	if (row < this->note->GetLength() - 1) {
		Glyph* next = this->note->GetAt(row + 1);
		while (row < this->note->GetLength() - 1 && dynamic_cast<DummyLine*>(next)) {
			line->Combine(next);
			this->note->Remove(row + 1);
			next = this->note->GetAt(row + 1);
		}
	}

	return row;
}

void DummyManager::Unfold(Long* start, Long* end) {
	Glyph* line;
	Glyph* next;

	line = this->note->GetAt(*start);
	while (dynamic_cast<DummyLine*>(line)) {
		(*start)--;
		line = this->note->GetAt(*start);
	}

	Long i = *start;
	while (i <= *end && i < this->note->GetLength() - 1) {
		line = this->note->GetAt(i);
		next = this->note->GetAt(i + 1);
		while (i < this->note->GetLength() - 1 && dynamic_cast<DummyLine*>(next)) {
			line->Combine(next);
			this->note->Remove(i + 1);
			if (i < *end) {
				(*end)--;
			}
			next = this->note->GetAt(i + 1);
		}
		i++;
	}
}

Long DummyManager::Fold(Long unfoldedRow) {
	Long cutColumn;
	Glyph* character;
	Glyph* divided;
	Glyph* dummy;
	Long cutLength;
	string content;
	Long count;
	Long i;
	Long lastFoldedRow = unfoldedRow;

	Glyph* line = this->note->GetAt(unfoldedRow);
	Long length = line->GetLength();
	Long lineWidth = this->characterMetrics->GetX(line, length);
	while (lineWidth > this->width) {
		count = 0;
		cutColumn = this->characterMetrics->GetColumn(line, this->width);
		character = line->GetAt(cutColumn);
		content = character->GetContent();
		while ((content == "." || content == "," || content == "!" || content == "?"
			|| content == ":" || content == ";" || content == ")" || content == ">"
			|| content == "]" || content == " ") && count <= 2) {
			count++;
			character = line->GetAt(--cutColumn);
			content = character->GetContent();
		}
		divided = line->Divide(cutColumn);
		cutLength = divided->GetLength();
		dummy = new DummyLine(cutLength);
		i = 0;
		while (i < cutLength) {
			character = divided->GetAt(i);
			dummy->Add(i, character->Clone());
			i++;
		}
		if (divided != NULL) {
			delete divided;
			divided = NULL;
		}
		lastFoldedRow = this->note->Add(lastFoldedRow + 1, dummy);
		line = this->note->GetAt(lastFoldedRow);
		length = line->GetLength();
		lineWidth = this->characterMetrics->GetX(line, length);
	}

	return lastFoldedRow;
}

Long DummyManager::CountDistance(Long row, Long column) {
	Long distance = 0;
	Glyph* line;
	Long length = 0;

	Long i = 0;
	while (i <= row) {
		if (i < this->note->GetLength()) {
			line = this->note->GetAt(i);
			if (!dynamic_cast<DummyLine*>(line) && i > 0) {
				distance++;
			}
			length = line->GetLength();
		}
		if (i >= row) {
			length = column;
		}
		distance += length;
		i++;
	}

	return distance;
}

void DummyManager::CountIndex(Long distance, Long* row, Long* column) {
	*row = 0;
	Glyph* line;
	Long noteLength = this->note->GetLength();
	Long length = 0;
	Long count = 0;
	while (count < distance && *row < noteLength) {
		line = this->note->GetAt(*row);
		if (!dynamic_cast<DummyLine*>(line) && *row > 0) {
			count++;
		}
		length = line->GetLength();
		(*row)++;
		count += length;
	}
	if (count >= distance) {
		*column = length - (count - distance);
	}
	else {
		*column = length;
	}

	if (*row > 0) {
		(*row)--;
	}
}