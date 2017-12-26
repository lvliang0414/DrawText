#ifndef _DRAW_TEXT_RENDER_H_
#define _DRAW_TEXT_RENDER_H_

#include "FontFormat.h"
#include "ft2build.h"


#include FT_FREETYPE_H

class CTextRender {
public:
    CTextRender();
    ~CTextRender();

    bool Init(string fontpath);
    int SetFontSize(int fontsize);

    vector<FontData> characters;
    vector<LineText> lineTexts;

    SIZE bmpSize;

    int fontColor;
    int backColor;
    int multiLines;
    int textAlign;
    int lineHeight;
    int fontAscender;

    int Draw(string str, const char * path);
private:
    FT_Library library;
    FT_Face face;

    int fontSize;
    
    SIZE totalSize;

    int GetCharBuf(unsigned short ch, FontData & data);
    int SplitTextToLines();

    char * GetOneLineStringBuffer();
    char * GetMultiLineStringBuffer();
    char * GetStringBuf(string str);

    void SetMultiLineDataToBmp(char * buffer, vector<FontData> & lineText,
                      int lineWidth, int page, int row, int curlines);
};

#endif
