#ifndef OCTREE_SOLID_H
#define OCTREE_SOLID_H

#include "utils/utility.h"

class Solid
{
public:
	virtual bool IsInside(point3 p) = 0;
	virtual point3 Min() = 0;
	virtual point3 Max() = 0;
	virtual point3 Center() const = 0;
};

#endif // !OCTREE_SOLID_H
