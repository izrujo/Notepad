#include "FindReplaceDialog.h"
#include "TextEditingForm.h"
#include "Glyph.h"
#include "Selection.h"
#include "DummyManager.h"

FindReplaceDialog::FindReplaceDialog(BOOL findOnly, string findWhat, CWnd* parent)
	: CFindReplaceDialog() {
	(findOnly == TRUE) ? (this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDF)) : (this->m_fr.lpTemplateName = MAKEINTRESOURCE(IDR));
	this->Create(findOnly, (LPCTSTR)findWhat.c_str(), NULL, FR_ENABLETEMPLATE | FR_HIDEWHOLEWORD | FR_DOWN, parent);
	this->textEditingForm = static_cast<TextEditingForm*>(parent);
}

BOOL FindReplaceDialog::OnInitDialog() {
	CFindReplaceDialog::OnInitDialog();

	return FALSE;
}

BOOL FindReplaceDialog::WrapAround() {
	BOOL wrapAround = FALSE;

	int check = ((CButton*)GetDlgItem(IDCNT_CHECKBOX_WRAPAROUND))->GetCheck();

	if (check == BST_CHECKED) {
		wrapAround = TRUE;
	}

	return wrapAround;
}

BOOL FindReplaceDialog::Find() {
	BOOL isFindSuccess;
	//TextEditingForm* textEditingForm = (TextEditingForm*)this->GetParent();

	BOOL isSearchDown = this->SearchDown();

	//========== �ڵ� ���� ó�� 1 ==========
	DummyManager* dummyManager = 0;
	Long currentRow = this->textEditingForm->note->GetCurrent();
	Long currentColumn = this->textEditingForm->current->GetCurrent();
	Long start = currentRow;
	Long end = this->textEditingForm->note->GetLength() - 1;
	if (isSearchDown == FALSE) {
		start = 0;
		end = currentRow;
	}
	Long distance = 0;
	if (this->textEditingForm->GetIsLockedHScroll() == TRUE) {
		CRect rect;
		this->textEditingForm->GetClientRect(rect);
		dummyManager = new DummyManager(this->textEditingForm->note, this->textEditingForm->characterMetrics, rect.Width());
		distance = dummyManager->CountDistance(currentRow, currentColumn);
		dummyManager->Unfold(&start, &end); //���� ����� ������.
		dummyManager->CountIndex(distance, &currentRow, &currentColumn);
		this->textEditingForm->note->Move(currentRow);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(currentRow);
		this->textEditingForm->current->Move(currentColumn);
	}
	//========== �ڵ� ���� ó�� 1 ==========

	String allContents = this->textEditingForm->note->GetContent();
	CString findString = this->GetFindString();
	String myFindString((LPCTSTR)findString);
	Long stringLength = myFindString.GetLength();
	
	BOOL wrapAround = this->WrapAround(); //Ŀ�����ؼ� '������ ��ġ'�� �ٲٱ�
	BOOL matchCase = this->MatchCase();
	if (matchCase == FALSE) {
		allContents.MakeLower();
		myFindString.MakeLower();
	}

	//String ������ ĳ����ġ ã��
	Long current = 0;
	Long lineLength;
	Long i = 0;
	while (i < this->textEditingForm->note->GetCurrent()) {
		lineLength = this->textEditingForm->note->GetAt(i)->GetLength();
		current += lineLength + 1;
		i++;
	}
	if (i == this->textEditingForm->note->GetCurrent()) {
		current += this->textEditingForm->note->GetAt(i)->GetCurrent();
	}

	if (isSearchDown == TRUE) {
		current = allContents.Find((char*)(LPCTSTR)findString, current);
		if (wrapAround == TRUE && current == -1) {
			this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLHOME, 0));
			current = 0; //String ������ ĳ����ġ - distance
			i = 0;
			while (i < this->textEditingForm->note->GetCurrent()) {
				lineLength = this->textEditingForm->note->GetAt(i)->GetLength();
				current += lineLength + 1;
				i++;
			}
			if (i == this->textEditingForm->note->GetCurrent()) {
				current += this->textEditingForm->note->GetAt(i)->GetCurrent();
			}
			current = allContents.Find((char*)(LPCTSTR)findString, current);
		}
	}
	else {
		current = allContents.ReversedFind((char*)(LPCTSTR)findString, current);
		if (wrapAround == TRUE && current == -1) {
			this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_CTRLEND, 0));
			current = 0; //String ������ ĳ����ġ
			i = 0;
			while (i < this->textEditingForm->note->GetCurrent()) {
				lineLength = this->textEditingForm->note->GetAt(i)->GetLength();
				current += lineLength + 1;
				i++;
			}
			if (i == this->textEditingForm->note->GetCurrent()) {
				current += this->textEditingForm->note->GetAt(i)->GetCurrent();
			}
			current = allContents.ReversedFind((char*)(LPCTSTR)findString, current);
		}
	}
	Long selectStart=0;
	Long startDistance=0;
	Long index = 0;
	if (current != -1) {
		if (this->textEditingForm->selection != NULL) {
			delete this->textEditingForm->selection;
			this->textEditingForm->selection = NULL;
			this->textEditingForm->note->UnselectAll();
		}

		//Glyph �� ��ġ ���ϰ� �̵��ϱ�
		if (isSearchDown == FALSE) {
			current += stringLength;
		}
		i = 0;
		Long linesLength = 0;
		while (linesLength <= current) {
			linesLength += this->textEditingForm->note->GetAt(i++)->GetLength() + 1;
		}

		this->textEditingForm->note->Move(--i);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(i);
		linesLength -= this->textEditingForm->note->GetAt(i)->GetLength() + 1;
		index = current - linesLength;
		this->textEditingForm->current->Move(index);

		//�����ϱ�
		i = 1;
		while (i <= stringLength) {
			if (isSearchDown == TRUE) {
				if (this->textEditingForm->current->GetCurrent() >= this->textEditingForm->current->GetLength()) {
					this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_RIGHT, 0));
				}
				this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_RIGHT, 0));
			}
			else {
				if (this->textEditingForm->current->GetCurrent() <= 0) {
					this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_MOVE_LEFT, 0));
				}
				this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_SELECTMOVE_LEFT, 0));
			}
			i++;
		}
		isFindSuccess = TRUE;
		selectStart = this->textEditingForm->selection->GetStart(); //�ڵ�����
	}
	else {
		isFindSuccess = FALSE;
	}

	//========== �ڵ� ���� ó�� 2 ==========
	if (dummyManager != NULL) {
		startDistance = dummyManager->CountDistance(selectStart, index); //�ڵ�����
		currentRow = this->textEditingForm->note->GetCurrent();
		currentColumn = this->textEditingForm->current->GetCurrent();
		distance = dummyManager->CountDistance(currentRow, currentColumn);

		Long lastFoldedRow;
		Long i = start;
		while (i <= end && end < this->textEditingForm->note->GetLength()) {
			lastFoldedRow = dummyManager->Fold(i);
			end += lastFoldedRow - i;
			i = lastFoldedRow + 1;
		}
		dummyManager->CountIndex(distance, &currentRow, &currentColumn);
		dummyManager->CountIndex(startDistance, &selectStart, &index);

		delete dummyManager;

		if (this->textEditingForm->selection != NULL && isFindSuccess == TRUE) {
			delete this->textEditingForm->selection;
			this->textEditingForm->selection = NULL;
			this->textEditingForm->selection = new Selection(selectStart, currentRow);
		}

		this->textEditingForm->note->Move(currentRow);
		this->textEditingForm->current = this->textEditingForm->note->GetAt(currentRow);
		this->textEditingForm->current->Move(currentColumn);
	}
	//========== �ڵ� ���� ó�� 2 ==========

	return isFindSuccess;
}

void FindReplaceDialog::Replace() {
	TextEditingForm* textEditingForm = (TextEditingForm*)this->GetParent();
	//���õ� ���ڵ�(ã��)�� �����.
	if (this->textEditingForm->selection != NULL) {
		this->textEditingForm->SetIsDeleteSelectionByInput(TRUE);
		this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_DELETESELECTION, 0));
		this->textEditingForm->SetIsDeleteSelectionByInput(FALSE);

		//���� �ڸ��� �ٲ� ������ ����.
		CString replaceString = this->GetReplaceString();
		String myReplaceString((LPCTSTR)replaceString);
		Long stringLength = myReplaceString.GetLength();
		Long i = 0;
		while (i < replaceString.GetLength()) {
			if (myReplaceString.IsHangle(i) == false) {
				this->textEditingForm->SetCurrentCharacter(myReplaceString.GetAt(i));
				this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_WRITE, 0));
			}
			else {
				this->textEditingForm->SetCurrentBuffer((TCHAR*)myReplaceString.GetDoubleByteAt(i).c_str());
				i++;
				this->textEditingForm->SendMessage(WM_COMMAND, MAKEWPARAM(IDCNT_EDIT_IMECHAR, 0));
			}
			i++;
		}
	}
}