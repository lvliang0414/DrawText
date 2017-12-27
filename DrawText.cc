#include "DrawText.h"
#include "TextRender.h"
#include "easylogging++.h"
#include "clipp.h"
#include "Functions.h"

INITIALIZE_EASYLOGGINGPP

using namespace clipp;

int main(int argc, char ** argv)
{
    string textPath;
    string bmpPath;

    CTextRender render;
    render.fontColor = 0xFF0000;
    render.bmpSize.width = 512;
    render.bmpSize.height = 16;
    render.textAlign = 6;
    render.multiLines = 1;

    string textstr = "";
    string fontPath = "pix.ttf";
    int fontSize = 12;
    int lineSpace = 0;

    string fontColor="#FFFFFF";
    string backColor="#000000";

    auto cli = (
        value("Text file", textPath),
        value("Bitmap file", bmpPath),
        required("-font") & value("path of ttf", fontPath),
        option("-size") & value("16", fontSize),
        option("-multiline") & value("0", render.multiLines),
        option("-align") & value("4", render.textAlign),
        option("-linespace") & value("0", lineSpace),
        option("-width") & value("512", render.bmpSize.width),
        option("-height") & value("64", render.bmpSize.height),
        option("-color") & value("#ff0000", fontColor),
        option("-backcolor") & value("#000000", backColor)
    );

    if (parse(argc, argv, cli)) {
        if (access(textPath.c_str(), F_OK) != 0) {
            LOG(ERROR) << "Text file (" << textPath << ") is not exist";
            return -1;
        }
    }
    else {
        cout << make_man_page(cli, "drawtext");
        return -1;
    }

    render.fontColor = GetColorFromString(fontColor);
    render.backColor = GetColorFromString(backColor);
    render.lineHeight = fontSize + lineSpace;
    textstr = ReadFile(textPath.c_str());

    if (!render.Init(fontPath)) {
        LOG(ERROR) << "TextRender init failed!";
        return -1;
    }

    render.SetFontSize(fontSize);

    render.Draw(textstr, bmpPath.c_str());
    
    return 0;
}
