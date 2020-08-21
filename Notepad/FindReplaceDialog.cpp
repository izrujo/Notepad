#include "FindReplaceDialog.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "Selection.h"

FindReplaceDialog::FindReplaceDialog(BOOL findOnly, string findWhat, CWnd* parent)
	: CFindReplaceDialog() {
	(findOnly == TRUE) ? (this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDF)) : (this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDR));
	this->Create(findOnly, (LPCTSTR)findWhat.c_str(), NULL, FR_ENABLETEMPLATE | FR_HIDEWHOLEWORD | FR_DOWN, parent);
}

BOOL FindReplaceDialog::OnInitDialog() {
	CFindReplaceDialog::OnInitDialog();

	return FALSE;
}

BOOL FindReplaceDialog::WrapAround() {
	BOOL wrapAround = FALSE;

	int check = ((CButton*)GetDlgItem(IDC_CHECKBOX_WRAPAROUND))->GetCheck();

	if (check == BST_CHECKED) {
		wrapAround = TRUE;
	}

	return wrapAround;
}

BOOL FindReplaceDialog::Find() {
	BOOL isFindSuccess;
	NotepadForm* notepadForm = (NotepadForm*)this->GetParent();

	String allContents = notepadForm->note->GetContent();
	CString findString = this->GetFindString();
	String myFindString((LPCTSTR)findString);
	Long stringLength = myFindString.GetLength();

	BOOL isSearchDown = this->SearchDown();
	BOOL wrapAround = this->WrapAround(); //커스텀해서 '주위에 배치'로 바꾸기
	BOOL matchCase = this->MatchCase();
	if (matchCase == FALSE) {
		allContents.MakeLower();
		myFindString.MakeLower();
	}

	//String 에서의 캐럿위치 찾기
	Long current = 0;
	Long lineLength;
	Long i = 0;
	while (i < notepadForm->note->GetCurrent()) {
		lineLength = notepadForm->note->GetAt(i)->GetLength();
		current += lineLength + 1;
		i++;
	}
	if (i == notepadForm->note->GetCurrent()) {
		current += notepadForm->note->GetAt(i)->GetCurrent();
	}

	if (isSearchDown == TRUE) {
		current = allContents.Find((char*)(LPCTSTR)findString, current);
		if (wrapAround == TRUE && current == -1) {
			notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_CTRLHOME, 0));
			current = 0; //String 에서의 캐럿위치
			i = 0;
			while (i < notepadForm->note->GetCurrent()) {
				lineLength = notepadForm->note->GetAt(i)->GetLength();
				current += lineLength + 1;
				i++;
			}
			if (i == notepadForm->note->GetCurrent()) {
				current += notepadForm->note->GetAt(i)->GetCurrent();
			}
			current = allContents.Find((char*)(LPCTSTR)findString, current);
		}
	}
	else {
		current = allContents.ReversedFind((char*)(LPCTSTR)findString, current);
		if (wrapAround == TRUE && current == -1) {
			notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_CTRLEND, 0));
			current = 0; //String 에서의 캐럿위치
			i = 0;
			while (i < notepadForm->note->GetCurrent()) {
				lineLength = notepadForm->note->GetAt(i)->GetLength();
				current += lineLength + 1;
				i++;
			}
			if (i == notepadForm->note->GetCurrent()) {
				current += notepadForm->note->GetAt(i)->GetCurrent();
			}
			current = allContents.ReversedFind((char*)(LPCTSTR)findString, current);
		}
	}

	if (current != -1) {
		if (notepadForm->selection != NULL) {
			delete notepadForm->selection;
			notepadForm->selection = NULL;
			notepadForm->note->UnselectAll();
		}

		//Glyph 상 위치 구하고 이동하기
		if (isSearchDown == FALSE) {
			current += stringLength;
		}
		Long index;
		i = 0;
		Long linesLength = 0;
		while (linesLength <= current) {
			linesLength += notepadForm->note->GetAt(i++)->GetLength() + 1;
		}

		notepadForm->note->Move(--i);
		notepadForm->current = notepadForm->note->GetAt(i);
		linesLength -= notepadForm->note->GetAt(i)->GetLength() + 1;
		index = current - linesLength;
		notepadForm->current->Move(index);

		//선택하기
		i = 1;
		while (i <= stringLength) {
			if (isSearchDown == TRUE) {
				if (notepadForm->current->GetCurrent() >= notepadForm->current->GetLength()) {
					notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_RIGHT, 0));
				}
				notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SELECTMOVE_RIGHT, 0));
			}
			else {
				if (notepadForm->current->GetCurrent() <= 0) {
					notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_MOVE_LEFT, 0));
				}
				notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SELECTMOVE_LEFT, 0));
			}
			i++;
		}
		isFindSuccess = true;
	}
	else {
		isFindSuccess = false;
	}

	return isFindSuccess;
}

void FindReplaceDialog::Replace() {
	NotepadForm* notepadForm = (NotepadForm*)this->GetParent();
	//선택된 글자들(찾은)을 지우다.
	if (notepadForm->selection != NULL) {
		notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDM_EDIT_DELETE, 0));

		//지운 자리에 바꿀 내용을 적다.
		CString replaceString = this->GetReplaceString();
		String myReplaceString((LPCTSTR)replaceString);
		Long stringLength = myReplaceString.GetLength();
		Long i = 0;
		while (i < replaceString.GetLength()) {
			if (myReplaceString.IsHangle(i) == false) {
				notepadForm->SetCurrentCharacter(myReplaceString.GetAt(i));
				notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_WRITE_CHAR, 0));
			}
			else {
				notepadForm->SetCurrentBuffer((TCHAR*)myReplaceString.GetDoubleByteAt(i).c_str());
				i++;
				notepadForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_IME_CHAR, 0));
			}
			i++;
		}
	}
}