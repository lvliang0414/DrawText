#include "TextRender.h"
#include "easylogging++.h"
#include "utf8.h"
#include "BmpFunctions.h"

CTextRender::CTextRender()
{

}

CTextRender::~CTextRender()
{
    for (int i = 0; i < characters.size(); i++) {
        delete characters[i].buf;
    }
}

bool CTextRender::Init(string fontpath)
{
    int error = 0;
    error = FT_Init_FreeType(&library);
    
    if (error) {
        LOG(ERROR) << "FT Init FreeType error!" << error;
        return false;
    }

    error = FT_New_Face (library, fontpath.c_str(), 0, &face);
    if (error)
    {
        LOG(ERROR) << "FT face font(" << fontpath << ") error: " << error;
        return false;
    }

    return true;
}

int CTextRender::SetFontSize(int size)
{
    fontSize = size;
    if (FT_Set_Pixel_Sizes(face, 0, size))
    {
        LOG(ERROR) << "Set pixel size error";
        return -1;
    }

    int error = 0;
    FT_CharMap charmap = NULL;

    for (int i = 0; i < face->num_charmaps; i++)
    {
        charmap = face->charmaps[i];

        if (charmap->platform_id == 3)
        {
            error = FT_Set_Charmap(face, charmap);
            if (error)
                LOG(ERROR) << "FT_Set_Charmap error: " << error;
        }
    }
    return error;
}

int CTextRender::GetCharBuf(unsigned short ch, FontData & data)
{
    int error = 0;
    FT_UInt glyph_index = FT_Get_Char_Index(face, ch);
    error = FT_Load_Glyph(face, glyph_index, FT_LOAD_MONOCHROME);
    if (error)
    {
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP);
        if (error)
        {
            LOG(ERROR) << "FT_Load_Glyph error: " << error;
            return -1;
        }
    }

    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
    if (error)
    {
        LOG(ERROR) << "Render Glyph error: " << error;
        return -1;
    }

    FT_GlyphSlot slot = face->glyph;

    data.ch = ch;
    data.left =  slot->bitmap_left;
    data.top = slot->bitmap_top;
    data.width = slot->bitmap.width;
    data.height = slot->bitmap.rows;

    data.len = data.width * data.height * 3;
    if (data.len == 0 && slot->advance.x == 0) {
        LOG(ERROR) << "data: " << ch << " width: " << data.width << " height: " << data.height;
        return -1;
    }

    data.buf = new char [data.len];

    unsigned char * p = slot->bitmap.buffer;

    bool flag = false;

    //y move up
    if (fontAscender - data.top + data.height > fontSize) {
        data.top +=(fontAscender - data.top + data.height - fontSize);
    }

    if (flag) printf("%c\n", ch);
    for (int i = 0; i < data.height; i++)
    {
        for (int j = 0; j < data.width; j++)
        {
            int t = j / 8;
            int m = j % 8;
            unsigned char gray = p[i * slot->bitmap.pitch + t];
            if ((gray >> (7-m)) & 1)
            {
                if (flag) printf("x");
                data.buf[data.width * 3 * i + j * 3] = fontColor & 0xFF;
                data.buf[data.width * 3 * i + j * 3 + 1] = (fontColor >> 8) & 0xFF;
                data.buf[data.width * 3 * i + j * 3 + 2] = (fontColor >> 16) & 0xFF;
            }
            else
            {
                if (flag) printf(" ");
                data.buf[data.width * 3 * i + j * 3] = backColor & 0xFF;
                data.buf[data.width * 3 * i + j * 3 + 1] = (backColor >> 8) & 0xFF;
                data.buf[data.width * 3 * i + j * 3 + 2] = (backColor >> 16) & 0xFF;
            }
        }
        if (flag) printf("\n");
    }

    data.incrementx = slot->advance.x >> 6;
    data.incrementy = slot->advance.y >> 6;

    return 0;
}

