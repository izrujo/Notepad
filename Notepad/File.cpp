#include "File.h"
#include <fstream>

File::File()
	: name("") {

}

File::File(string name)
	: name(name) {

}

File::File(const File& source)
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

string File::Load() {
	string content = "";
	string line;
	fstream fs;

	fs.open(this->name, ios_base::in);
	if (fs.is_open()) {
		while (!fs.eof()) {
			getline(fs, line);
			content += line + "\r\n";
		}
		content = content.substr(0, content.length() - 2);
	}

	return content;
}