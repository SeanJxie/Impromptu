#ifndef MATRIX4_H
#define MATRIX4_H

#include <stdio.h>
#include <math.h>

#include "vector3.h"

struct Matrix4 {
    // Notation is row-column,
    // Disregarding struct padding, 
    // the matrix is stored in row-major order.
    float x00, x01, x02, x03,
          x10, x11, x12, x13,
          x20, x21, x22, x23,
          x30, x31, x32, x33;
};

struct Vector3; // Forward declaration.

// We move Matrix4 instances with stack pointers.

// Operations.
inline void           Matrix4_add(const struct Matrix4 *a, const struct Matrix4 *b, struct Matrix4 *out);
inline void           Matrix4_sub(const struct Matrix4 *a, const struct Matrix4 *b, struct Matrix4 *out);
inline void           Matrix4_mul(const struct Matrix4 *a, const struct Matrix4 *b, struct Matrix4 *out);
inline void           Matrix4_smul(const struct Matrix4 *a, float s, struct Matrix4 *out);
inline struct Vector3 Matrix4_vmul(const struct Matrix4 *a, struct Vector3 v);
inline void           Matrix4_transpose(const struct Matrix4 *a, struct Matrix4 *out);
inline float          Matrix4_tr(const struct Matrix4 *a);
inline float          Matrix4_det(const struct Matrix4 *a);
inline void           Matrix4_inverse(const struct Matrix4 *a, struct Matrix4 *out);
inline void           Matrix4_copy(const struct Matrix4 *a, struct Matrix4 *out);

// Transformations.

#define RAD(x) (x * 3.14159265358979323846264338327950288 / 180)

void        Matrix4_zero(struct Matrix4 *out);
void        Matrix4_identity(struct Matrix4 *out);

void        Matrix4_translate(float dx, float dy, float dz, struct Matrix4 *out);
void        Matrix4_rotate_x(float r, struct Matrix4 *out);
void        Matrix4_rotate_y(float r, struct Matrix4 *out);
void        Matrix4_rotate_z(float r, struct Matrix4 *out);
void        Matrix4_rotate_xyz(float rx, float ry, float rz, struct Matrix4 *out);
void        Matrix4_scale(float sx, float sy, float sz, struct Matrix4 *out);

void        Matrix4_perspective(float fov, float aspect_ratio, float znear, float zfar, struct Matrix4 *out);
inline void Matrix4_look_at(struct Vector3 eye, struct Vector3 target, struct Vector3 up, struct Matrix4 *out);
inline void Matrix4_viewport(int window_width, int window_height, struct Matrix4 *out);

// Utility.
inline void Matrix4_print(const struct Matrix4 *a);

#endif