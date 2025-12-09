#ifndef COLORS_H
#define COLORS_H

#define NOMINMAX
#include <windows.h>

// Color codes
// 0 = Black       8 = Dark Gray
// 1 = Dark Blue   9 = Blue
// 2 = Dark Green  10 = Green
// 3 = Dark Cyan   11 = Cyan
// 4 = Dark Red    12 = Red
// 5 = Dark Magenta 13 = Magenta
// 6 = Dark Yellow 14 = Yellow
// 7 = Light Gray  15 = White

inline void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#endif
