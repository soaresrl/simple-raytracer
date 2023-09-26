#ifndef OCTREE_CONE
#define OCTREE_CONE

#include "utils/utility.h"
#include "solid.h"

class Cone : public Solid
{
public:
	Cone(point3 _basePoint, double _radius, double _height) : basePoint(_basePoint), radius(_radius), height(_height) {}

	void ComputeBoundingBox()
	{
		min.setX(basePoint.x() - radius);
		min.setY(basePoint.y());
		min.setZ(basePoint.z() - radius);

		max.setX(basePoint.x() + radius);
		max.setY(basePoint.y() + height);
		max.setZ(basePoint.z() + radius);
	}

	virtual bool IsInside(point3 p) override
	{
		if (p.y() >= min.y() && p.y() <= max.y())
		{
			double radiusForHeight = (radius / height) * p.y();

			double distance = pow((p.x() - basePoint.x()), 2) + pow((p.z() - basePoint.z()), 2);

			if (distance <= pow(radiusForHeight, 2))
			{
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

	point3 Center() const override {
		return point3{ (max.x() + min.x()) / 2.0f, (max.y() + min.y()) / 2.0f, (max.z() + min.z()) / 2.0f };
	}

private:
	point3 basePoint;
	double radius;
	double height;

public:
	point3 min;
	point3 max;
};

#endif // !OCTREE_CONE
