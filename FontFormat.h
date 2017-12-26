#ifndef _DRAW_TEXT_FONT_FORMAT_H_
#define _DRAW_TEXT_FONT_FORMAT_H_

#include <string>
#include <vector>
using namespace std;

typedef struct {
    int width;
    int height;
}SIZE;

typedef struct {
    int left;
    int top;
    int width;
    int height;
}RECT;

typedef struct
{
    char rgbBlue;
    char rgbGreen;
    char rgbRed;
    char rgbReserved;
}__attribute__((packed)) RGBQUAD;

//40 Bytes
typedef struct
{
    unsigned int   biSize;
    int            biWidth;
    int            biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
}__attribute__((packed)) BITMAPINFOHEADER;

typedef struct
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[1];
}__attribute__((packed)) BITMAPINFO;

//14 Bytes
typedef struct
{
    unsigned short  bfType;
    unsigned int   bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned int   bfOffBits;
} __attribute__((packed)) BITMAPFILEHEADER;


typedef struct {
    int left;
    int top;
    int width;
    int height;
    char * buf;
    int len;
    int incrementx;
    int incrementy;
    string text;
} FontData;

typedef struct {
    int lineWidth;
    vector<FontData> texts;
} LineText;



#endif
