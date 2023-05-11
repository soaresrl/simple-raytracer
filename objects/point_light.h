#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "geometry/color.h"
#include "geometry/vec3.h"
#include "objects/light.h"
#include "utils/raycolor.h"

class point_light : public Light {
public:
	point_light() {};

	point_light(point3 _position, color _intensity) : position(_position), intensity(_intensity) {};

    virtual color calc_contribution(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) override;
    virtual color calc_diffuse_component(point3 camera_pos, point3 _point, vec3 _normal, hittable& world) override;
    virtual color calc_specular_component(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) override;

    virtual vec3 get_direction(point3 _point) const override;

public:
	point3 position;
    color intensity;
};

color point_light::calc_diffuse_component(point3 camera_pos, point3 _point, vec3 _normal, hittable& world) {
    color contribution(0.0f, 0.0f, 0.0f);

    vec3 light_direction = get_direction(_point);

    double n_dot_l = dot(_normal, light_direction);

    // Shadow check
    hit_record shadow_hit_record;

    ray r(_point, light_direction);

    if (world.hit(ray(_point, light_direction), 0.001, 1.0, shadow_hit_record)) {
        //if (shadow_hit_record.refractive_index == -1) {
        return contribution;
        //}

        //return 0.4 * abs(dot(shadow_hit_record.normal, r.direction())) * calc_contribution(camera_pos, shadow_hit_record.p, shadow_hit_record.normal, _specular, world);
    }

    // Diffuse
    if (n_dot_l > 0) {
        contribution += intensity * n_dot_l / (_normal.length() * light_direction.length());
    }

    return contribution;
}

color point_light::calc_specular_component(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) {
    color contribution(0.0f, 0.0f, 0.0f);

    vec3 light_direction = get_direction(_point);

    double n_dot_l = dot(_normal, light_direction);

    // Shadow check
    hit_record shadow_hit_record;

    ray r(_point, light_direction);

    if (world.hit(ray(_point, light_direction), 0.001, 1.0, shadow_hit_record)) {
        //if (shadow_hit_record.refractive_index == -1) {
        return contribution;
        //}

        //return 0.4 * abs(dot(shadow_hit_record.normal, r.direction())) * calc_contribution(camera_pos, shadow_hit_record.p, shadow_hit_record.normal, _specular, world);
    }

    // Specular
    if (_specular != -1) {
        vec3 reflected_light_ray = reflect(light_direction, _normal);
        double r_dot_v = dot(reflected_light_ray, _point - camera_pos);

        if (r_dot_v > 0) {
            contribution += intensity * pow(r_dot_v / (reflected_light_ray.length() * (_point - camera_pos).length()), _specular);
        }
    }

    return contribution;
}

color point_light::calc_contribution(point3 camera_pos, point3 _point, vec3 _normal, int _specular, hittable& world) {
    color contribution(0.0f, 0.0f, 0.0f);

    vec3 light_direction = get_direction(_point);

    double n_dot_l = dot(_normal, light_direction);

    // Shadow check
    hit_record shadow_hit_record;

    ray r(_point, light_direction);

    if (world.hit(ray(_point, light_direction), 0.001, 1.0, shadow_hit_record)) {
        //if (shadow_hit_record.refractive_index == -1) {
            return contribution;
        //}
        
        //return 0.4 * abs(dot(shadow_hit_record.normal, r.direction())) * calc_contribution(camera_pos, shadow_hit_record.p, shadow_hit_record.normal, _specular, world);
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
