#include "matrix4.h"

// For next to no reason, the code convention we adopt for the following 
// implementations is: "What's an array? What's a loop?"

inline void Matrix4_add(const struct Matrix4 *a, const struct Matrix4 *b, struct Matrix4 *out) {
    out->x00 = a->x00 + b->x00;
    out->x01 = a->x01 + b->x01;
    out->x02 = a->x02 + b->x02;
    out->x03 = a->x03 + b->x03;
    out->x10 = a->x10 + b->x10;
    out->x11 = a->x11 + b->x11;
    out->x12 = a->x12 + b->x12;
    out->x13 = a->x13 + b->x13;
    out->x20 = a->x20 + b->x20;
    out->x21 = a->x21 + b->x21;
    out->x22 = a->x22 + b->x22;
    out->x23 = a->x23 + b->x23;
    out->x30 = a->x30 + b->x30;
    out->x31 = a->x31 + b->x31;
    out->x32 = a->x32 + b->x32;
    out->x33 = a->x33 + b->x33;
}

inline void Matrix4_sub(const struct Matrix4 *a, const struct Matrix4 *b, struct Matrix4 *out) {
    out->x00 = a->x00 - b->x00;
    out->x01 = a->x01 - b->x01;
    out->x02 = a->x02 - b->x02;
    out->x03 = a->x03 - b->x03;
    out->x10 = a->x10 - b->x10;
    out->x11 = a->x11 - b->x11;
    out->x12 = a->x12 - b->x12;
    out->x13 = a->x13 - b->x13;
    out->x20 = a->x20 - b->x20;
    out->x21 = a->x21 - b->x21;
    out->x22 = a->x22 - b->x22;
    out->x23 = a->x23 - b->x23;
    out->x30 = a->x30 - b->x30;
    out->x31 = a->x31 - b->x31;
    out->x32 = a->x32 - b->x32;
    out->x33 = a->x33 - b->x33;
}

inline void Matrix4_mul(const struct Matrix4 *a, const struct Matrix4 *b, struct Matrix4 *out) {
    float a00 = a->x00;
    float a01 = a->x01;
    float a02 = a->x02;
    float a03 = a->x03;
    float a10 = a->x10;
    float a11 = a->x11;
    float a12 = a->x12;
    float a13 = a->x13;
    float a20 = a->x20;
    float a21 = a->x21;
    float a22 = a->x22;
    float a23 = a->x23;
    float a30 = a->x30;
    float a31 = a->x31;
    float a32 = a->x32;
    float a33 = a->x33;
    float b00 = b->x00;
    float b01 = b->x01;
    float b02 = b->x02;
    float b03 = b->x03;
    float b10 = b->x10;
    float b11 = b->x11;
    float b12 = b->x12;
    float b13 = b->x13;
    float b20 = b->x20;
    float b21 = b->x21;
    float b22 = b->x22;
    float b23 = b->x23;
    float b30 = b->x30;
    float b31 = b->x31;
    float b32 = b->x32;
    float b33 = b->x33;
    out->x00 = a00 * b00 + a01 * b10 + a02 * b20 + a03 * b30;
    out->x01 = a00 * b01 + a01 * b11 + a02 * b21 + a03 * b31;
    out->x02 = a00 * b02 + a01 * b12 + a02 * b22 + a03 * b32;
    out->x03 = a00 * b03 + a01 * b13 + a02 * b23 + a03 * b33;
    out->x10 = a10 * b00 + a11 * b10 + a12 * b20 + a13 * b30;
    out->x11 = a10 * b01 + a11 * b11 + a12 * b21 + a13 * b31;
    out->x12 = a10 * b02 + a11 * b12 + a12 * b22 + a13 * b32;
    out->x13 = a10 * b03 + a11 * b13 + a12 * b23 + a13 * b33;
    out->x20 = a20 * b00 + a21 * b10 + a22 * b20 + a23 * b30;
    out->x21 = a20 * b01 + a21 * b11 + a22 * b21 + a23 * b31;
    out->x22 = a20 * b02 + a21 * b12 + a22 * b22 + a23 * b32;
    out->x23 = a20 * b03 + a21 * b13 + a22 * b23 + a23 * b33;
    out->x30 = a30 * b00 + a31 * b10 + a32 * b20 + a33 * b30;
    out->x31 = a30 * b01 + a31 * b11 + a32 * b21 + a33 * b31;
    out->x32 = a30 * b02 + a31 * b12 + a32 * b22 + a33 * b32;
    out->x33 = a30 * b03 + a31 * b13 + a32 * b23 + a33 * b33;
}

inline void Matrix4_smul(const struct Matrix4 *a, float s, struct Matrix4 *out) {
    out->x00 = a->x00 * s;
    out->x01 = a->x01 * s;
    out->x02 = a->x02 * s;
    out->x03 = a->x03 * s;
    out->x10 = a->x10 * s;
    out->x11 = a->x11 * s;
    out->x12 = a->x12 * s;
    out->x13 = a->x13 * s;
    out->x20 = a->x20 * s;
    out->x21 = a->x21 * s;
    out->x22 = a->x22 * s;
    out->x23 = a->x23 * s;
    out->x30 = a->x30 * s;
    out->x31 = a->x31 * s;
    out->x32 = a->x32 * s;
    out->x33 = a->x33 * s;
}

