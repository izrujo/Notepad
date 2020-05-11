#ifndef _GLYPHFACTORY_H
#define _GLYPHFACTORY_H

typedef signed long int Long;
class NotepadForm;
class Glyph;
class GlyphFactory {
public:
	GlyphFactory(NotepadForm *notepadForm=0);
	GlyphFactory(const GlyphFactory& source);
	~GlyphFactory();
	Glyph* Make(const char(*content));
	Glyph* Make(Glyph *line, Long cilentWidth);
	//오버로딩 - DummyLine : 이렇게 하기보다 원본 Make에 조건식을 줘서 비어있는 DummyLine을 반환하는게 맞음. 수정요망.
private:
	NotepadForm *notepadForm;
};

#endif //_GLYPHFACTORY_H