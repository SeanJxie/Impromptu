#include "vector3.h"

inline struct Vector3 Vector3_create_point(float x, float y, float z) {
    // A point has w = 1.
    return (struct Vector3){x, y, z, 1}; 
}

inline struct Vector3 Vector3_create_direction(float x, float y, float z) {
    // A direction (represented as a point at infinity) has w = 0.
    return (struct Vector3){x, y, z, 0}; 
}

inline struct Vector3 Vector3_add(const struct Vector3 v1, const struct Vector3 v2) {
    return (struct Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline struct Vector3 Vector3_sub(const struct Vector3 v1, const struct Vector3 v2) {
    return (struct Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

inline struct Vector3 Vector3_smul(const struct Vector3 v, const float s) {
    return (struct Vector3){s * v.x, s * v.y, s * v.z, s * v.w};
}

inline struct Vector3 Vector3_sdiv(const struct Vector3 v, const float s) {
    return Vector3_smul(v, 1 / s);
}

inline struct Vector3 Vector3_cross(const struct Vector3 v1, const struct Vector3 v2) {
    return (struct Vector3){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        0 // Cross product only makes sense in terms of directions.
    };
}

inline float Vector3_dot(const struct Vector3 v1, const struct Vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float Vector3_norm(const struct Vector3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float Vector3_norm_squared(const struct Vector3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline struct Vector3 Vector3_normalize(const struct Vector3 v) {
    return Vector3_smul(v, 1.0 / Vector3_norm(v));
}

inline struct Vector3 Vector3_mul_Matrix4(const struct Vector3 v, const struct Matrix4 *m) {
    return (struct Vector3) {
        v.x * m->x00 + v.y * m->x01 + v.z * m->x02 + v.w * m->x03,
        v.x * m->x10 + v.y * m->x11 + v.z * m->x12 + v.w * m->x13,
        v.x * m->x20 + v.y * m->x21 + v.z * m->x22 + v.w * m->x23,
        v.x * m->x30 + v.y * m->x31 + v.z * m->x32 + v.w * m->x33,
    };
}

inline void Vector3_print(const struct Vector3 v) {
    printf("Vector3[%f, %f, %f, (%f)]", v.x, v.y, v.z, v.w);
}
