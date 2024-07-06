#ifndef VERTEX_H
#define VERTEX_H

#include "vector3.h"

struct Vertex {
    struct Vector3 pos;

    // Vertex attributes

    // Normal.
    //struct Vector3 normal;

    // Texture coordiantes.
    //float tex_u, tex_v;

    // Colour (optional use).
    struct Vector3 col;
};

#endif

