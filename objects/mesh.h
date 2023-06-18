#ifndef MESH_H
#define MESH_H

#include "hittable_list.h"

class Mesh : public hittable_list
{
public:
	Mesh ();

private:
	std::vector<point3> points;

};

#endif // !MESH_H
