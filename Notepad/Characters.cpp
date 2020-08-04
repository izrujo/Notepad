#include "Characters.h"
#include "Visitor.h"

Character::Character() {
	this->isSelected = false;
}

Character::Character(const Character& source) {
	this->isSelected = source.isSelected;
}

Character::~Character() {

}

Character& Character::operator=(const Character& source) {
	this->isSelected = source.isSelected;

	return *this;
}

void Character::Select(bool isSelected) {
	this->isSelected = isSelected;
}

void Character::Accept(Visitor* visitor) {
	visitor->Visit(this);
}

//SingleByteCharacter
SingleByteCharacter::SingleByteCharacter()
	: Character() {
	this->content = '\0';
}

SingleByteCharacter::SingleByteCharacter(char content)
	: Character() {
	this->content = content;
}

SingleByteCharacter::SingleByteCharacter(const SingleByteCharacter& source)
	: Character(source) {
	this->content = source.content;
}

SingleByteCharacter::~SingleByteCharacter() {

}

SingleByteCharacter& SingleByteCharacter::operator=(const SingleByteCharacter& source) {
	Character::operator=(source);
	this->content = source.content;

	return *this;
}

bool SingleByteCharacter::IsEqual(const SingleByteCharacter& other) {
	bool ret = false;
	if (this->content == other.content) {
		ret = true;
	}
	return ret;
}

bool SingleByteCharacter::IsNotEqual(const SingleByteCharacter& other) {
	bool ret = false;
	if (this->content != other.content) {
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator==(const SingleByteCharacter& other) {
	bool ret = false;
	if (this->content == other.content) {
		ret = true;
	}

	return ret;
}

bool SingleByteCharacter::operator !=(const SingleByteCharacter& other) {
	bool ret = false;
	if (this->content != other.content) {
		ret = true;
	}

	return ret;
}

Glyph* SingleByteCharacter::Clone() {
	return new SingleByteCharacter(*this);
}

string SingleByteCharacter::GetContent() {
	string content;
	if (this->content != '\t') {
		content = this->content;
	}
	else {
		content = "        ";
	}

	return content;
}

//DoubleByteCharacter
DoubleByteCharacter::DoubleByteCharacter()
	: Character() {
	this->content[0] = '\0';
	this->content[1] = '\0';
}

DoubleByteCharacter::DoubleByteCharacter(const char(*content))
	: Character() {
	this->content[0] = content[0];
	this->content[1] = content[1];
}

DoubleByteCharacter::DoubleByteCharacter(const DoubleByteCharacter& source)
	: Character(source) {
	this->content[0] = source.content[0];
	this->content[1] = source.content[1];
}

DoubleByteCharacter::~DoubleByteCharacter() {

}

DoubleByteCharacter& DoubleByteCharacter::operator=(const DoubleByteCharacter& source) {
	Character::operator=(source);
	this->content[0] = source.content[0];
	this->content[1] = source.content[1];

	return *this;
}

bool DoubleByteCharacter::IsEqual(const DoubleByteCharacter& other) {
	bool ret = false;
	if (this->content[0] == other.content[0] &&
		this->content[1] == other.content[1]) {
		ret = true;
	}
	return true;
}

bool DoubleByteCharacter::IsNotEqual(const DoubleByteCharacter& other) {
	bool ret = false;
	if (this->content[0] != other.content[0] ||
		this->content[1] != other.content[1]) {
		ret = true;
	}
	return true;
}

bool DoubleByteCharacter::operator==(const DoubleByteCharacter& other) {
	bool ret = false;
	if (this->content[0] == other.content[0] &&
		this->content[1] == other.content[1]) {
		ret = true;
	}
	return true;
}

bool DoubleByteCharacter::operator !=(const DoubleByteCharacter& other) {
	bool ret = false;
	if (this->content[0] != other.content[0] ||
		this->content[1] != other.content[1]) {
		ret = true;
	}
	return true;
}

Glyph* DoubleByteCharacter::Clone() {
	return new DoubleByteCharacter(*this);
}

string DoubleByteCharacter::GetContent() {
	char content[3];
	content[0] = this->content[0];
	content[1] = this->content[1];
	content[2] = '\0';

	return content;
}