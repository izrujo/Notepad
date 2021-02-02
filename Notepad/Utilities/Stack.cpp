#if 0

#include "Stack.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	Long object;
	Long top;
	Long ret;
	bool isEmpty;

	//1. 스택을 만들다.
	Stack<Long> stack(2);
	//2. 100을 넣다.
	object = 100;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;
	//3. 200을 넣다.
	object = 200;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;
	//4. 50을 넣다.
	object = 50;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;

	//복사 생성자
	Stack<Long> other(stack);
	cout << "other top : " << other.GetTop() << " other obj : " << other.Top() << endl;
	object = 40;
	top = other.Push(object);
	cout << "other top : " << top << " other obj : " << other.Top() << endl;

	//5. 꺼내다.
	ret = stack.Pop();
	if (ret == -1) {
		cout << "지웠다." << endl;
	}
	cout << "top : " << stack.GetTop() << " obj : " << stack.Top() << endl;
	//비어있는지 확인한다.
	isEmpty = stack.IsEmpty();
	if (isEmpty == true) {
		cout << "비어있다." << endl;
	}
	else {
		cout << "비어있지 않다." << endl;
	}
	//6. 비우다.
	stack.Empty();
	if (stack.GetTop() == 0) {
		cout << "모두 지웠다." << endl;
	}
	//비어있는지 확인한다.
	isEmpty = stack.IsEmpty();
	if (isEmpty == true) {
		cout << "비어있다." << endl;
	}
	else {
		cout << "비어있지 않다." << endl;
	}
	//7. 20을 넣다.
	object = 20;
	top = stack.Push(object);
	cout << "top : " << top << " obj : " << stack.Top() << endl;

	//치환 연산자
	stack = other;
	cout << "top : " << stack.GetTop() << " obj : " << stack.Top() << endl;

	//8. 스택을 지우다.

	return 0;
}

//========== 실행 취소 추가 ==========
string type = command->GetType();
BOOL condition1 = FALSE;
BOOL condition2 = FALSE;
History* history;
if (undoHistoryBook->GetLength() > 0) {
	history = this->undoHistoryBook->LookAt();
	Command* previousCommand = history->Top();

	//현재 커맨드가 Write이고 엔터키 입력이 아니거나 현재 커맨드가 ImeChar이고
	//직전 커맨드가 Write이거나 ImeChar이면
	//현재 히스토리에 커맨드를 넣는다.
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
//실행 취소 역사책이 비었거나
//비었을 때에도 현재 커맨드가 Write, ImeChar, Delete, Backspace, Paste, Cut, DeleteSelection 중에 하나여야 하고
//비어있지 않으면
//현재 커맨드가 Write나 ImeChar가 아니고
//Write나 ImeChar라 하더라도 Write인 경우 엔터키 입력이며 또 엔터키 입력이라 하더라도
//직전 커맨드가 Write나 ImeChar가 아니면
//새 히스토리를 만들어 쓴다.
if ((type == "Wirte" || type == "ImeChar" || type == "Delete" || type == "Backspace"
	|| type == "Paste" || type == "Cut" || type == "DeleteSelection")
	&& (condition1 == FALSE || condition2 == FALSE)) {
	history = new History;
	history->Push(command->Clone());
	this->undoHistoryBook->Write(history);
}
//========== 실행 취소 추가 ==========
#endif

#if 0
//HistoryBook 스택 잘 돌아가는지 확인하기
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
	HistoryBook* otherBook=0; //소멸요
	Long top;
	Long bookTop;

	//1. 역사책을 만들다.
	HistoryBook *historyBook = new HistoryBook(10);
	//2. 썼다.
	command = factory.Make(IDC_WRITE_CHAR);
	//2.1. 역사를 만들다.
	history = new History(10);
	//2.2. 역사를 발생시키다.
	top = history->Happen(command);
	//2.3. 역사책에 쓰다.
	bookTop = historyBook->Write(history);

	////////////확인하다./////////
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

	//3. 한글 썼다.
	command = factory.Make(IDC_IME_CHAR);
	//3.1. 역사책에서 역사를 보다.
	bookRet = historyBook->OpenAt();
	//3.2. 역사를 발생시키다.
	top = bookRet->Happen(command);
	
	////////////확인하다./////////
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