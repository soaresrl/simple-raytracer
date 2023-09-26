#ifndef OCTREE_BLOCK
#define OCTREE_BLOCK

#include "utils/utility.h"
#include "solid.h"

class Block : public Solid
{
public:
	Block(point3 _center, double _size) : center(_center), size(_size){}

	void ComputeBoundingBox()
	{
		min.setX(center.x() - size / 2.0f);
		min.setY(center.y() - size / 2.0f);
		min.setZ(center.z() - size / 2.0f);

		max.setX(center.x() + size / 2.0f);
		max.setY(center.y() + size / 2.0f);
		max.setZ(center.z() + size / 2.0f);
	}

	virtual bool IsInside(point3 p) override
	{
		if (
			(p.x() <= max.x() && p.y() <= max.y() && p.z() <= max.z())
			&&
			(p.x() >= min.x() && p.y() >= min.y() && p.z() >= min.z())
			)
		{
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

	point3 Center() const override {
		return center;
	}

private:
	point3 center;
	double size;

public:
	point3 min;
	point3 max;
};

#endif // !OCTREE_BLOCK
