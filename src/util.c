#include "util.h"

void shift(unsigned char* out, const unsigned char* in, const int len, const int offset) {
    for (int i = 0; i < len; ++i) {
        out[i] = in[i] + offset;
    }
}
