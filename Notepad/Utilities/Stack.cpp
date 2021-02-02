#if 0

#include "Stack.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	Long object;
	Long top;
	Long ret;
	bool isEmpty;

	//1. ������ �����.
	Stack<Long> stack(2);
	//2. 100�� �ִ�.
	object = 100;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;
	//3. 200�� �ִ�.
	object = 200;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;
	//4. 50�� �ִ�.
	object = 50;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;

	//���� ������
	Stack<Long> other(stack);
	cout << "other top : " << other.GetTop() << " other obj : " << other.Top() << endl;
	object = 40;
	top = other.Push(object);
	cout << "other top : " << top << " other obj : " << other.Top() << endl;

	//5. ������.
	ret = stack.Pop();
	if (ret == -1) {
		cout << "������." << endl;
	}
	cout << "top : " << stack.GetTop() << " obj : " << stack.Top() << endl;
	//����ִ��� Ȯ���Ѵ�.
	isEmpty = stack.IsEmpty();
	if (isEmpty == true) {
		cout << "����ִ�." << endl;
	}
	else {
		cout << "������� �ʴ�." << endl;
	}
	//6. ����.
	stack.Empty();
	if (stack.GetTop() == 0) {
		cout << "��� ������." << endl;
	}
	//����ִ��� Ȯ���Ѵ�.
	isEmpty = stack.IsEmpty();
	if (isEmpty == true) {
		cout << "����ִ�." << endl;
	}
	else {
		cout << "������� �ʴ�." << endl;
	}
	//7. 20�� �ִ�.
	object = 20;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;

	//ġȯ ������
	stack = other;
	cout << "top : " << stack.GetTop() << " obj : " << stack.Top() << endl;

	//8. ������ �����.

	return 0;
}

//========== ���� ��� �߰� ==========
string type = command->GetType();
BOOL condition1 = FALSE;
BOOL condition2 = FALSE;
History* history;
if (undoHistoryBook->GetLength() > 0) {
	history = this->undoHistoryBook->LookAt();
	Command* previousCommand = history->Top();

	//���� Ŀ�ǵ尡 Write�̰� ����Ű �Է��� �ƴϰų� ���� Ŀ�ǵ尡 ImeChar�̰�
	//���� Ŀ�ǵ尡 Write�̰ų� ImeChar�̸�
	//���� �����丮�� Ŀ�ǵ带 �ִ´�.
	if ((type == "Write" && this->currentCharacter != VK_RETURN)
		|| type == "ImeChar") {
		condition1 = TRUE;
	}
	if (previousCommand->GetType() == "Write" || previousCommand->GetType() == "ImeChar") {
		condition2 = TRUE;
	}

	if (condition1 == TRUE && condition2 == TRUE) {
		history->Push(command->Clone());
	}
}
//���� ��� ����å�� ����ų�
//����� ������ ���� Ŀ�ǵ尡 Write, ImeChar, Delete, Backspace, Paste, Cut, DeleteSelection �߿� �ϳ����� �ϰ�
//������� ������
//���� Ŀ�ǵ尡 Write�� ImeChar�� �ƴϰ�
//Write�� ImeChar�� �ϴ��� Write�� ��� ����Ű �Է��̸� �� ����Ű �Է��̶� �ϴ���
//���� Ŀ�ǵ尡 Write�� ImeChar�� �ƴϸ�
//�� �����丮�� ����� ����.
if ((type == "Wirte" || type == "ImeChar" || type == "Delete" || type == "Backspace"
	|| type == "Paste" || type == "Cut" || type == "DeleteSelection")
	&& (condition1 == FALSE || condition2 == FALSE)) {
	history = new History;
	history->Push(command->Clone());
	this->undoHistoryBook->Write(history);
}
//========== ���� ��� �߰� ==========
#endif

#if 0
//HistoryBook ���� �� ���ư����� Ȯ���ϱ�
#include "HistoryBook.h"
#include "Commands.h"
#include "CommandFactory.h"
#include "resource.h"

int main(int agrc, char* argv[]) {
	CommandFactory factory;
	Command* command;
	Command* ret;
	History* history;
	History* bookRet;
	History* other;
	HistoryBook* otherBook=0; //�Ҹ��
	Long top;
	Long bookTop;

	//1. ����å�� �����.
	HistoryBook *historyBook = new HistoryBook(10);
	//2. ���.
	command = factory.Make(IDC_WRITE_CHAR);
	//2.1. ���縦 �����.
	history = new History(10);
	//2.2. ���縦 �߻���Ű��.
	top = history->Happen(command);
	//2.3. ����å�� ����.
	bookTop = historyBook->Write(history);

	////////////Ȯ���ϴ�./////////
	otherBook = new HistoryBook(*historyBook);
	while (otherBook->GetLength() > 0) {
		other = otherBook->OpenAt();
		cout << "Top" << "\t" << "Type" << endl;

		while (other->GetLength() > 0) {
			ret = other->Reveal();
			cout << other->GetTop() << "\t" << ret->GetType() << endl;
			other->Forget();
		}

		otherBook->Erase();
	}
	cout << endl;
	//////////////////////////////

	//3. �ѱ� ���.
	command = factory.Make(IDC_IME_CHAR);
	//3.1. ����å���� ���縦 ����.
	bookRet = historyBook->OpenAt();
	//3.2. ���縦 �߻���Ű��.
	top = bookRet->Happen(command);
	
	////////////Ȯ���ϴ�./////////
	*otherBook = *historyBook;
	while (otherBook->GetLength() > 0) {
		other = otherBook->OpenAt();
		cout << "Top" << "\t" << "Type" << endl;

		while (other->GetLength() > 0) {
			ret = other->Reveal();
			cout << other->GetTop() << "\t" << ret->GetType() << endl;
			other->Forget();
		}

		otherBook->Erase();
	}
	cout << endl;
	//////////////////////////////

	
	if (otherBook != 0) {
		delete otherBook;
	}
	if (historyBook != 0) {
		delete historyBook;
	}

	return 0;

}
#endif