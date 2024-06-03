#ifndef UTIL_H
#define UTIL_H

#include <string.h>

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

inline void slice(const char *s, unsigned int start, unsigned int end, char *out);

#endif