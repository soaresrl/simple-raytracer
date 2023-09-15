#ifndef OCTREE_SPHERE_H
#define OCTREE_SPHERE_H

#include "solid.h"

class Sphere : public Solid
{
public:
    Sphere(point3 _center, double r) : center(_center), radius(r) {};
    
	void ComputeBoundingBox()
	{
		min.setX(center.x() - radius);
		min.setY(center.y() - radius);
		min.setZ(center.z() - radius);

		max.setX(center.x() + radius);
		max.setY(center.y() + radius);
		max.setZ(center.z() + radius);
	}

	virtual bool IsInside(point3 p) override 
	{
		double dist = (p - center).length();

		if (dist < radius) {
			return true;
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
		return center;
	}

private:
    point3 center;

    double radius;

public:
    point3 min;
	point3 max;
};

#endif // !OCTREE_SPHERE_H
