#ifndef OCTREE_BLOCK
#define OCTREE_BLOCK

#include "utils/utility.h"
#include "solid.h"

class Block : public Solid
{
public:
	Block(point3 _center, double lx, double ly, double lz) : center(_center), m_lx(lx), m_ly(ly), m_lz(lz) {}

	void ComputeBoundingBox()
	{
		min.setX(center.x() - m_lx / 2.0f);
		min.setY(center.y() - m_ly / 2.0f);
		min.setZ(center.z() - m_lz / 2.0f);

		max.setX(center.x() + m_lx / 2.0f);
		max.setY(center.y() + m_ly / 2.0f);
		max.setZ(center.z() + m_lz / 2.0f);
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
	double m_lx;
	double m_ly;
	double m_lz;

public:
	point3 min;
	point3 max;
};

#endif // !OCTREE_BLOCK
