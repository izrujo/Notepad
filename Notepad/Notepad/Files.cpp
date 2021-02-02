#include "Files.h"
#include <fstream>
#include <locale.h>
#include <windows.h>

#pragma warning (disable:4996)

File::File(string name)
	: name(name) {

}

File::File(const File &source)
	: name(source.name) {

}

File::~File() {

}

File& File::operator=(const File& source) {
	this->name = source.name;

	return *this;
}

Long File::Save(string content) {
	fstream fs;

	fs.open(this->name.c_str(), ios_base::out);
	if (fs.is_open()) {
		fs << content;
		fs.close();
	}

	return content.length();
}

////////////지우지 말기 Big Endian 시도한 코드 있음
#if 0
string File::Load(string mode) {
	string content = "";

	FILE *file;

	char(*line) = new char[99999];
	wchar_t(*wLine) = new wchar_t[99999];
	string str;
#if 0
	wchar_t wbytes;
	wchar_t wtemp;
	char temp[2];
	char bytes[2] = { 0, };
	Long i = 0;
#endif
	setlocale(LC_ALL, "ko-KR");
	if (fopen_s(&file, this->name.c_str(), mode.c_str()) == 0) {
		//swap the byte order of all data.
		//한 줄씩 해보기. wLine은 무조건 한줄.
		if (this->encodingType == ENCODINGTYPE::UTF_16_BE) {
			//i dunno how to do...
#if 0
			while (!feof(file)) {
				//wbytes = fgetwc(file);
				//fread(&bytes, 1, 2, file);
				temp[0] = fgetc(file);
				temp[1] = fgetc(file);
				bytes[0] = temp[1];
				bytes[1] = temp[0];
				//wbytes = (wbytes << 8) | ((wbytes >> 8) & 0xFF);
			}
			int length = wcslen(wLine);
			for (i = 0; i < length; i += 2) {
				wtemp = wLine[i];
				wLine[i] = wLine[i + 1];
				wLine[i + 1] = wtemp;
				wstring wstr(wLine[i], wLine[i + 1]);
			}
			WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);
			//fseek(file, 0, SEEK_SET);
#endif
		}

		while (!feof(file)) {
			if (this->encodingType == ENCODINGTYPE::UTF_16_LE ||
				this->encodingType == ENCODINGTYPE::UTF_8_BOM ||
				this->encodingType == ENCODINGTYPE::UTF_8) {
				fgetws(wLine, 99998, file);
				WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);
			}
			else if (this->encodingType == ENCODINGTYPE::ANSI) {
				fgets(line, 99998, file);
			}

			str = string(line);
			if (str.at(str.length() - 1) == '\n') {
				str = str.substr(0, str.length() - 1);
			}
			content += str + "\r\n";
		}
		fclose(file);
	}
	content = content.substr(0, content.length() - 2);

	if (line != 0) {
		delete[] line;
	}
	if (wLine != 0) {
		delete[] wLine;
	}

	return content;
}
#endif
/////////////////////

//Text File of ANSI
AnsiFile::AnsiFile(string name)
	: File(name) {

}

AnsiFile::AnsiFile(const AnsiFile &source)
	: File(source) {

}

AnsiFile::~AnsiFile() {

}

AnsiFile &AnsiFile::operator=(const AnsiFile &source) {
	File::operator=(source);

	return *this;
}

string AnsiFile::Load() {
	string content = "";
	FILE *file;
	char(*line) = new char[99999];
	string str;

	if (fopen_s(&file, this->name.c_str(), "rt") == 0) {
		while (!feof(file)) {
			fgets(line, 99998, file);

			str = string(line);
			if (str.at(str.length() - 1) == '\n') {
				str = str.substr(0, str.length() - 1);
			}
			content += str + "\r\n";
		}
		fclose(file);
	}
	content = content.substr(0, content.length() - 2);

	if (line != 0) {
		delete[] line;
	}

	return content;
}

string AnsiFile::GetType() {
	return "ANSI";
}

//Text File of UTF-16 BE

Utf16BigEndianFile::Utf16BigEndianFile(string name)
	: File(name) {

}

Utf16BigEndianFile::Utf16BigEndianFile(const Utf16BigEndianFile &source)
	: File(source) {

}

Utf16BigEndianFile::~Utf16BigEndianFile() {

}

