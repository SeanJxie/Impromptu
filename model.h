#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>

#include "vector3.h"
#include "matrix4.h"

struct Tri {
    struct Vector3 p1, p2, p3;
    unsigned char r, g, b;
};

// We move Tri instances by value.

struct Mesh {
    struct Tri *tris;
    int         num_tris;
}; 

// We move Mesh instances by value.

struct Model {
    struct Mesh mesh;

    // The mesh coordinates are local to the model.
    // (i.e., the center of the model is (0, 0, 0)).
    // The following is the model matrix, which
    // maps the mesh to world space.
    struct Matrix4 model_to_world;
};

// We move Model instances by heap pointer.

struct Model *Model_create(float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz, struct Mesh mesh);
void          Model_destroy(struct Model *m);

inline void   Model_translate(struct Model *m, float delta_x, float delta_y, float delta_z);
inline void   Model_rotate(struct Model *m, float delta_rx, float delta_ry, float delta_rz);
inline void   Model_scale(struct Model *m, float delta_sx, float delta_sy, float delta_sz);

// Presets.
struct Model *Model_unit_cube();

#endif