#ifndef _FILE_H
#define _FILE_H

#include <string>
#include <iostream>

using namespace std;
typedef signed long int Long;

class File {
public:
	File();
	File(string name, ios_base::openmode mode);
	File(const File& source);
	~File();
	
	Long Save(string content);
	string Load();

	File& operator =(const File& source);
	string& GetName() const;
private:
	string name;
	ios_base::openmode mode;
};
inline string& File::GetName() const {
	return const_cast<string&>(this->name);
}

#endif //_FILE_H