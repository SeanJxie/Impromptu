#ifndef LIGHT_H
#define LIGHT_H

#include "vector3.h"

#define M_PI4 12.56637061435917295385057353311801152

#define LIGHT_TYPE_POINT 0;

struct LightSource {
    int type;
    float power;
    struct Vector3 pos;
};

inline float LightSource_get_intensity(struct LightSource s, struct Vector3 p);

#endif