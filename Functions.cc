#include "Functions.h"
#include <string.h>
#include <algorithm>

string ReadFile(const char * path)
{
    FILE * fp = NULL;
    fp = fopen(path, "r");
    if (fp == NULL)
        return "";

    string str = "";
    int size = 1024;
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int len = fread(buf, 1, size, fp);
    while (len > 0) {
        str += string(buf);

        memset(buf, 0, sizeof(buf));
        len = fread(buf, 1, size, fp);
    }
    fclose(fp);
    return str;
}

int GetColorFromString(string colorstr)
{
    if (colorstr.length() == 7 && colorstr.at(0) == '#') {
        colorstr = colorstr.substr(1);
    }
    else if (colorstr.length() == 9) {
        colorstr = colorstr.substr(2, 6);
    }
    else if (colorstr.length() != 6) {
        return 0; 
    }

    char * offset;
    int color = strtol(colorstr.c_str(), &offset, 16);
    //printf("color: #%s == 0x%x\n", colorstr.c_str(), color);
    return color;
}

string StringReplace(string str, string src, string target)
{
    string retstr = str;
    size_t pos = string::npos;
    do {
        pos = retstr.find(src);
        if (pos != string::npos) {
            retstr = retstr.replace(pos, src.length(), target);
        }
    }while (pos != string::npos);

    return retstr;
}

string UnescapeHtml(string str)
{
    string retstr = StringReplace(str, "&ldquo;", "\"");
    retstr = StringReplace(retstr, "&rdquo;", "\"");
    retstr = StringReplace(retstr, "&quot;", "\"");
    retstr = StringReplace(retstr, "&amp;", "&");
    retstr = StringReplace(retstr, "&lt;", "<");
    retstr = StringReplace(retstr, "&gt;", ">");

    return retstr;
}

void response(int retcode, string message)
{
    printf("{\"retcode\":%d, \"retmsg\":\"%s\"}\n", retcode, message.c_str());
}
