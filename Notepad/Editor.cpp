#include "Editor.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "Highlight.h"
#include "Selector.h"
#include "Scanner.h"

Editor::Editor(NotepadForm* notepadForm) {
	this->notepadForm = notepadForm;
	this->selector = 0;
}

Editor::Editor(const Editor& source) {
	this->notepadForm = source.notepadForm;
	this->selector = source.selector;
}

Editor::~Editor() {
	if (this->selector != NULL) {
		delete this->selector;
	}
}

Editor& Editor::operator=(const Editor& source) {
	this->notepadForm = source.notepadForm;
	this->selector = source.selector;

	return *this;
}

void Editor::UpSelect(Long noteCurrent, Long lineCurrent, Long row, Long column) {
	Long i = noteCurrent;
	while (i >= row) {
		Glyph* line = this->notepadForm->note->GetAt(i);
		Long startColumn;
		Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
		Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
		Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
		Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
		if (noteStartPosition >= noteEndPosition &&
			(noteStartPosition != noteEndPosition || lineStartPosition >= lineEndPosition)) {
			if (i == noteCurrent) {
				startColumn = lineCurrent;
			}
			else {
				startColumn = line->GetLength();
			}
		}
		else {
			if (i == noteCurrent) {
				if (noteStartPosition == noteEndPosition) {
					startColumn = lineStartPosition;
				}
				else {
					startColumn = 0;
				}
			}
			else {
				if (i == noteStartPosition) {
					if (column > lineStartPosition) {
						startColumn = column;
					}
					else {
						startColumn = lineStartPosition;
					}
				}
				else if (i == row) {
					startColumn = column;
				}
				else {
					startColumn = 0;
				}
			}
		}
		Long endColumn = 0;
		if (i == row) {
			endColumn = column;
		}
		this->selector->Left(i, startColumn, endColumn);
		i--;
	}
}

void Editor::DownSelect(Long noteCurrent, Long lineCurrent, Long row, Long column) {
	Long i = noteCurrent;
	while (i <= row) {
		Glyph* line = this->notepadForm->note->GetAt(i);
		Long startColumn;
		Long noteStartPosition = this->notepadForm->editor->selector->GetNoteStartPosition();
		Long noteEndPosition = this->notepadForm->editor->selector->GetNoteEndPosition();
		Long lineStartPosition = this->notepadForm->editor->selector->GetLineStartPosition();
		Long lineEndPosition = this->notepadForm->editor->selector->GetLineEndPosition();
		if (noteStartPosition <= noteEndPosition &&
			(noteStartPosition != noteEndPosition || lineStartPosition <= lineEndPosition)) {
			if (i == noteCurrent) {
				startColumn = lineCurrent;
			}
			else {
				startColumn = 0;
			}
		}
		else {
			if (i == noteCurrent) {
				if (noteStartPosition == noteEndPosition) {
					startColumn = lineStartPosition;
				}
				else {
					startColumn = line->GetLength();
				}
			}
			else {
				if (i == noteStartPosition) {
					if (column < lineStartPosition) {
						startColumn = column;
					}
					else {
						startColumn = lineStartPosition;
					}
				}
				else if (i == row) {
					startColumn = column;
				}
				else {
					startColumn = 0;
				}
			}
		}
		Long endColumn = line->GetLength();
		if (i == row) {
			endColumn = column;
		}
		this->selector->Right(i, startColumn, endColumn);
		i++;
	}
}

void Editor::Select(Long startRow, Long startColumn, Long rowLength, Long lastColumn) {
	Long i = startRow;
	while (i <= rowLength) {
		Long column = 0;
		Glyph* line = this->notepadForm->note->GetAt(i);
		Long endColumn = line->GetLength() - 1;
		if (i == startRow) {
			column = startColumn;
		}
		else if (i == rowLength) {
			endColumn = lastColumn;
		}

		this->selector->Right(i, column, endColumn);

		i++;
	}
}