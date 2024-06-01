#include "matrix4.h"

// The code convention we adopt for the following implementations is "psychopathic".

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
    struct Matrix4 persp_no_scale;
    Matrix4_zero(&persp_no_scale);

    persp_no_scale.x00 = 1;
    persp_no_scale.x11 = aspect_ratio;
    persp_no_scale.x22 = zfar / (zfar - znear);
    persp_no_scale.x23 = -zfar * znear / (zfar - znear);
    persp_no_scale.x32 = 1;

    /*
    As we have it above, the coordinates map as follows:
    (note (x, y, z) are camera-local coordinates).

        x' -> x
        y' -> y * aspect_ratio 
            = y * (x_res / y_res)

              f(z - nw)
        z' -> ---------
                f - n

        w' -> z
    
    To account for fov, we need to scale (x, y) to fit within [-1, 1].

    tanf(fov / 2) is the length from one side of the view frustum to the xz or yz plane at z = 1.
    Dividing x and y by this value normalizes them to unit lengths.

    Also, to account for the aspect ratio (xres / yres), we scale the y coordinate.
    */

    struct Matrix4 scale;
    float inv_tan = 1 / tanf(RAD(fov) * 0.5);
    Matrix4_scale(inv_tan,  inv_tan, 1, &scale);

    // First apply the perpspective with no scale, then scale.
    Matrix4_mul(&scale, &persp_no_scale, out);
}

inline void Matrix4_look_at(struct Vector3 eye, struct Vector3 target, struct Vector3 up, struct Matrix4 *out) {
    struct Vector3 forward = Vector3_normalize(Vector3_sub(target, eye));
    struct Vector3 right   = Vector3_cross(up, forward); // Assuming up is unit length.
    struct Vector3 new_up  = Vector3_cross(forward, right);

    // Invert the camera to world transformation to get the world to camera transformation
    // called the view matrix. 
    // The inverse is:

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

void Matrix4_viewport(int window_width, int window_height, struct Matrix4 *out) {
    struct Matrix4 translate;
    Matrix4_translate(1, 1, 0, &translate);
    struct Matrix4 scale;
    Matrix4_scale(0.5 * window_width, 0.5 * window_height, 1, &scale);

    Matrix4_mul(&scale, &translate, out);
}

inline void Matrix4_print(const struct Matrix4 *a) {
    printf("[%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f]\n", 
        a->x00, a->x01, a->x02, a->x03,
        a->x10, a->x11, a->x12, a->x13,
        a->x20, a->x21, a->x22, a->x23,
        a->x30, a->x31, a->x32, a->x33
    );
}