Utf16BigEndianFile &Utf16BigEndianFile::operator=(const Utf16BigEndianFile &source) {
	File::operator=(source);

	return *this;
}

string Utf16BigEndianFile::Load() {
	//fail to implement. try later.

	return "";
}

string Utf16BigEndianFile::GetType() {
	return "UTF-16 BE";
}

//Text File of UTF-16 LE

Utf16LittleEndianFile::Utf16LittleEndianFile(string name)
	: File(name) {

}

Utf16LittleEndianFile::Utf16LittleEndianFile(const Utf16LittleEndianFile &source)
	: File(source) {

}

Utf16LittleEndianFile::~Utf16LittleEndianFile() {

}

Utf16LittleEndianFile &Utf16LittleEndianFile::operator=(const Utf16LittleEndianFile &source) {
	File::operator=(source);

	return *this;
}

string Utf16LittleEndianFile::Load() {
	string content = "";
	FILE *file;
	char(*line) = new char[99999];
	wchar_t(*wLine) = new wchar_t[99999];
	string str;

	setlocale(LC_ALL, "ko-KR");
	if (fopen_s(&file, this->name.c_str(), "rt, ccs=UTF-16LE") == 0) {
		while (!feof(file)) {
			fgetws(wLine, 99998, file);
			WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);

			str = string(line);
			if (str.at(str.length() - 1) == '\n') {
				str = str.substr(0, str.length() - 1);
			}
			content += str + "\r\n";
		}
		fclose(file);
	}
	content = content.substr(0, content.length() - 2);

	if (line != 0) {
		delete[] line;
	}
	if (wLine != 0) {
		delete[] wLine;
	}

	return content;
}

string Utf16LittleEndianFile::GetType() {
	return "UTF-16 LE";
}

//Text File of UTF-8 BOM
Utf8ByteOrderMarkFile::Utf8ByteOrderMarkFile(string name)
	: File(name) {

}

Utf8ByteOrderMarkFile::Utf8ByteOrderMarkFile(const Utf8ByteOrderMarkFile &source)
	: File(source) {

}

Utf8ByteOrderMarkFile::~Utf8ByteOrderMarkFile() {

}

Utf8ByteOrderMarkFile &Utf8ByteOrderMarkFile::operator=(const Utf8ByteOrderMarkFile &source) {
	File::operator=(source);

	return *this;
}

string Utf8ByteOrderMarkFile::Load() {
	string content = "";
	FILE *file;
	char(*line) = new char[99999];
	wchar_t(*wLine) = new wchar_t[99999];
	string str;

	setlocale(LC_ALL, "ko-KR");
	if (fopen_s(&file, this->name.c_str(), "rt, ccs=UTF-8") == 0) {
		while (!feof(file)) {
			fgetws(wLine, 99998, file);
			WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);

			str = string(line);
			if (str.at(str.length() - 1) == '\n') {
				str = str.substr(0, str.length() - 1);
			}
			content += str + "\r\n";
		}
		fclose(file);
	}
	content = content.substr(0, content.length() - 2);

	if (line != 0) {
		delete[] line;
	}
	if (wLine != 0) {
		delete[] wLine;
	}

	return content;
}

string Utf8ByteOrderMarkFile::GetType() {
	return "UTF-8 BOM";
}

//Text File of UTF-8
Utf8File::Utf8File(string name)
	: File(name) {

}

Utf8File::Utf8File(const Utf8File &source)
	: File(source) {

}

Utf8File::~Utf8File() {

}

Utf8File &Utf8File::operator=(const Utf8File &source) {
	File::operator=(source);

	return *this;
}

string Utf8File::Load() {
	string content = "";
	FILE *file;
	char(*line) = new char[99999];
	wchar_t(*wLine) = new wchar_t[99999];
	string str;

	setlocale(LC_ALL, "ko-KR");
	if (fopen_s(&file, this->name.c_str(), "rt, ccs=UTF-8") == 0) {
		while (!feof(file)) {
			fgetws(wLine, 99998, file);
			WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);

			str = string(line);
			if (str.at(str.length() - 1) == '\n') {
				str = str.substr(0, str.length() - 1);
			}
			content += str + "\r\n";
		}
		fclose(file);
	}
	content = content.substr(0, content.length() - 2);

	if (line != 0) {
		delete[] line;
	}
	if (wLine != 0) {
		delete[] wLine;
	}

	return content;
}

string Utf8File::GetType() {
	return "UTF-8";
}