#include "TextRender.h"
#include "easylogging++.h"
#include "utf8.h"
#include "BmpFunctions.h"

CTextRender::CTextRender()
{

}

CTextRender::~CTextRender()
{

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
            LOG(ERROR) << "FT_Set_Charmap error";
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

    data.left =  slot->bitmap_left;
    data.top = slot->bitmap_top;
    data.width = slot->bitmap.width;
    data.height = slot->bitmap.rows;

    data.len = data.width * data.height * 3;
    data.buf = new char [data.len];

    unsigned char * p = slot->bitmap.buffer;

    for (int i = 0; i < data.height; i++)
    {
        for (int j = 0; j < data.width; j++)
        {
            int t = j / 8;
            int m = j % 8;
            unsigned char gray = p[i * slot->bitmap.pitch + t];
            if ((gray >> (7-m)) & 1)
            {
                data.buf[data.width * 3 * i + j * 3] = fontColor & 0xFF;
                data.buf[data.width * 3 * i + j * 3 + 1] = (fontColor >> 8) & 0xFF;
                data.buf[data.width * 3 * i + j * 3 + 2] = (fontColor >> 16) & 0xFF;
            }
            else
            {
                memset(data.buf + data.width * 3 * i + j * 3, 0, 3);
            }
        }
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
    //fontAscender = face->size->metrics.ascender >> 6 ;

    for (int i = 0; i < length; i++)
    {
        FontData fontdata;
        if (GetCharBuf(utf16[i], fontdata) < 0) {
            continue;
        }
        characters.push_back(fontdata);
    }

    return NULL;
}

int CTextRender::Draw(string str, const char * path)
{
    if (str == "")
    {
        char * emptybuf = new char [bmpWidth * bmpHeight * 3];
        memset(emptybuf, 0, bmpWidth * bmpHeight * 3);
        WriteBmpFile(path, emptybuf, bmpWidth, bmpHeight);
        delete emptybuf;
        LOG(DEBUG) << "Draw empty bmp! " << bmpWidth << bmpHeight;
    }
    else
    {
        char * buffer = GetStringBuf(str);

        if (buffer) {
            WriteBmpFile(path, buffer, totalWidth, totalHeight);
            delete buffer;
        }
        else
            return -1;
    }
    return 0;
}
