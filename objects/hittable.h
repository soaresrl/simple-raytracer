#ifndef HITTABLE_H
#define HITTABLE_H

#include "geometry/ray.h"

struct hit_record
{
	point3 p;
	vec3 normal;
	double t;
	bool is_front_face;
	color col;
	int specular;
	float reflective;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		is_front_face = dot(r.direction(), outward_normal) < 0;
		normal = is_front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
	public:
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // !HITTABLE_H
