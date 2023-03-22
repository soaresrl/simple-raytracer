#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction)
    {
        invdir = vec3(1.0 / dir.x(), 1.0 / dir.y(), 1.0 / dir.z());
        sign[0] = (invdir.x() < 0);
        sign[1] = (invdir.y() < 0);
        sign[2] = (invdir.z() < 0);

    }

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t) const {
        return orig + t * dir;
    }

public:
    point3 orig;
    vec3 dir;
    vec3 invdir;
    int sign[3];
};

#endif