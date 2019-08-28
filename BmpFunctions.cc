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

extern int showDebug;

int CopyFontDataToBmp(char * bmpBuffer, L_SIZE bmpSize, char * fontBuffer, 
        int fontstartx, int fontstarty, L_RECT fontRect, int pageSplit)
{
    int height = fontRect.height;

    if (fontRect.height + fontRect.top > bmpSize.height)
    {
        if (fontRect.height < bmpSize.height)
            fontRect.top = (bmpSize.height - fontRect.height);
        else
        {
            height = bmpSize.height;
            fontRect.top = 0;
        }
    }

    //printf("width:%d, height:%d\n", bmpSize.width, height);
    //printf("start: %d %d\n", fontstartx, fontstarty);
    //printf("font: %d %d\n", fontRect.top, fontRect.left);
    /*if (pageSplit) {
        for (int i = 0; i < bmpSize.width / 2; i++) {
            int pos = (bmpSize.height - 1 - pageSplit) * bmpSize.width * 3 + i * 6;
            bmpBuffer[pos] = 0xFF;
            bmpBuffer[pos+1] = 0xFF;
            bmpBuffer[pos+2] = 0xFF;
        }
    }*/

	for (int i = 0; i < height; i++) {
        memcpy(bmpBuffer + (bmpSize.height - 1 - i - fontRect.top - fontstarty) * bmpSize.width * 3 + fontstartx * 3 + fontRect.left * 3, 
                fontBuffer + i * fontRect.width * 3, 
                fontRect.width * 3 );
    }
}


int WriteBmpFile(const char * filename, char * image, L_SIZE bmpSize)
{
    FILE * f;
    BITMAPINFO  bmpInfo;
    BITMAPFILEHEADER    bmpFileHeader;

    f = fopen(filename, "wb");
    if ( f == NULL) {
        if (showDebug) LOG(ERROR) << "Failed: open " << filename << " failed!";
        return -1;
    }

    memset(&bmpInfo, 0, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = 0;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biWidth = bmpSize.width;
    bmpInfo.bmiHeader.biHeight = bmpSize.height;
    bmpInfo.bmiHeader.biSizeImage=bmpSize.width*bmpSize.height*3;
    bmpFileHeader.bfReserved1=0;
    bmpFileHeader.bfReserved2=0;
    bmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmpInfo.bmiHeader.biSizeImage;
    bmpFileHeader.bfType=0x4D42;
    //bmpFileHeader.bfType='MB';
    bmpFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    
    fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), f);
    fwrite(&bmpInfo.bmiHeader, 1, sizeof(BITMAPINFOHEADER), f);
    fwrite(image, 1, bmpSize.width*bmpSize.height*3, f);

    fclose(f);

    return 0;
}

void SetBmpBackgroundColor(char * buffer, int color, L_SIZE bmpSize)
{
    for (int i = 0; i < bmpSize.height; i++)
    {
        for (int j = 0; j < bmpSize.width; j++) {
            buffer[bmpSize.width * 3 * i + j * 3] = color & 0xFF;
            buffer[bmpSize.width * 3 * i + j * 3 + 1] = (color >> 8) & 0xFF;
            buffer[bmpSize.width * 3 * i + j * 3 + 2] = (color >> 16) & 0xFF;
        }
    }
}
