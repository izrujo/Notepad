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
		//���� 1����Ʈ�� �д´�.
		encode = fgetc(file);
		/* <����>
		1. UTF-8 �Ǵ� Ansi�� ���ڵ��� �ؽ�Ʈ ������ ��� �ƽ�Ű �ڵ�ǥ ������ �ִ� ���ڰ� ������ �� ó���� �ִٸ�
		�� �ƽ�Ű �ڵ�ǥ ���� 10���� ��ȣ�� ��ȯ�ȴ�. ���� �⺻ �ƽ�Ű �ڵ�ǥ�� �ִ� ���� 127�� �ƴ� ���� ã�´�.
		2. ���� �ڵ�� UTF-8 �̶� �ѱ��� ���� �����̶�� Ansi�� ���� ���̴�. ��Ƽ����Ʈ�� �ƴϸ� ������� ���д�.
		3. ������ ���ڵ� ���ĵ��� �ƽ�Ű ���ڰ� �� ó���� �ֵ� ���� ������ ���� ��ȯ�Ѵ�.
		*/
		while (encode <= 127 && !feof(file)) {
			encode = fgetc(file);
		}
		fclose(file);
		//���� ����Ʈ ���� ���� ������ �з��Ѵ�.
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