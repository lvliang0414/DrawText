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

    int bmpWidth;
    int bmpHeight;
    int fontColor;

    int Draw(string str, const char * path);
private:
    FT_Library library;
    FT_Face face;

    int fontSize;
    int totalWidth;
    int totalHeight;
    

    int GetCharBuf(unsigned short ch, FontData & data);
    char * GetStringBuf(string str);
};

#endif
