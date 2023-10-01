#ifndef VOXEL_H
#define VOXEL_H

#include <vector>

#include "utils/utility.h"
#include "materials/material.h"
#include "objects/hittable.h"

enum VoxelType
{
	FILLED,
	PARTIAL,
	EMPTY
};

class Voxel : public hittable
{
public:
	Voxel(double _size, point3 _center, shared_ptr<Material> mat, int index) : size(_size), center(_center), material_ptr(mat), parent(nullptr), indexRelativeToFather(index) {
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
	
	Voxel(double _size, point3 _center, shared_ptr<Material> mat) : size(_size), center(_center), material_ptr(mat), parent(nullptr), indexRelativeToFather(-1) {
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
	/*

	Voxel(double _size, point3 _center, shared_ptr<Material> mat, shared_ptr<Voxel> _parent, int index) : size(_size), center(_center), material_ptr(mat), parent(_parent), indexRelativeToFather(index) {
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
	}*/

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
			
			auto vox = make_shared<Voxel>(size / 2, voxelCenter, material_ptr, i);

			vox->parent = this;

			children.push_back(vox);
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

		if (type == FILLED /*|| (type == PARTIAL && children.size() == 0)*/)
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

	bool IsEmpty() {
		return children.size() == 0;
	}

	bool Contains(point3 p) {
		if (
			((center.x() - size / 2.0) <= p.x() &&
				(center.y() - size / 2.0) <= p.y() &&
				(center.z() - size / 2.0) <= p.z()) &&
			((center.x() + size / 2.0) >= p.x() &&
				(center.y() + size / 2.0) >= p.y() &&
				(center.z() + size / 2.0) >= p.z())
			)
		{
			return true;
		}

		return false;
	}

	bool IsBoxInside(point3 min, point3 max) {

		if ((
				min.x() >= (center.x() - size / 2.0) &&
				min.y() >= (center.y() - size / 2.0) &&
				min.z() >= (center.z() - size / 2.0)) &&
			(
				max.x() <= (center.x() + size / 2.0) &&
				max.y() <= (center.y() + size / 2.0) &&
				max.z() <= (center.z() + size / 2.0)

				)
			) 
		{
			return true;
		}

		return false;
	}

	VoxelType Type() const {
		return type;
	}

private:
	VoxelType type = EMPTY;

	vec3 bounds[2];

public:
	double size;
	point3 center;
	shared_ptr<Material>			material_ptr;
	std::vector<shared_ptr<Voxel>>	children;
	Voxel*							parent;
	int								indexRelativeToFather;
	int								depth = 0;
	// 0 -> BBL
	// 1 -> BFL
	// 2 -> UBL
	// 3 -> UFL
	// 4 -> BBR
	// 5 -> BBR
	// 6 -> BFR
	// 7 -> UBR
};

#endif // !VOXEL_H
