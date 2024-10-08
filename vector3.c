#include "vector3.h"

inline struct Vector3 Vector3_create_point(float x, float y, float z) {
    // A point has w = 1.
    return (struct Vector3){x, y, z, 1}; 
}

inline struct Vector3 Vector3_create_direction(float x, float y, float z) {
    // A direction (represented as a point at infinity) has w = 0.
    return (struct Vector3){x, y, z, 0}; 
}

inline struct Vector3 Vector3_add(struct Vector3 v1, struct Vector3 v2) {
    return (struct Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

inline struct Vector3 Vector3_sub(struct Vector3 v1, struct Vector3 v2) {
    return (struct Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

inline struct Vector3 Vector3_smul(struct Vector3 v, const float s) {
    return (struct Vector3){s * v.x, s * v.y, s * v.z, s * v.w};
}

inline struct Vector3 Vector3_sdiv(struct Vector3 v, const float s) {
    return Vector3_smul(v, 1 / s);
}

inline struct Vector3 Vector3_cross(struct Vector3 v1, struct Vector3 v2) {
    return (struct Vector3){
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        0 // Cross product only makes sense in terms of directions.
    };
}

inline float Vector3_dot(struct Vector3 v1, struct Vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float Vector3_norm(struct Vector3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline float Vector3_norm_squared(struct Vector3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline struct Vector3 Vector3_normalize(struct Vector3 v) {
    return Vector3_smul(v, 1 / Vector3_norm(v));
}

inline void Vector3_print(const char *str, struct Vector3 v) {
    printf("%s: Vector3[%f, %f, %f, (%f)]\n", str, v.x, v.y, v.z, v.w);
}
