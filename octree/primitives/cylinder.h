#ifndef OCTREE_CYLINDER
#define OCTREE_CYLINDER

#include "geometry/vec3.h"
#include "solid.h"

class Cylinder : public Solid
{
public:
	Cylinder(point3 _basePoint, double _radius, double _length) : basePoint(_basePoint), radius(_radius), length(_length) {}

	void ComputeBoundingBox()
	{
		min.setX(basePoint.x() - radius);
		min.setY(basePoint.y());
		min.setZ(basePoint.z() - radius);

		max.setX(basePoint.x() + radius);
		max.setY(basePoint.y() + length);
		max.setZ(basePoint.z() + radius);
	}

	virtual bool IsInside(point3 p) override
	{
		if (p.y() >= min.y() && p.y() <= max.y())
		{
			double distance = pow((p.x() - basePoint.x()), 2) + pow((p.z() - basePoint.z()), 2);

			if (distance <= pow(radius, 2)) {
				return true;
			}
		}

		return false;
	}


	point3 Min() override
	{
		return min;
	}

	point3 Max() override
	{
		return max;
	}

	point3 Center() const {
		return point3{ (max.x() + min.x())/2.0f, (max.y() + min.y()) / 2.0f,(max.z() + min.z()) / 2.0f };
	}

private:
	point3 basePoint;
	double radius;
	double length;

public:
	point3 min;
	point3 max;
};

#endif // !OCTREE_CYLINDER
