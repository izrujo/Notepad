#ifndef _SELECTION_H
#define _SELECTION_H

typedef signed long int Long;

class Selection {
public:
	Selection(Long start = -1, Long end = -1);
	Selection(const Selection& source);
	~Selection();
	Selection& operator=(const Selection& source);

	Long GetStart() const;
	Long GetEnd() const;

private:
	Long start;
	Long end;
};

inline Long Selection::GetStart() const {
	return this->start;
}

inline Long Selection::GetEnd() const {
	return this->end;
}

#endif //_SELECTION_H