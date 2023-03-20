#ifndef SPHERE_H
#define SPHERE_H

#include "objects/hittable.h"
#include "geometry/vec3.h"

class sphere : public hittable {
	public:
		sphere(){}
		sphere(point3 _center, double r, color color, int _specular, float _reflective) : center(_center), radius(r), col(color), specular(_specular), reflective(_reflective) {};

		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        color col;
        int specular;
        float reflective;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 origin_to_center = r.origin() - center;

    auto a = r.direction().length_squared();
    auto half_b = dot(origin_to_center, r.direction());
    auto c = origin_to_center.length_squared() - (radius * radius);

    auto delta = (half_b * half_b) - (a * c);

    if (delta < 0) {
        return false;
    }

    auto sqrt_delta = sqrt(delta);

    // encontrar o menor valor de t (mais próximo) no range t_min t_max
    auto min_root = (-half_b - sqrt_delta) / a;

    if (min_root < t_min || t_max < min_root) {
        min_root = (-half_b + sqrt_delta) / a;
        if (min_root < t_min || t_max < min_root) {
            return false;
        }
    }

    rec.t = min_root;
    rec.p = r.at(rec.t);
    rec.col = col;
    rec.specular = specular;
    rec.reflective = reflective;
    
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif // !SPHERE_H