inline struct Vector3 Matrix4_vmul(const struct Matrix4 *a, struct Vector3 v) {
    return (struct Vector3) {
        v.x * a->x00 + v.y * a->x01 + v.z * a->x02 + v.w * a->x03,
        v.x * a->x10 + v.y * a->x11 + v.z * a->x12 + v.w * a->x13,
        v.x * a->x20 + v.y * a->x21 + v.z * a->x22 + v.w * a->x23,
        v.x * a->x30 + v.y * a->x31 + v.z * a->x32 + v.w * a->x33,
    };
}

inline void Matrix4_transpose(const struct Matrix4 *a, struct Matrix4 *out) {
    out->x00 = a->x00;
    out->x01 = a->x10;
    out->x02 = a->x20;
    out->x03 = a->x30;
    out->x10 = a->x01;
    out->x11 = a->x11;
    out->x12 = a->x21;
    out->x13 = a->x31;
    out->x20 = a->x02;
    out->x21 = a->x12;
    out->x22 = a->x22;
    out->x23 = a->x32;
    out->x30 = a->x03;
    out->x31 = a->x13;
    out->x32 = a->x23;
    out->x33 = a->x33;
}

inline float Matrix4_tr(const struct Matrix4 *a) {
    return a->x00 + a->x11 + a->x22 + a->x33;
}

float Matrix4_det(const struct Matrix4 *m) {
    // We compute, arbitrarily, along the 0th row.

    // For 3 x 3 matrix first minors.
    float a = m->x11;
    float b = m->x12;
    float c = m->x13;
    float d = m->x21;
    float e = m->x22;
    float f = m->x23;
    float g = m->x31;
    float h = m->x32;
    float i = m->x33;

    float det00 = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    a = m->x10;
    b = m->x12;
    c = m->x13;
    d = m->x20;
    e = m->x22;
    f = m->x23;
    g = m->x30;
    h = m->x32;
    i = m->x33;

    float det01 = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    a = m->x10;
    b = m->x11;
    c = m->x13;
    d = m->x20;
    e = m->x21;
    f = m->x23;
    g = m->x30;
    h = m->x31;
    i = m->x33;

    float det02 = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    a = m->x10;
    b = m->x11;
    c = m->x12;
    d = m->x20;
    e = m->x21;
    f = m->x22;
    g = m->x30;
    h = m->x31;
    i = m->x32;

    float det03 = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);

    return m->x00 * det00 - m->x01 * det01 + m->x02 * det02 - m->x03 * det03;
}               

inline void Matrix4_inverse(const struct Matrix4 *a, struct Matrix4 *out) {
    // https://en.wikipedia.org/wiki/Invertible_matrix#Analytic_solution
    // Cayley–Hamilton method.

    struct Matrix4 a2;
    struct Matrix4 a3;
    
    Matrix4_mul(a, a, &a2);
    Matrix4_mul(&a2, a, &a3);

    float tr_a      = Matrix4_tr(a);
    float tr_a2     = Matrix4_tr(&a2);
    float tr_a3     = Matrix4_tr(&a3);
    float inv_det_a = 1 / Matrix4_det(a);

    struct Matrix4 term1;
    Matrix4_identity(&term1);
    Matrix4_smul(&term1, (tr_a * tr_a * tr_a - 3 * tr_a * tr_a2 + 2 * tr_a3) / 6, &term1);

    struct Matrix4 term2;
    Matrix4_copy(a, &term2);
    Matrix4_smul(&term2, (tr_a * tr_a - tr_a2) / 2, &term2);

    struct Matrix4 term3;
    Matrix4_copy(&a2, &term3);
    Matrix4_smul(&term3, tr_a, &term3);

    struct Matrix4 term4;
    Matrix4_copy(&a3, &term4);

    Matrix4_copy(&term1, out);
    Matrix4_sub(out, &term2, out);
    Matrix4_add(out, &term3, out);
    Matrix4_sub(out, &term4, out);
    Matrix4_smul(out, inv_det_a, out);
}

inline void Matrix4_copy(const struct Matrix4 *a, struct Matrix4 *out) {
    out->x00 = a->x00;
    out->x01 = a->x01;
    out->x02 = a->x02;
    out->x03 = a->x03;
    out->x10 = a->x10;
    out->x11 = a->x11;
    out->x12 = a->x12;
    out->x13 = a->x13;
    out->x20 = a->x20;
    out->x21 = a->x21;
    out->x22 = a->x22;
    out->x23 = a->x23;
    out->x30 = a->x30;
    out->x31 = a->x31;
    out->x32 = a->x32;
    out->x33 = a->x33;
}

