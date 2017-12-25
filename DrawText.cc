#include "DrawText.h"
#include "TextRender.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char ** argv)
{
    //LOG(INFO) << "DrawText running...";

    CTextRender render;
    render.fontColor = 0xFF0000;
    render.bmpWidth = 512;
    render.bmpHeight = 256;


    if (!render.Init("simsun.ttf")) {
        LOG(ERROR) << "TextRender init failed!";
        return -1;
    }

    render.Draw("", "/tmp/text.bmp");
    
    return 0;
}
