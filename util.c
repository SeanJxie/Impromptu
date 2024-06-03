#include "util.h"

inline void slice(const char *s, unsigned int start, unsigned int end, char *out) {
    memcpy(out, s + start, end - start);
}