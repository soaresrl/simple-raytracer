#ifndef OCTREE_BUILD_H
#define OCTREE_BUILD_H

#include <memory>

#include "voxel.h"
#include "primitives/solid.h"

VoxelType Classify(Solid* s, shared_ptr<Voxel> voxel) {
	int n = 0;

	for (int i = 0; i < 8; ++i) {
		point3 cornerPoint = voxel->Center();

		cornerPoint.setX(voxel->Center().x() + voxel->Size() * (i & 4 ? .5f : -.5f)); // 0010 & 0100 = 0000 -> 0100 (4) 0101 (5) 0110 (6) 0111 (7)
		cornerPoint.setY(voxel->Center().y() + voxel->Size() * (i & 2 ? .5f : -.5f)); // 0010 & 0010 = 0010 -> 0010 (2) 0011 (3) 0110 (6) 0111 (7)
		cornerPoint.setZ(voxel->Center().z() + voxel->Size() * (i & 1 ? .5f : -.5f)); // 0010 & 0001 = 0001 -> 0001 (1) 0011 (3) 0101 (5) 0111 (7)

		if (s->IsInside(cornerPoint)) n++;
	}

	if (n == 8) return FILLED;
	if (n == 0 && !s->IsInside(voxel->Center())) {
		return VOID;
	}

	return PARTIAL;
}

void BuildOctree(Solid* s, shared_ptr<Voxel> root, int depth)
{
	shared_ptr<Voxel> parent = root;

	VoxelType type;

	if (depth > 1)
	{
		type = Classify(s, parent);
		parent->SetType(type);

		if (type == PARTIAL)
		{
			parent->Subdivide();

			for (shared_ptr<Voxel> child : parent->children)
			{
				BuildOctree(s, child, depth - 1);
			}
		}
	}
}

#endif // !OCTREE_BUILD_H
