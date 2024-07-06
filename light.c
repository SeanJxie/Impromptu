#include "light.h"

inline float LightSource_get_intensity(struct LightSource s, struct Vector3 p) {
    float r2 = Vector3_norm_squared(Vector3_sub(p, s.pos));

    return s.power / (M_PI4 * r2);
}