char * CTextRender::GetStringBuf(string str)
{
    char * buffer = NULL;
    int len = str.length();

    string::iterator end_itr = utf8::find_invalid(str.begin(), str.end());
    if (end_itr != str.end())
    {
        LOG(ERROR) << "Invalid UTF-8 encoding!";
        return NULL;
    }

    int length = utf8::distance(str.begin(), end_itr);
    vector <unsigned short> utf16;
    utf8::utf8to16(str.begin(), end_itr, back_inserter(utf16));

    LOG(DEBUG) << str;

    FT_GlyphSlot slot = face->glyph;
    fontAscender = face->size->metrics.ascender >> 6 ;

    for (int i = 0; i < length; i++)
    {
        FontData fontdata;
        if (GetCharBuf(utf16[i], fontdata) < 0) {
            continue;
        }
        characters.push_back(fontdata);
    }

    LOG(DEBUG) << "characters size :" << characters.size();

    if (multiLines == 0) {
        return GetOneLineStringBuffer();
    }
    else {
        return GetMultiLineStringBuffer();
    }
}

char * CTextRender::GetOneLineStringBuffer()
{
    totalSize.width = 0;
    totalSize.height = bmpSize.height;

    for (int i = 0; i < characters.size(); i++) {
        totalSize.width += characters[i].incrementx;
    }
    int textWidth = totalSize.width;
    int t = totalSize.width / bmpSize.width;
    int m = totalSize.width % bmpSize.width;
    if (m || t == 0)
        t++;

    totalSize.width = t * bmpSize.width;

    char * buffer = new char [totalSize.width * totalSize.height * 3];
    memset(buffer, 0, totalSize.width * totalSize.height * 3);
    SetBmpBackgroundColor(buffer, backColor, totalSize);
    //LOG(DEBUG) << "W: " << totalSize.width << ", H: " << totalSize.height;

    int left = 0;
    int top = 0;
    for (int i = 0; i < characters.size(); i++) {
        FontData fontData = characters[i];
        L_RECT fontRect;

        fontRect.left = left + fontData.left;
        fontRect.top = fontAscender - fontData.top;
        fontRect.width = fontData.width;
        fontRect.height = fontData.height;

        int startx = 0;
        int starty = 0;

        //Offset Y
        switch (textAlign) {
            case 0:
            case 1:
            case 2:
                break;
            case 3:
            case 4:
            case 5:
                starty = (totalSize.height - fontSize) / 2;
                break;
            case 6:
            case 7:
            case 8:
                starty = totalSize.height - fontSize;
                break;
        }

        //Offset X
        if (t == 1) {
            switch (textAlign) {
                case 0:
                case 3:
                case 6:
                    break;
                case 1:
                case 4:
                case 7:
                    startx = (totalSize.width - textWidth) / 2;
                    break;
                case 2:
                case 5:
                case 8:
                    startx = totalSize.width - textWidth;
                    break;
            }
        }

        CopyFontDataToBmp(buffer, totalSize, fontData.buf, startx, starty, fontRect);
        left += fontData.incrementx;
    }
    return buffer;
}

char * CTextRender::GetMultiLineStringBuffer()
{
    totalSize.width = bmpSize.width;
    totalSize.height = 0;

    int lines = SplitTextToLines();
    printf("total %d lines\n", lines);

    int onePageLines = bmpSize.height / lineHeight;
    
    int t = lines / onePageLines;
    int m = lines % onePageLines;
    int pages = t;
    if (m)
        pages++;
    
    totalSize.height = pages * bmpSize.height;

    char * buffer = new char [totalSize.width * totalSize.height * 3];
    memset(buffer, 0, totalSize.width * totalSize.height * 3);
    SetBmpBackgroundColor(buffer, backColor, totalSize);

    for (int i = 0; i < t; i ++) 
    {
        for (int j = 0; j < onePageLines; j++) {
            vector<FontData> lineText = lineTexts[i * onePageLines + j].texts;
            int lineWidth = lineTexts[i * onePageLines + j].lineWidth;

            SetMultiLineDataToBmp(buffer, lineText, lineWidth, i, j, onePageLines);
        }
    }

    if (m)
    {
        for (int j = 0; j < m; j++)
        {
            vector<FontData> lineText = lineTexts[t*onePageLines + j].texts;
            int lineWidth = lineTexts[t*onePageLines + j].lineWidth;

            SetMultiLineDataToBmp(buffer, lineText, lineWidth, t, j, m);    
        }
    }

    return buffer;
}

