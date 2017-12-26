#include "Functions.h"
#include <string.h>

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