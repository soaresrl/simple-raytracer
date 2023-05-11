#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"
#include "geometry/color.h"
#include "geometry/ray.h"
#include "geometry/vec3.h"

class plane : public hittable {
public:
	plane(point3 _point, vec3 _normal, shared_ptr<Material> mat) : point(_point), normal(_normal), material_ptr(mat) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
	point3 point;
	vec3 normal;
	shared_ptr<Material> material_ptr;
};

bool plane::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	double denom = dot(normal, r.direction());

	if (denom > 1e-6) {
		vec3 origin_to_plane = point - r.origin();

		double t = dot(origin_to_plane, normal) / denom;

		if (t >= t_min && t <= t_max) {
			rec.t = t;
			rec.p = r.at(rec.t);
			rec.material_ptr = material_ptr;
			rec.set_face_normal(r, normal);

			return true;
		}
	}

	return false;
}
#endif // !PLANE_H
