#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"
#include "geometry/color.h"
#include "geometry/ray.h"
#include "geometry/vec3.h"



class plane : public hittable {
public:
	plane(){}
	plane(point3 _point, vec3 _normal, color _col, int _specular, float _reflective) : point(_point), normal(_normal), col(_col), specular(_specular), reflective(_reflective) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	point3 point;
	vec3 normal;
	color col;
	int specular;
	float reflective;
};

bool plane::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double denom = dot(normal, r.direction());

	if (denom > 1e-6) {
		vec3 origin_to_plane = point - r.origin();

		double t = dot(origin_to_plane, normal) / denom;

		if (t >= t_min && t <= t_max) {
			//std::cerr << t << ' ' << std::flush;
			rec.t = t;
			rec.p = r.at(rec.t);
			rec.col = col;
			rec.specular = specular;
			rec.reflective = reflective;
			rec.set_face_normal(r, normal);

			return true;
		}
	}

	return false;
}
#endif // !PLANE_H
