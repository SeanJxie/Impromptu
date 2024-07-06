#ifndef OBJ_PARSE
#define OBJ_PARSE

#include <stdlib.h>
#include <string.h>

#include "vector3.h"
#include "model.h"

// Some useful size constants.
#define MAX_LL  512  // Max string length of a line in OBJ and MTL files.
#define MAX_LSS 128  // Max string length of a delimited substring.
#define MAX_NSS 16   // Max number of delimited substrings.
#define MAX_LM  64   // Max string length of a material name in OBJ and MTL files.
#define MAX_NM  1024 // Max number of defined materials.

struct Obj_mtl {
    char name[MAX_LM];
    struct Vector3 kd;
};

inline void split_space(const char *line, char out[MAX_NSS][MAX_LSS], int *out_n);
inline void split_slash(const char *line, char out[MAX_NSS][MAX_LSS], int *out_n);

inline struct Tri *parse_obj(const char *file_name, int *out_n); // Outputs a heap allocated mesh.
inline void parse_mtl(const char *file_name, struct Obj_mtl *out, int *out_n);

#endif