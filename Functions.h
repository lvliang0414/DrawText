#ifndef _DRAW_TEXT_FUNCTIONS_H_
#define _DRAW_TEXT_FUNCTIONS_H_

#include <unistd.h>
#include <string>
using namespace std;

string ReadFile(const char * path);

int GetColorFromString(string colorstr);
string UnescapeHtml(string str);

void response(int retcode, string message);

#endif
