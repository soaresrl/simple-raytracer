#ifndef STL_H
#define STL_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <vector>

#include <string>

#include "materials/lambertian.h"

#include "objects/triangle.h"
#include "objects/hittable.h"
#include "objects/hittable_list.h"

std::vector<shared_ptr<hittable_list>> FromOBJ(std::string filename)
{
	std::ifstream in(filename);

	if (!in) {
		throw "Can't open file";
	}

	std::vector<shared_ptr<hittable_list>> meshes;
	std::vector<point3> points;

	meshes.clear();
	points.clear();

	int numberOfObjects = 0;
	int numberOfVertices = 0;

	std::string stepLine;

	while (std::getline(in, stepLine))
	{
		std::stringstream stream;
		stream << stepLine;

		char typeDelimiter = NULL;

		stream >> typeDelimiter;

		if (typeDelimiter == 'o')
		{
			auto mesh = make_shared<hittable_list>();

			meshes.push_back(mesh);
			numberOfObjects++;
		}
		else if (typeDelimiter == 'v')
		{
			numberOfVertices++;

			float x, y, z;

			stream >> x;
			stream >> y;
			stream >> z;

			point3 point{ x, y, z };

			points.push_back(point);
		}
		else if (typeDelimiter == 'f')
		{
			int id1, id2, id3;

			stream >> id1;
			stream >> id2;
			stream >> id3;

			auto red = make_shared<Lambertian>(color(.65, .05, .05));

			meshes[numberOfObjects - 1]->add(make_shared<Triangle>(points[id1 - 1], points[id2 - 1], points[id3 - 1], red));
		}
	}

	return meshes;
}

#endif // !STL_H
