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
	int encode;

	if (fopen_s(&file, filePathName.c_str(), "rb") == 0) {
		//선두 1바이트만 읽는다.
		encode = fgetc(file);
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