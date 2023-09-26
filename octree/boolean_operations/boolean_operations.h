#ifndef BOOLEAN_OPERATIONS_H
#define BOOLEAN_OPERATIONS_H

#include <stack>

#include "octree/voxel.h"

shared_ptr<Voxel> Union(shared_ptr<Voxel> octree1, shared_ptr<Voxel> octree2);

shared_ptr<Voxel> Intersection(shared_ptr<Voxel> octree1, shared_ptr<Voxel> octree2);

shared_ptr<Voxel> Difference(shared_ptr<Voxel> octree1, shared_ptr<Voxel> octree2);

#endif // !BOOLEAN_OPERATIONS_H
