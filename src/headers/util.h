#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

void shift(unsigned char* out, const unsigned char* in, const int len, const int offset);
void toScreenXY(int* x, int* y);
void hideWindow(void);
void showWindow(void);

#endif