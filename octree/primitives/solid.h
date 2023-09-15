#ifndef OCTREE_SOLID_H
#define OCTREE_SOLID_H

#include "geometry/vec3.h"

class Solid
{
public:
	virtual bool IsInside(point3 p) = 0;
	virtual point3 Min() = 0;
	virtual point3 Max() = 0;
};

#endif // !OCTREE_SOLID_H
