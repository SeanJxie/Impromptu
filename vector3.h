#ifndef VECTOR3_H
#define VECTOR3_H

#include <stdio.h>
#include <math.h>

#include "matrix4.h"

struct Vector3 {
    float x, y, z, w;
}; 

struct Matrix4; // Forward declaration.

// We move Vector3 instances by value.

inline struct Vector3 Vector3_create_point(float x, float y, float z);
inline struct Vector3 Vector3_create_direction(float x, float y, float z);

// Operations.
inline struct Vector3 Vector3_add(struct Vector3 v1, struct Vector3 v2);
inline struct Vector3 Vector3_sub(struct Vector3 v1, struct Vector3 v2);
inline struct Vector3 Vector3_smul(struct Vector3 v, float s);
inline struct Vector3 Vector3_sdiv(struct Vector3 v, float s);
inline struct Vector3 Vector3_cross(struct Vector3 v1, struct Vector3 v2);
inline float          Vector3_dot(struct Vector3 v1, struct Vector3 v2);
inline float          Vector3_norm(struct Vector3 v);
inline float          Vector3_norm_squared(struct Vector3 v);
inline struct Vector3 Vector3_normalize(struct Vector3 v);
inline struct Vector3 Vector3_mul_Matrix4(struct Vector3 v, const struct Matrix4 *m);

// Utility.
inline void           Vector3_print(struct Vector3 v);

#endif