void Matrix4_zero(struct Matrix4 *out) {
    out->x00 = 0;
    out->x01 = 0;
    out->x02 = 0;
    out->x03 = 0;
    out->x10 = 0;
    out->x11 = 0;
    out->x12 = 0;
    out->x13 = 0;
    out->x20 = 0;
    out->x21 = 0;
    out->x22 = 0;
    out->x23 = 0;
    out->x30 = 0;
    out->x31 = 0;
    out->x32 = 0;
    out->x33 = 0;
}

void Matrix4_identity(struct Matrix4 *out) {
    Matrix4_zero(out);

    out->x00 = 1;
    out->x11 = 1;
    out->x22 = 1;
    out->x33 = 1;
}

void Matrix4_translate(float dx, float dy, float dz, struct Matrix4 *out) {
    Matrix4_zero(out);

    out->x00 = 1;
    out->x11 = 1;
    out->x22 = 1;
    out->x33 = 1;
    out->x03 = dx;
    out->x13 = dy;
    out->x23 = dz;
}

void Matrix4_rotate_x(float r, struct Matrix4 *out) {
    Matrix4_zero(out);

    float theta = RAD(r);

    out->x00 = 1;
    out->x11 = cosf(theta);
    out->x12 = -sinf(theta);
    out->x21 = sinf(theta);
    out->x22 = cosf(theta);
    out->x33 = 1;
}

void Matrix4_rotate_y(float r, struct Matrix4 *out) {
    Matrix4_zero(out);

    float theta = RAD(r);

    out->x00 = cosf(theta);
    out->x02 = sinf(theta);
    out->x11 = 1;
    out->x20 = -sinf(theta);
    out->x22 = cosf(theta);
    out->x33 = 1;
}

void Matrix4_rotate_z(float r, struct Matrix4 *out) {
    Matrix4_zero(out);

    float theta = RAD(r);

    out->x00 = cosf(theta);
    out->x01 = -sinf(theta);
    out->x10 = sinf(theta);
    out->x11 = cosf(theta);
    out->x22 = 1;
    out->x33 = 1;
}

void Matrix4_rotate_xyz(float rx, float ry, float rz, struct Matrix4 *out) {
    struct Matrix4 rotate_x;
    struct Matrix4 rotate_y;
    struct Matrix4 rotate_z;

    Matrix4_rotate_x(rx, &rotate_x);
    Matrix4_rotate_y(ry, &rotate_y);
    Matrix4_rotate_z(rz, &rotate_z);

    struct Matrix4 rotate_xy;
    Matrix4_mul(&rotate_y, &rotate_x, &rotate_xy);

    // Rotate xyz.
    Matrix4_mul(&rotate_z, &rotate_xy, out);
}


void Matrix4_scale(float sx, float sy, float sz, struct Matrix4 *out) {
    Matrix4_zero(out);

    out->x00 = sx;
    out->x11 = sy;
    out->x22 = sz;
    out->x33 = 1;
}

void Matrix4_perspective(float fov, float aspect_ratio, float znear, float zfar, struct Matrix4 *out) {
    Matrix4_zero(out);

    float inv_tan = 1 / tanf(RAD(fov) * 0.5);
    
    out->x00 = inv_tan;
    out->x11 = aspect_ratio * inv_tan;
    out->x22 = zfar / (zfar - znear);
    out->x23 = -zfar * znear / (zfar - znear);
    out->x32 = 1;
}

inline void Matrix4_look_at(struct Vector3 eye, struct Vector3 target, struct Vector3 up, struct Matrix4 *out) {
    struct Vector3 forward = Vector3_normalize(Vector3_sub(target, eye));
    struct Vector3 right   = Vector3_cross(up, forward); // Assuming up is unit length.
    struct Vector3 new_up  = Vector3_cross(forward, right);

    // Invert the camera to world transformation to get the world to camera transformation
    // called the view matrix. We hard-code the inverse instead of calling Matrix4_inverse.
    out->x00 = right.x;
    out->x01 = right.y;
    out->x02 = right.z;
    out->x03 = -Vector3_dot(eye, right);

    out->x10 = new_up.x;
    out->x11 = new_up.y;
    out->x12 = new_up.z;
    out->x13 = -Vector3_dot(eye, new_up);

    out->x20 = forward.x;
    out->x21 = forward.y;
    out->x22 = forward.z;
    out->x23 = -Vector3_dot(eye, forward);

    out->x30 = 0;
    out->x31 = 0;
    out->x32 = 0;
    out->x33 = 1;
}

inline void Matrix4_viewport(int window_width, int window_height, struct Matrix4 *out) {
    struct Matrix4 translate;
    Matrix4_translate(1, 1, 0, &translate);
    struct Matrix4 scale;
    Matrix4_scale(0.5 * window_width, 0.5 * window_height, 1, &scale);

    Matrix4_mul(&scale, &translate, out);
}

inline void Matrix4_print(const struct Matrix4 *a) {
    printf("[%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f]", 
        a->x00, a->x01, a->x02, a->x03,
        a->x10, a->x11, a->x12, a->x13,
        a->x20, a->x21, a->x22, a->x23,
        a->x30, a->x31, a->x32, a->x33
    );
}
