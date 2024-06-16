#include "transform.h"

void Transform_create(struct Matrix4 *matrix, struct Transform *out) {
    out->m = matrix;
    Matrix4_inverse(out->m, out->m_tinv);
    Matrix4_transpose(out->m_tinv, out->m_tinv);
}

inline struct Vector3 Transform_apply(const struct Transform *t, struct Vector3 v) {
    return Matrix4_vmul(t->m, v);
}

inline struct Vector3 Transform_apply_normal(const struct Transform *t, struct Vector3 n) {
    // For normals, we apply transformations differently. Specifically, we use
    // the transpose of the inverse of the regular matrix.
    return Matrix4_vmul(t->m_tinv, n);
}
