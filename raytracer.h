#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include <string>
#include <fstream>

#include "utils/utility.h"
#include "utils/raycolor.h"

#include "io/stl.h"

#include "geometry/color.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "objects/scene.h"
#include "objects/camera.h"
#include "objects/point_light.h"
#include "objects/directional_light.h"
#include "objects/light.h"
#include "objects/plane.h"
#include "objects/xy_plane.h"
#include "objects/flip_face.h"
#include "objects/triangle.h"

#include "materials/diffuse_light.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"

#include "octree/primitives/sphere.h"
#include "octree/primitives/block.h"
#include "octree/primitives/cylinder.h"
#include "octree/primitives/cone.h"
#include "octree/octree.h"
#include "octree/boolean_operations/boolean_operations.h"

void GenerateImage();

#endif // !RAYTRACER_H
