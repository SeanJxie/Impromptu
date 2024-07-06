#ifndef TRI_H
#define TRI_H

#include <stdlib.h>

#include "vertex.h"

struct Tri {
    struct Vertex v0, v1, v2;
};

//struct Tri Tri_create(struct Vertex *v0, struct Vertex *v1, struct Vertex *v2);

#endif