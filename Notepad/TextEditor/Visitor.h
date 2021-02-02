#ifndef _VISITOR_H
#define _VISITOR_H

class Note;
class Line;
class Character;
class Book;

class Visitor {
public:
	Visitor();
	virtual ~Visitor() = 0;

	virtual void Visit(Note* note) = 0;
	virtual void Visit(Line* line) = 0;
	virtual void Visit(Character* character) = 0;
	virtual void Visit(Book* book) = 0; //Not implement
};

#endif //_VISITOR_H