int CTextRender::SplitTextToLines()
{
    lineTexts.clear();

    int lineWidth = 0;

    LineText lineText;
    for (int i = 0; i < characters.size(); i++) {
        lineWidth += characters[i].incrementx;

        if (lineWidth > bmpSize.width || characters[i].ch == '\n') {
            lineText.lineWidth = lineWidth - characters[i].incrementx;
            lineTexts.push_back(lineText);
            lineText.texts.clear();
            lineWidth = characters[i].incrementx;
        }
        if (characters[i].ch == '\n') {
            lineWidth = 0;
            lineText.lineWidth = 0;
        }
        else {
            lineText.lineWidth = lineWidth;
            lineText.texts.push_back(characters[i]);
        }
    }
    if (lineText.texts.size() > 0) {
        lineTexts.push_back(lineText);
    }

    return lineTexts.size();
}

void CTextRender::SetMultiLineDataToBmp(char * buffer, vector<FontData> & lineText,
                      int lineWidth, int page, int row, int curlines)
{
    int left = 0;
    for (int i = 0; i < lineText.size(); i++)
    {
        FontData font = lineText[i];
        L_RECT fontRect;
        fontRect.left = left + font.left;
        fontRect.top = fontAscender - font.top;
        fontRect.width = font.width;
        fontRect.height = font.height;

        int starty = 0;
        int startx = 0;
        int bmpHeight = bmpSize.height;
        int totalWidth = totalSize.width;
        int totalHeight = totalSize.height;

        switch (textAlign)
        {
        case 0:         //左上
            starty = page * bmpHeight + row * lineHeight;
            break;
        case 1:         //中上
            startx = (totalWidth - lineWidth) / 2;
            starty = page * bmpHeight + row * lineHeight;
            break;
        case 2:         //右上
            startx = totalWidth - lineWidth - 1;
            starty = page * bmpHeight + row * lineHeight;
            break;
        case 3:         //左中
            starty = page * bmpHeight + row * lineHeight + (bmpHeight - curlines * lineHeight) / 2;
            break;
        case 4:         //中
            startx = (totalWidth - lineWidth) / 2;
            starty = page * bmpHeight + row * lineHeight + (bmpHeight - curlines * lineHeight) / 2;
            break;
        case 5:         //右中
            startx = totalWidth - lineWidth - 1;
            starty = page * bmpHeight + row * lineHeight + (bmpHeight - curlines * lineHeight) / 2;
            break;
        case 6:         //左下
            starty = page * bmpHeight + row * lineHeight + (bmpHeight - curlines * lineHeight - 1);
            break;
        case 7:         //中下
            startx = (totalWidth - lineWidth) / 2;
            starty = page * bmpHeight + row * lineHeight + (bmpHeight - curlines * lineHeight - 1);
            break;
        case 8:         //右下
            startx = totalWidth - lineWidth - 1;
            starty = page * bmpHeight + row * lineHeight + (bmpHeight - curlines * lineHeight - 1);
            break;
        }
        if (page > 0 && row == 0) {
            CopyFontDataToBmp(buffer, totalSize, font.buf, startx, starty, fontRect, page * bmpHeight);
        }
        else {
            CopyFontDataToBmp(buffer, totalSize, font.buf, startx, starty, fontRect);
        }
        
        left += font.incrementx;
    }
}


int CTextRender::Draw(string str, const char * path)
{
    if (str == "")
    {
        char * emptybuf = new char [bmpSize.width * bmpSize.height * 3];
        memset(emptybuf, 0, bmpSize.width * bmpSize.height * 3);
        WriteBmpFile(path, emptybuf, bmpSize);
        delete emptybuf;
        LOG(DEBUG) << "Draw empty bmp! " << bmpSize.width << " x " << bmpSize.height;
    }
    else
    {
        char * buffer = GetStringBuf(str);

        if (buffer) {
            LOG(DEBUG) << "Draw bmp: " << totalSize.width << " x " << totalSize.height;
            WriteBmpFile(path, buffer, totalSize);
            delete buffer;
        }
        else
            return -1;
    }
    return 0;
}
