#include "utils/utility.h"

#include "octree/voxel.h"
#include "octree/octree.h"
#include "octree/primitives/sphere.h"


int main() {
	Sphere sphere{ point3{0.0f, 0.0f, 0.0f}, 1.0 };

	sphere.ComputeBoundingBox();

	double size = sphere.max.x() - sphere.min.x();

	Voxel octree{ size, sphere.Center() };

	BuildOctree(&sphere, &octree, 4);

	return 0;
}