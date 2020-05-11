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
	//�����ε� - DummyLine : �̷��� �ϱ⺸�� ���� Make�� ���ǽ��� �༭ ����ִ� DummyLine�� ��ȯ�ϴ°� ����. �������.
private:
	NotepadForm *notepadForm;
};

#endif //_GLYPHFACTORY_H