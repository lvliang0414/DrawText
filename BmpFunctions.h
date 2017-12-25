#ifndef _YCTH_TEXT_FUNCTIONS_
#define _YCTH_TEXT_FUNCTIONS_


int CopyFontDataToBmp(char * bmpBuffer, int bmpWidth, int bmpHeight, char * fontBuffer, 
               int fontstartx, int fontstarty, int fontLeft, int fontTop, int fontWidth, int fontHeight);

int WriteBmpFile(const char * filename, char * image, int width, int height);

#endif
