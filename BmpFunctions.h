#ifndef _YCTH_TEXT_FUNCTIONS_
#define _YCTH_TEXT_FUNCTIONS_

#include "FontFormat.h"

int CopyFontDataToBmp(char * bmpBuffer, L_SIZE bmpSize, char * fontBuffer, 
               int fontstartx, int fontstarty, L_RECT fontRect, int pageSplit = 0);

int WriteBmpFile(const char * filename, char * image, L_SIZE bmpSize);

void SetBmpBackgroundColor(char * buffer, int color, L_SIZE bmpSize);

#endif
