#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "geometry/color.h"
#include "geometry/vec3.h"
#include "objects/light.h"
#include "objects/hittable_list.h"
#include "scene.h"

class point_light : public Light {
public:
	point_light() {};

	point_light(point3 _position, color _intensity) : position(_position), intensity(_intensity) {};

    color calc_contribution(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) override;
    vec3 get_direction(point3 _point) const override;

public:
	point3 position;
    color intensity;
};

color point_light::calc_contribution(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) {
    color contribution(0.0f, 0.0f, 0.0f);

    vec3 light_direction = position - _point;

    double n_dot_l = dot(_normal, light_direction);

    // Shadow check
    hit_record shadow_hit_record;

    if (world.hit(ray(_point, light_direction), 0.001, 1.0, shadow_hit_record)) {
        return contribution;
    }

    // Diffuse
    if (n_dot_l > 0) {
        contribution += intensity * n_dot_l / (_normal.length() * light_direction.length());
    }

    // Specular
    if (_specular != -1) {
        vec3 reflected_light_ray = 2 * _normal * dot(_normal, light_direction) - light_direction;
        double r_dot_v = dot(reflected_light_ray, camera_pos - _point);

        if (r_dot_v > 0) {
            contribution += intensity * pow(r_dot_v / (reflected_light_ray.length() * (camera_pos - _point).length()), _specular);
        }
    }

    return contribution;
}

vec3 point_light::get_direction(point3 _point) const {
    return position - _point;
}

#endif // !POINT_LIGHT_H
