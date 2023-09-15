#ifndef VOXEL_H
#define VOXEL_H

#include <vector>

#include "geometry/vec3.h"
#include "materials/material.h"
#include "objects/hittable.h"

enum VoxelType
{
	FILLED,
	PARTIAL,
	VOID
};

class Voxel : public hittable
{
public:
	Voxel(double _size, point3 _center, shared_ptr<Material> mat) : size(_size), center(_center), material_ptr(mat) {
		bounds[0] = vec3{
			_center.x() - size / 2,
			_center.y() - size / 2,
			_center.z() - size / 2
		};

		bounds[1] = vec3{
			_center.x() + size / 2,
			_center.y() + size / 2,
			_center.z() + size / 2
		};
	}

	float Size() const {
		return size;
	}

	point3 Center() const {
		return center;
	}

	void SetType(VoxelType _type) {
		type = _type;
	}

	void Subdivide() {
		for (int i = 0; i < 8; i++)
		{
			point3 voxelCenter;
			voxelCenter.setX(center.x() + size * (i & 4 ? .25f : -.25f)); // 0010 & 0100 = 0000 -> 0100 (4) 0101 (5) 0110 (6) 0111 (7)
			voxelCenter.setY(center.y() + size * (i & 2 ? .25f : -.25f)); // 0010 & 0010 = 0010 -> 0010 (2) 0011 (3) 0110 (6) 0111 (7)
			voxelCenter.setZ(center.z() + size * (i & 1 ? .25f : -.25f)); // 0010 & 0001 = 0001 -> 0001 (1) 0011 (3) 0101 (5) 0111 (7)
			
			children.push_back(make_shared<Voxel>(size / 2, voxelCenter, material_ptr));
		}
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
	{
		float tmin, tmax, tymin, tymax, tzmin, tzmax;
		double closest = t_max;

		tmin = (bounds[r.sign[0]].x() - r.orig.x()) * r.invdir.x();
		tmax = (bounds[1 - r.sign[0]].x() - r.orig.x()) * r.invdir.x();
		tymin = (bounds[r.sign[1]].y() - r.orig.y()) * r.invdir.y();
		tymax = (bounds[1 - r.sign[1]].y() - r.orig.y()) * r.invdir.y();

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (bounds[r.sign[2]].z() - r.orig.z()) * r.invdir.z();
		tzmax = (bounds[1 - r.sign[2]].z() - r.orig.z()) * r.invdir.z();

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		if (tmin < t_min || t_max < tmin) return false;

		if (type == FILLED)
		{
			vec3 center_to_point = (r.at(tmin) - center);

			center_to_point /= center_to_point.length();

			center_to_point /= max(max(abs(center_to_point.x()), abs(center_to_point.y())), abs(center_to_point.z()));

			double x = (int) center_to_point.x();
			double y = (int) center_to_point.y();
			double z = (int) center_to_point.z();

			vec3 normal{ x, y, z };

			rec.t = tmin;
			rec.set_face_normal(r, normal);
			rec.material_ptr = material_ptr;
			rec.p = r.at(tmin);

			return true;
		}

		if (type == PARTIAL)
		{
			bool hit_anything = false;
			hit_record temp_rec;

			for (shared_ptr<Voxel> v : children)
			{
				if (v->hit(r, t_min, closest, temp_rec)) {
					hit_anything = true;
					closest = temp_rec.t;
					rec = temp_rec;
				}
			}

			return hit_anything;
		}

		return false;
	}


private:
	VoxelType type;
	double size;
	point3 center;

	vec3 bounds[2];

	shared_ptr<Material> material_ptr;

public:
	std::vector<shared_ptr<Voxel>> children;
};

#endif // !VOXEL_H
