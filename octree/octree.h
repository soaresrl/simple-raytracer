#ifndef OCTREE_BUILD_H
#define OCTREE_BUILD_H

#include <memory>

#include <fstream>
#include <stack>

#include "voxel.h"
#include "primitives/solid.h"

VoxelType Classify(Solid* s, shared_ptr<Voxel> voxel);
void BuildOctree(Solid* s, shared_ptr<Voxel> root, int depth);
void GetPointsFromTraverse(shared_ptr<Voxel> octree, std::vector<float>* points, std::vector<unsigned int>* elems);
double ComputeTotalVolume(shared_ptr<Voxel> octree);
void WriteFromTraverse(shared_ptr<Voxel> octree, std::ofstream& write_file);

#endif // !OCTREE_BUILD_H
