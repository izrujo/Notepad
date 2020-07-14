#include "File.h"
#include <fstream>
#include <locale.h>
#include <windows.h>

#pragma warning (disable:4996)

File::File()
	: name("") {
	this->encodingType = ANSI;
}

File::File(string name)
	: name(name) {
	this->encodingType = ANSI;
}

File::File(const File& source)
	: name(source.name) {
	this->encodingType = source.encodingType;
}

File::~File() {

}

File& File::operator=(const File& source) {
	this->name = source.name;
	this->encodingType = source.encodingType;

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

string File::Load() {
	string content = "";
	string mode = "";
	int encode;

	FILE *file;
	fopen_s(&file, this->name.c_str(), "rb");
	if (file != NULL) {
		//선두 1바이트만 읽는다.
		encode = fgetc(file);
		
		//선두 바이트 값에 따라 형식을 분류한다.
		if (encode == 254) {
			this->encodingType = UTF_16_BE;
			mode = "rb";
		}
		else if (encode == 255) {
			this->encodingType = UTF_16_LE;
			mode = "rt, ccs=UTF-16LE";
		}
		else if (encode == 239) {
			this->encodingType = UTF_8_BOM;
			mode = "rt, ccs=UTF-8";
		}
		else if (encode > 200) {
			this->encodingType = UTF_8;
			mode = "rt, ccs=UTF-8";
		}
		else {
			this->encodingType = ANSI;
			mode = "rb";
		}
		fclose(file);
	}

	//형식에 따라 파일에서 데이터를 읽어온다.
	char (*line) = new char[99999];
	wchar_t (*wLine) = new wchar_t[99999];
	string str;

	setlocale(LC_ALL, "ko-KR");
	if (fopen_s(&file, this->name.c_str(), mode.c_str()) == 0) {
		//swap the byte order of all data.
		while (!feof(file)) {
			switch (this->encodingType) {
			case UTF_16_BE:
				//i dunno how to do...
				break;

			case UTF_16_LE:
				fgetws(wLine, 99998, file);
				WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);
				break;

			case UTF_8_BOM:
				fgetws(wLine, 99998, file);
				WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);
				break;

			case UTF_8:
				fgetws(wLine, 99998, file);
				WideCharToMultiByte(CP_ACP, 0, wLine, -1, line, 99999, NULL, NULL);
				break;

			case ANSI:
				fgets(line, 99998, file);
				break;

			default:
				break;
			}

			str = string(line);
			if (str.at(str.length() - 1) == '\n') {
				str = str.substr(0, str.length() - 2);
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