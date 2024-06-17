#ifndef OBJ_PARSE
#define OBJ_PARSE

#include <stdlib.h>
#include <string.h>

#include "vector3.h"

#define MAX_LEN_LINE     512 // Length of line in obj and mtl files.
#define MAX_LEN_SUBSTR   128 // Length of whitespace-delimited substrings in a line.
#define MAX_NUM_SUBSTR   16  // Number of substrings.
#define MAX_LEN_MTL_ID   64  // Length of material ID in obj and mtl files.
#define MAX_NUM_MTL      1024

struct Obj_mtl {
    char id[MAX_LEN_MTL_ID];
    struct Vector3 kd;
};

inline void split(const char *line, char out[MAX_NUM_SUBSTR][MAX_LEN_SUBSTR], int *out_n);
inline void parse_mtl(const char *file_name, struct Obj_mtl *out, int *out_n);

#endif