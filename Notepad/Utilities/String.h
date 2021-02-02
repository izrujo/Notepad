/******************************************************************
* ���� �̸� : String.h
* ��� : ���ڿ� ���̺귯�� ����
* �ۼ��� : ����â
* �ۼ����� : 2015�� 3�� 23�� 
*******************************************************************/

#ifndef STRING_H
#define STRING_H

#include <string>
// #define MAX 65536
#define MAX 1000

using namespace std;

typedef signed long int Long;

class String{
	public:
		String();
		String(const String& source);
		String(char character, Long nRepeat = 1);         // ��� : String('x',6); ��� : "xxxxxx"		
		String(string characters);                        // ��� : String(stirng("abc")); ��� : "abc"		
		String(char (*chracters));                           // ��� : String("abc"); ��� : "abc"
		                                                  // ��� : String(character); ��� : "abc"
		String(char (*chracters),Long length);               // ��� : String("abc", 4); ��� : "abc"
		                                                  // ��� : String(character,4); ��� : "abc"
		~String();
		
		void Clear();                                     // �޸� ���� ����
		void Empty();                                     // 0��°�� '\0' ���� ����    capacity ���� ����� length ���� ���
		Long Delete(Long index, Long count);              // �޸� �̵� ����          Capacity ���� ����� length ���� ���

		Long Insert(Long index, char character);          //capacity ���� ��� length ���� ���
		Long Insert(Long index, char (*characters));
		Long Insert(Long index, string characters);
		Long Insert(Long index, String *characters);
		
		Long Store(Long index, char character);           //capacity ���� ����� length ���� ���
		Long Store(Long index, char (*characters));
		Long Store(Long index, string characters);
		Long Store(Long index, String *characters);
		
		Long SetAt(Long index, char character);           //Modify ���� capacity ���� ����� length ���� ����� 		
		
		Long Find(char character);                        //Searches this string for the first match of a substring.
		                                                  //The zero-based index of the first character; -1 if the substring or character is not found.
		Long Find(char (*characters));

		Long Find(char character, Long index);            //The index of the character in the string to begin the search with, or 0 to start from the beginning. 
		                                                  //The character at nStart is excluded from the search if nStart is not equal to 0.
		Long Find(char (*characters), Long index);

		Long ReversedFind(char character);                //Searches this CString object for the last match of a substring
		Long ReversedFind(char(*characters), Long index);
		Long FindOneOf(char (*characters));               //Searches this string for the first character that matches any character contained

		Long Remove(char character);

		Long Replace(char oldCharacter, char newCharacter);
		Long Replace(char (*oldCharacter), char (*newCharacter));
		
		Long Compare(char (*characters));
		Long CompareNoCase(char (*characters));            //��ҹ��� ������� ��

		void TrimLeft();                                   // TrimLeft removes newline, space, and tab characters. 
		                                                   // remove a particular character from the beginning of a string.
		void TrimLeft(char character);
		void TrimLeft(char (*characters));
		void TrimRight();                                  // remove a particular character from the end of a string
		void TrimRight(char character);
		void TrimRight(char (*characters));

		void MakeUpper();
		void MakeLower();
		void MakeReverse();

		char GetAt(Long index);
		string GetDoubleByteAt(Long index);
		char* GetString();

		bool IsEmpty() const;
		bool IsHangle(Long index) const;

		String& operator=(const String& source);
		String& operator=(const char character);
		String& operator=(const char (*characters));
		String& operator=(const string characters);

		String& operator+(const String& characters);
		String& operator+(const char character);
		String& operator+(const char (*characters));
		String& operator+(const string characters);

		String& operator+=(const String& characters);
		String& operator+=(const char character);
		String& operator+=(const char (*characters));
		String& operator+=(const string characters);		

		// ÷�ڿ����� char �տ� &�� ����ؾ� ������ ����ȯ �Լ� ������ �ʹ� ���Ƽ� ������
		char operator[](Long index);
		
		operator char*();                                   //String�� char(*)�� ����ȯ �Լ�
		// operator char*(void); �̷��� �ϸ� �� ���� �ְڴٴ� ������ ������ test���غ� 20160622

		// String�� char(*)�� ����ȯ �Լ�
		// operator char* (); // 20160505 ����ȯ �����ڴ� ÷�ڿ�����(operator[]) �� �Բ� ���̸� �������� ������ c_str() �Լ��� ����
		// const char* c_str() const �׷��� ���� ���� ���� ���̼� ����
		// char* c_str() const;
		bool operator==(const char (*other));
		bool operator==(const string other);
		bool operator==(const String& other);

		bool operator!=(const char (*other));
		bool operator!=(const string other);
		bool operator!=(const String& other);

		bool operator<(const char (*other));
		bool operator<(const string other);
		bool operator<(const String& other);

		bool operator>(const char (*other));
		bool operator>(const string other);
		bool operator>(const String& other);

		bool operator<=(const char (*other));
		bool operator<=(const string other);
		bool operator<=(const String other);

		bool operator>=(const char (*other));
		bool operator>=(const string other);
		bool operator>=(const String other);

		Long GetCapacity() const;
		Long GetLength() const;
		Long GetCurrent() const; 

		// 2015-06-18
		Long First();
		Long Previous();
		Long Next();
		Long Last();

		// 2019-09-11 �ؽ�Ʈ�� �� ���� ���� ����
		Long CountLine();
		// 2019-11-28 ���� ���� �ؽ�Ʈ �� ���� �� ���� �ؽ�Ʈ ������ ���� ����
		Long CountLongestLine();
		// 2019-09-19 � ���ڿ��� ���� ��°���� �������� Ȯ���ϴ� ����
		Long FindUntilLength(char(*characters));
		// 2019-12-27 index���� �������
		Long RemoveFrom(Long index);
	private:
		char (*front);
		Long capacity;
		Long length;
		Long current;		
};

inline Long String::GetCapacity() const{
	return this->capacity;
}

inline Long String::GetLength() const{
	return this->length;
}

inline Long String::GetCurrent() const{
	return this->current;
}

#endif STRING_H
