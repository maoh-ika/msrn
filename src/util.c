#include <gb/gb.h>
#include "util.h"

void shift(unsigned char* out, const unsigned char* in, const int len, const int offset) {
    for (int i = 0; i < len; ++i) {
        out[i] = in[i] + offset;
    }
}

void toScreenXY(int* x, int* y) {
    *x += 8;
    *y += 16;
}

void hideWindow(void) {
    while(STAT_REG & 3); // wait for HBlank
    HIDE_WIN;
}

void showWindow(void) {
    SHOW_WIN;
}