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
    render.textAlign = 1;
    render.multiLines = 1;

    string textstr = "";
    string fontPath = "pix.ttf";
    int fontSize = 12;
    int lineSpace = 0;

    string fontColor="#FFFFFF";
    string backColor="#000000";

    auto cli = (
        required("-i") &  value("Text path", textPath),
        required("-o") &value("Bitmap path", bmpPath),
        required("-font") & value("Font path", fontPath),
        option("-size") & value("16", fontSize),
        option("-multiline") & value("0", render.multiLines),
        option("-align") & value("4", render.textAlign),
        option("-linespace") & value("0", lineSpace),
        option("-width") & value("512", render.bmpSize.width),
        option("-height") & value("64", render.bmpSize.height),
        option("-color") & opt_value("#ff0000", fontColor),
        option("-backcolor") & opt_value("#ff0000", backColor)
    );
    LOG(DEBUG) << "fontcolor: " << fontColor << "  bgColor: " << backColor;
    if (parse(argc, argv, cli)) {
        if (access(textPath.c_str(), F_OK) != 0) {
            LOG(ERROR) << "Failed: Text file (" << textPath << ") is not exist";
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

    textstr = UnescapeHtml(textstr);

    if (!render.Init(fontPath)) {
        LOG(ERROR) << "Failed: TextRender init failed!";
        return -1;
    }

    if (render.multiLines == 0) {
        std::replace(textstr.begin(), textstr.end(), '\n', ' ');
    }

    if (render.SetFontSize(fontSize) == 0) {
        if (render.Draw(textstr, bmpPath.c_str()) == 0) {
            LOG(DEBUG) << "OK";
        }
    }
    
    
    return 0;
}
