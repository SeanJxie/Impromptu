#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix4.h"

struct Transform {
    struct Matrix4 *m;
    struct Matrix4 *m_tinv;
};

void Transform_create(struct Matrix4 *mat, struct Transform *out);
inline struct Vector3 Transform_apply(const struct Transform *t, struct Vector3 v);
inline struct Vector3 Transform_apply_normal(const struct Transform *t, struct Vector3 n);

#endif