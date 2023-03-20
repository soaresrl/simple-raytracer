#ifndef LIGHT_H
#define LIGHT_H

#include "geometry/color.h"
#include "objects/hittable.h"

class Light {
public:
	virtual color calc_contribution(point3 camera_post, point3 point, vec3 normal, int _specular, hittable& world) = 0;
	virtual vec3 get_direction(point3 _point) const = 0;
};
#endif // !LIGHT_H
