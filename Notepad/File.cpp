#include "File.h"
#include <fstream>

File::File()
	: name("") {
	this->mode = ios_base::in | ios_base::out;
}

File::File(string name, ios_base::openmode mode)
	: name(name) {
	this->mode = mode;
}

File::File(const File& source)
	: name(source.name) {
	this->mode = source.mode;
}

File::~File() {
}

Long File::Save(string content) {
	fstream fs;

	fs.open(this->name.c_str(), this->mode);
	if (fs.is_open()) {
		fs << content;
		fs.close();
	}

	return content.length();
}

string File::Load() {
	string content = "";
	string line;
	fstream fs;

	fs.open(this->name, this->mode);
	if (fs.is_open()) {
		while (!fs.eof()) {
			getline(fs, line);
			content += line + "\r\n";
		}
		content = content.substr(0, content.length() - 2);
	}

	return content;
}

File& File::operator =(const File& source) {
	this->name = source.name;
	this->mode = source.mode;

	return *this;
}