#ifndef _DUMMYMANAGER_H
#define _DUMMYMANAGER_H

typedef signed long int Long;
class Glyph;
class CharacterMetrics;

class DummyManager {
public:
	DummyManager(Glyph* note = 0, CharacterMetrics* characterMetrics = 0, Long width = 0);
	DummyManager(const DummyManager& source);
	~DummyManager();
	DummyManager& operator=(const DummyManager& source);

	Long Unfold(Long row);
	void Unfold(Long* start, Long* end); // overloading
	Long Fold(Long unfoldedRow);

	Long CountDistance(Long row, Long column);
	void CountIndex(Long distance, Long* row, Long* column);
private:
	Glyph* note;
	CharacterMetrics* characterMetrics;
	Long width;
};


#endif //_DUMMYMANAGER_H