#include "FileFactory.h"
#include "Files.h"

FileFactory::FileFactory() {

}

FileFactory::FileFactory(const FileFactory& source) {

}

FileFactory::~FileFactory() {

}

FileFactory& FileFactory::operator=(const FileFactory& source) {
	return *this;
}

File* FileFactory::MakeSaveFile(string filePathName, string encodingType) {
	File* fileObject = 0;

	if (encodingType == "UTF-16 BE") {
		fileObject = new Utf16BigEndianFile(filePathName);
	}
	else if (encodingType == "UTF-16 LE") {
		fileObject = new Utf16LittleEndianFile(filePathName);
	}
	else if (encodingType == "UTF-8 BOM") {
		fileObject = new Utf8ByteOrderMarkFile(filePathName);
	}
	else if (encodingType == "UTF-8") {
		fileObject = new Utf8File(filePathName);
	}
	else if (encodingType == "ANSI") {
		fileObject = new AnsiFile(filePathName);
	}
	
	return fileObject;
}

File* FileFactory::MakeOpenFile(string filePathName) {
	File* fileObject = 0;
	FILE* file;
	int encode = 0;

	if (fopen_s(&file, filePathName.c_str(), "rb") == 0) {
		//선두 1바이트만 읽는다.
		encode = fgetc(file);
		/* <참고>
		1. UTF-8 또는 Ansi로 인코딩된 텍스트 파일의 경우 아스키 코드표 범위에 있는 문자가 파일의 맨 처음에 있다면
		그 아스키 코드표 상의 10진수 번호가 반환된다. 따라서 기본 아스키 코드표의 최대 값인 127이 아닌 값을 찾는다.
		2. 현재 코드는 UTF-8 이라도 한글이 없는 파일이라면 Ansi로 열릴 것이다. 멀티바이트가 아니면 상관없어 놔둔다.
		3. 나머지 인코딩 형식들은 아스키 문자가 맨 처음에 있든 없든 일정한 값을 반환한다.
		*/
		while (encode <= 127 && !feof(file)) {
			encode = fgetc(file);
		}
		fclose(file);
		//선두 바이트 값에 따라 형식을 분류한다.
		if (encode == 254) {
			fileObject = new Utf16BigEndianFile(filePathName);
		}
		else if (encode == 255) {
			fileObject = new Utf16LittleEndianFile(filePathName);
		}
		else if (encode == 239) {
			fileObject = new Utf8ByteOrderMarkFile(filePathName);
		}
		else if (encode > 200) {
			fileObject = new Utf8File(filePathName);
		}
		else {
			fileObject = new AnsiFile(filePathName);
		}
	}

	return fileObject;
}