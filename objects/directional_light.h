#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "utils/utility.h"
#include "objects/light.h"
#include "geometry/color.h"
#include "geometry/vec3.h"

class directional_light : public Light {
public:
	directional_light() {};

	directional_light(color _intensity, vec3 _direction) : intensity(_intensity), direction(_direction) {};

    color calc_contribution(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) override;
    vec3 get_direction(point3 _point) const override;

public:
	color intensity;
    vec3 direction;
};

color directional_light::calc_contribution(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) {
    color contribution(0.0f, 0.0f, 0.0f);

    double n_dot_l = dot(_normal, direction);

    // Shadow check
    hit_record shadow_hit_record;

    if (world.hit(ray(_point, direction), 0.001, infinity, shadow_hit_record)) {
        return contribution;
    }

    // Diffuse
    if (n_dot_l > 0) {
        contribution += intensity * n_dot_l / (_normal.length() * direction.length());
    }

    // Specular
    if (_specular != -1) {
        vec3 reflected_light_ray = 2 * _normal * dot(_normal, direction) - direction;
        double r_dot_v = dot(reflected_light_ray, (camera_pos - _point));

        if (r_dot_v > 0) {
            contribution += intensity * pow(r_dot_v / (reflected_light_ray.length() * (camera_pos - _point).length()), _specular);
        }
    }

    return contribution;
}

vec3 directional_light::get_direction(point3 _point) const {
    return direction;
}

#endif // !DIRECTIONAL_LIGHT_H
