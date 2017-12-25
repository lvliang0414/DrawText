#include "BmpFunctions.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include "easylogging++.h"
#include "FontFormat.h"
#include <stdio.h>

int CopyFontDataToBmp(char * bmpBuffer, int bmpWidth, int bmpHeight, char * fontBuffer, 
        int fontstartx, int fontstarty, int fontLeft, int fontTop, int fontWidth, int fontHeight)
{
    int height = fontHeight;
    if (fontHeight + fontTop > bmpHeight)
    {
        if (fontHeight < bmpHeight)
            fontTop = (bmpHeight - fontHeight);
        else
        {
            height = bmpHeight;
            fontTop = 0;
        }
    }
    
	for (int i = 0; i < height; i++)
    {
        memcpy(bmpBuffer + (bmpHeight - 1 - i - fontTop - fontstarty) * bmpWidth * 3 + fontstartx * 3 + fontLeft * 3, 
                fontBuffer + i * fontWidth * 3, 
                fontWidth * 3 );
    }
}


int WriteBmpFile(const char * filename, char * image, int width, int height)
{
    FILE * f;
    BITMAPINFO  bmpInfo;
    BITMAPFILEHEADER    bmpFileHeader;

    f = fopen(filename, "wb");
    if ( f == NULL) {
        LOG(ERROR) << "open " << filename << " failed!";
        return -1;
    }

    memset(&bmpInfo, 0, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = 0;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biWidth = width;
    bmpInfo.bmiHeader.biHeight = height;
    bmpInfo.bmiHeader.biSizeImage=width*height*3;
    bmpFileHeader.bfReserved1=0;
    bmpFileHeader.bfReserved2=0;
    bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
    bmpFileHeader.bfType=0x4D42;
    //bmpFileHeader.bfType='MB';
    bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    
    fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), f);
    fwrite(&bmpInfo.bmiHeader, 1, sizeof(BITMAPINFOHEADER), f);
    fwrite(image, 1, width*height*3, f);

    fclose(f);

    return 0;
}
