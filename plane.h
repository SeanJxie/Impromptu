#ifndef PLANE_H
#define PLANE_H

#include "vector3.h"
#include "util.h"

struct Plane {
    struct Vector3 p0; // Point on plane.
    struct Vector3 n;  // Plane normal.
};

// We move Plane instances by value.

inline int Plane_intersect_line(struct Plane p, struct Vector3 start, struct Vector3 end, struct Vector3 *intersect);

#endif