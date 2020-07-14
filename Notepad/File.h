#ifndef _FILE_H
#define _FILE_H

#include <string>
#include <iostream>

using namespace std;
typedef signed long int Long;

enum ENCODINGTYPE { ANSI = 0, UTF_8 = 1, UTF_8_BOM = 2, UTF_16_LE = 3, UTF_16_BE = 4 };

class File {
public:
	File();
	File(string name);
	File(const File& source);
	~File();
	File& operator=(const File& source);
	
	Long Save(string content);
	string Load();

	string& GetName() const;
private:
	string name;
	ENCODINGTYPE encodingType;
};

inline string& File::GetName() const {
	return const_cast<string&>(this->name);
}

#endif //_FILE_H