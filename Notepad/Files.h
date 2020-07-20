//Files.h
/**************************************************
Classification : (will classify by UML class stero type)
Function : Directly connect with files processed.
MVC : ?
Author : Chan Hyung Kim
Written Date : 2019.05
Recently Updated : 2019.05
**************************************************/

#ifndef _FILES_H
#define _FILES_H

#include <string>
#include <iostream>

using namespace std;
typedef signed long int Long;

class File {
public:
	File(string name = "");
	File(const File& source);
	virtual ~File() = 0;
	File& operator=(const File& source);
	
	//Save() is implemented only for ansi because of fail to implement event of file save dialog customized.
	//it should be abstract.
	virtual Long Save(string content);
	virtual string Load() = 0;

	virtual string GetType() = 0;

	virtual string& GetName() const;
protected:
	string name;
};

inline string& File::GetName() const {
	return const_cast<string&>(this->name);
}

//Text File of ANSI
class AnsiFile : public File {
public:
	AnsiFile(string name = "");
	AnsiFile(const AnsiFile& source);
	virtual ~AnsiFile();
	AnsiFile& operator=(const AnsiFile& source);

	//virtual Long Save(string content);
	virtual string Load();

	virtual string GetType();
};

//Text File of UTF-16 BE
class Utf16BigEndianFile : public File {
public:
	Utf16BigEndianFile(string name = "");
	Utf16BigEndianFile(const Utf16BigEndianFile& source);
	virtual ~Utf16BigEndianFile();
	Utf16BigEndianFile& operator=(const Utf16BigEndianFile& source);

	//virtual Long Save(string content);
	virtual string Load(); //fail to implement..

	virtual string GetType();
};

//Text File of UTF-16 LE
class Utf16LittleEndianFile : public File {
public:
	Utf16LittleEndianFile(string name = "");
	Utf16LittleEndianFile(const Utf16LittleEndianFile& source);
	virtual ~Utf16LittleEndianFile();
	Utf16LittleEndianFile& operator=(const Utf16LittleEndianFile& source);

	//virtual Long Save(string content);
	virtual string Load();

	virtual string GetType();
};

//Text File of UTF-8 BOM
class Utf8ByteOrderMarkFile : public File {
public:
	Utf8ByteOrderMarkFile(string name = "");
	Utf8ByteOrderMarkFile(const Utf8ByteOrderMarkFile& source);
	virtual ~Utf8ByteOrderMarkFile();
	Utf8ByteOrderMarkFile& operator=(const Utf8ByteOrderMarkFile& source);

	//virtual Long Save(string content);
	virtual string Load();

	virtual string GetType();
};

//Text File of UTF-8
class Utf8File : public File {
public:
	Utf8File(string name = "");
	Utf8File(const Utf8File& source);
	virtual ~Utf8File();
	Utf8File& operator=(const Utf8File& source);

	//virtual Long Save(string content);
	virtual string Load();

	virtual string GetType();
};

#endif //_FILES_H