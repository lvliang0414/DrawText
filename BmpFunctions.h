#ifndef _YCTH_TEXT_FUNCTIONS_
#define _YCTH_TEXT_FUNCTIONS_

#include "FontFormat.h"

int CopyFontDataToBmp(char * bmpBuffer, SIZE bmpSize, char * fontBuffer, 
               int fontstartx, int fontstarty, RECT fontRect, int pageSplit = 0);

int WriteBmpFile(const char * filename, char * image, SIZE bmpSize);

void SetBmpBackgroundColor(char * buffer, int color, SIZE bmpSize);

#endif
