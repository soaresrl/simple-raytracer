#ifndef AA_PLANE
#define AA_PLANE

#include <memory>

#include "geometry/vec3.h"
#include "materials/material.h"
#include "objects/hittable.h"

using namespace std;

class XY_Plane : public hittable
{
public:
	XY_Plane(point3 _center, float w, float h, shared_ptr<Material> mat) : center(_center), width(w), height(h), material(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override 
	{
		double t = (center.z() - r.origin().z()) / r.direction().z();

		if (t < t_min || t > t_max) return false;

		double x = r.origin().x() + t * r.direction().x();
		double y = r.origin().y() + t * r.direction().y();

		if (x < center.x() - width / 2 || x > center.x() + width / 2 || y < center.y() - height / 2 || y > center.y() + height / 2)
			return false;

		rec.t = t;
		vec3 outward_normal = vec3(0, 0, 1);
		rec.set_face_normal(r, outward_normal);
		rec.material_ptr = material;
		rec.p = r.at(t);

		return true;
	}

private:
	float width;
	float height;
	point3 center;
	shared_ptr<Material> material;
};

class XZ_Plane : public hittable
{
public:
	XZ_Plane(point3 _center, float w, float h, shared_ptr<Material> mat) : center(_center), width(w), height(h), material(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override 
	{
		double t = (center.y() - r.origin().y()) / r.direction().y();

		if (t < t_min || t > t_max) return false;

		double x = r.origin().x() + t * r.direction().x();
		double z = r.origin().z() + t * r.direction().z();

		if (x < center.x() - width / 2 || x > center.x() + width / 2 || z < center.z() - height / 2 || z > center.z() + height / 2)
			return false;

		rec.t = t;
		vec3 outward_normal = vec3(0, 1, 0);
		rec.set_face_normal(r, outward_normal);
		rec.material_ptr = material;
		rec.p = r.at(t);

		return true;
	}

	virtual double pdf_value(const point3& origin, const vec3& v) const override {
		hit_record rec;
		if (!this->hit(ray(origin, v), 0.001, infinity, rec))
			return 0;

		auto area = width * height;
		auto distance_squared = rec.t * rec.t * v.length_squared();
		auto cosine = fabs(dot(v, rec.normal) / v.length());

		return distance_squared / (cosine * area);
	}

	virtual vec3 random(const point3& origin) const override {
		auto random_point = point3(random_double(center.x() - (width / 2.0), center.x() + (width / 2.0)), center.y(), random_double(center.z() - (height / 2.0), center.z() + (height / 2.0)));
		return random_point - origin;
	}

private:
	float width;
	float height;
	point3 center;
	shared_ptr<Material> material;
};

class YZ_Plane : public hittable
{
public:
	YZ_Plane(point3 _center, float w, float h, shared_ptr<Material> mat) : center(_center), width(w), height(h), material(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
	{
		double t = (center.x() - r.origin().x()) / r.direction().x();

		if (t < t_min || t > t_max) return false;

		double y = r.origin().y() + t * r.direction().y();
		double z = r.origin().z() + t * r.direction().z();

		if (y < center.y() - width / 2 || y > center.y() + width / 2 || z < center.z() - height / 2 || z > center.z() + height / 2)
			return false;

		rec.t = t;
		vec3 outward_normal = vec3(1, 0, 0);
		rec.set_face_normal(r, outward_normal);
		rec.material_ptr = material;
		rec.p = r.at(t);

		return true;
	}

private:
	float width;
	float height;
	point3 center;
	shared_ptr<Material> material;
};

#endif // !AA_PLANE
