#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>

#include "hittable.h"

using namespace std;

class Triangle : public hittable
{
public:
	Triangle(point3 _v1, point3 _v2, point3 _v3, shared_ptr<Material> mat) : v1(_v1), v2(_v2), v3(_v3), mat_ptr(mat) {}
	
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
	{
		vec3 normal = unit_vector(cross(v2 - v1, v3 - v1));

		// check if triangle's plane and ray are parallel
		double normalDotRay = dot(normal, r.direction());
		if (fabs(normalDotRay) < 0.00001)
		{
			return false;
		}

		double D = - dot(normal, v1);

		float t = -(dot(normal, r.origin()) + D) / (dot(normal, r.direction()));

		if (t < t_min || t > t_max) return false;

		point3 p = r.origin() + t * r.direction();

		vec3 e1 = v2 - v1;
		vec3 ep1 = p - v1;

		vec3 perp = cross(e1, ep1);
		if (dot(normal, perp) < 0.0) return false;
		
		vec3 e2 = v3 - v2;
		vec3 ep2 = p - v2;

		perp = cross(e2, ep2);
		if (dot(normal, perp) < 0.0) return false;

		vec3 e3 = v1 - v3;
		vec3 ep3 = p - v3;

		perp = cross(e3, ep3);
		if (dot(normal, perp) < 0.0) return false;


		rec.t = t;
		rec.p = p;
		rec.material_ptr = mat_ptr;

		rec.set_face_normal(r, normal);

		return true;
	}


private:
	point3 v1, v2, v3;
	shared_ptr<Material> mat_ptr;
};

#endif // !TRIANGLE_H
