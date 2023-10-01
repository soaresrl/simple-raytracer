#ifndef OCTREE_BUILD_H
#define OCTREE_BUILD_H

#include <memory>

#include <fstream>
#include <stack>

#include "voxel.h"
#include "primitives/solid.h"

#include "materials/lambertian.h"

VoxelType Classify(Solid* s, shared_ptr<Voxel> voxel);
void BuildOctree(Solid* s, shared_ptr<Voxel> root, int depth);
void GetPointsFromTraverse(shared_ptr<Voxel> octree, std::vector<float>* points, std::vector<unsigned int>* elems);
double ComputeTotalVolume(shared_ptr<Voxel> octree);
double ComputeSurfaceArea(shared_ptr<Voxel> octree);
void WriteFromTraverse(shared_ptr<Voxel> octree, std::ofstream& write_file);
shared_ptr<Voxel> ReadFromFile(std::ifstream& file);
void ScaleOct(shared_ptr<Voxel> octree, float scale_factor);
void TranslateOct(shared_ptr<Voxel> octree, float dx, float dy, float dz);

#endif // !OCTREE_BUILD_H
