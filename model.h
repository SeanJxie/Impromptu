#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>
#include <string.h>

#include "vector3.h"
#include "matrix4.h"
#include "tri.h"
#include "util.h"

struct Model {
    struct Tri *mesh;
    int num_tris;

    // The mesh coordinates are local to the model.
    // (i.e., the center of the model is (0, 0, 0)).
    // The following is the model matrix, which
    // maps the mesh to world space.
    struct Matrix4 model_to_world;
};

// We move Model instances by heap pointer.

#define MAX_LINE_LEN_OBJ 128

struct Model *Model_create(struct Tri *mesh, int num_tris, float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz);
struct Model *Model_from_obj(const char *file_name, float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz);
void          Model_destroy(struct Model *m);

inline void   Model_translate(struct Model *m, float delta_x, float delta_y, float delta_z);
inline void   Model_rotate(struct Model *m, float delta_rx, float delta_ry, float delta_rz);
inline void   Model_scale(struct Model *m, float delta_sx, float delta_sy, float delta_sz);

// Presets.
struct Model *Model_unit_cube();
struct Model *Model_unit_triangle();
struct Model *Model_unit_square();

#endif