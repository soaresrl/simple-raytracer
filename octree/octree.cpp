#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include <qdebug.h>

#include "octree.h"

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
	if (n == 0) {
		int n = 0;

		for (int i = 0; i < 8; ++i) {
			point3 cornerPoint = s->Center();

			cornerPoint.setX(s->Center().x() + (s->Max().x() - s->Min().x())* (i & 4 ? .5f : -.5f)); // 0010 & 0100 = 0000 -> 0100 (4) 0101 (5) 0110 (6) 0111 (7)
			cornerPoint.setY(s->Center().y() + (s->Max().y() - s->Min().y()) * (i & 2 ? .5f : -.5f)); // 0010 & 0010 = 0010 -> 0010 (2) 0011 (3) 0110 (6) 0111 (7)
			cornerPoint.setZ(s->Center().z() + (s->Max().z() - s->Min().z()) * (i & 1 ? .5f : -.5f)); // 0010 & 0001 = 0001 -> 0001 (1) 0011 (3) 0101 (5) 0111 (7)

			if (voxel->Contains(cornerPoint)) n++;
		}

		if (n == 0) return EMPTY;

		return PARTIAL;
	}

	return PARTIAL;
}

void BuildOctree(Solid* s, shared_ptr<Voxel> root, int depth)
{
	shared_ptr<Voxel> parent = root;
	parent->depth = depth;

	VoxelType type = Classify(s, parent);
	parent->SetType(type);

	if (depth > 1)
	{
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

void GetPointsFromTraverse(shared_ptr<Voxel> octree, std::vector<float>* points, std::vector<unsigned int>* elems) {
	stack<shared_ptr<Voxel>> stack;

	stack.push(octree);
	unsigned int count = 0;

	while (!stack.empty())
	{
		auto node = stack.top();

		stack.pop();

		switch (node->Type())
		{
		case FILLED:
			for (int i = 0; i < 8; ++i) {
				points->push_back(node->Center().x() + node->Size() * (i & 4 ? .5f : -.5f));
				points->push_back(node->Center().y() + node->Size() * (i & 2 ? .5f : -.5f));
				points->push_back(node->Center().z() + node->Size() * (i & 1 ? .5f : -.5f));
			}

			elems->push_back(count + 0);
			elems->push_back(count + 1);
			elems->push_back(count + 1);
			elems->push_back(count + 3);
			elems->push_back(count + 3);
			elems->push_back(count + 2);
			elems->push_back(count + 2);
			elems->push_back(count + 0);

			elems->push_back(count + 4);
			elems->push_back(count + 6);
			elems->push_back(count + 6);
			elems->push_back(count + 7);
			elems->push_back(count + 7);
			elems->push_back(count + 5);
			elems->push_back(count + 5);
			elems->push_back(count + 4);

			elems->push_back(count + 3);
			elems->push_back(count + 7);
			elems->push_back(count + 6);
			elems->push_back(count + 2);
			elems->push_back(count + 0);
			elems->push_back(count + 4);
			elems->push_back(count + 5);
			elems->push_back(count + 1);

			count += 8;
			break;
		case PARTIAL:
			if (node->children.size() > 0)
			{
				for (int i = 0; i < 8; i++)
				{
					stack.push(node->children[i]);
				}
			}
			/*else {
				for (int i = 0; i < 8; ++i) {
					points->push_back(node->Center().x() + node->Size() * (i & 4 ? .5f : -.5f));
					points->push_back(node->Center().y() + node->Size() * (i & 2 ? .5f : -.5f));
					points->push_back(node->Center().z() + node->Size() * (i & 1 ? .5f : -.5f));
				}

				elems->push_back(count + 0);
				elems->push_back(count + 1);
				elems->push_back(count + 1);
				elems->push_back(count + 3);
				elems->push_back(count + 3);
				elems->push_back(count + 2);
				elems->push_back(count + 2);
				elems->push_back(count + 0);

				elems->push_back(count + 4);
				elems->push_back(count + 6);
				elems->push_back(count + 6);
				elems->push_back(count + 7);
				elems->push_back(count + 7);
				elems->push_back(count + 5);
				elems->push_back(count + 5);
				elems->push_back(count + 4);

				elems->push_back(count + 3);
				elems->push_back(count + 7);
				elems->push_back(count + 6);
				elems->push_back(count + 2);
				elems->push_back(count + 0);
				elems->push_back(count + 4);
				elems->push_back(count + 5);
				elems->push_back(count + 1);

				count += 8;
			}*/
			break;
		case EMPTY:
		default:
			break;
		}


	}
}

double ComputeTotalVolume(shared_ptr<Voxel> octree) {
	stack<shared_ptr<Voxel>> stack;

	stack.push(octree);

	double total_volume = 0.0;

	while (!stack.empty())
	{
		auto node = stack.top();

		stack.pop();


		switch (node->Type())
		{
		case FILLED:
			total_volume = total_volume + pow(node->size, 3);
			break;
		case PARTIAL:
		case EMPTY:
		default:
			break;
		}

		if (node->children.size() > 0)
		{
			for (int i = 0; i < 8; i++)
			{
				stack.push(node->children[i]);
			}
		}
	}

	return total_volume;
}

double ComputeSurfaceArea(shared_ptr<Voxel> octree) {
	stack<shared_ptr<Voxel>> stack;

	stack.push(octree);

	double surface_area = 0.0;

	while (!stack.empty())
	{
		auto node = stack.top();

		stack.pop();

		if (node->Type() == FILLED)
		{
			
			point3 neighboursCenters[6] = {
				{node->Center().x() - node->Size(), node->Center().y(), node->Center().z()},
				{node->Center().x(), node->Center().y() - node->Size(), node->Center().z()},
				{node->Center().x(), node->Center().y(), node->Center().z() - node->Size()},
				{node->Center().x() + node->Size(), node->Center().y(), node->Center().z()},
				{node->Center().x(), node->Center().y() + node->Size(), node->Center().z()},
				{node->Center().x(), node->Center().y(), node->Center().z() + node->Size()},
			};

			for (int i = 0; i < 6; i++)
			{
				// check the limits of root node bounding box
				// if neighbour is outsid this is a extreme voxel and needs to be computed
				if (
					neighboursCenters[i].x() < node->Center().x() - node->Size() / 2 ||
					neighboursCenters[i].y() < node->Center().y() - node->Size() / 2 ||
					neighboursCenters[i].z() < node->Center().z() - node->Size() / 2 ||
					neighboursCenters[i].x() > node->Center().x() + node->Size() / 2 ||
					neighboursCenters[i].y() > node->Center().y() + node->Size() / 2 ||
					neighboursCenters[i].z() > node->Center().z() + node->Size() / 2
					)
				{
					surface_area = surface_area + pow(node->size, 2);
					continue;
				}

				for (auto neighbour : node->parent->children)
				{
					if (neighbour != node)
					{
						if (neighbour->Type() == EMPTY) {
							surface_area = surface_area + pow(node->size, 2);
							continue;
						}
						else if (neighbour->Type() == PARTIAL) {
							if (node->children.size() == 0)
							{
								node->Subdivide();
								for (auto c : node->children)
								{
									c->SetType(FILLED);
								}
							}

							for (auto c : node->children)
							{
								stack.push(c);
							}
						}
					}
				}
			}
			
		}

		if (node->children.size() > 0)
		{
			for (int i = 0; i < 8; i++)
			{
				stack.push(node->children[i]);
			}
		}
	}

	return surface_area;
}

void ScaleOct(shared_ptr<Voxel> octree, float scale_factor) {
	stack<shared_ptr<Voxel>> stack;

	stack.push(octree);

	point3 scale_point = octree->center;

	while (!stack.empty())
	{
		auto node = stack.top();

		stack.pop();
		
		vec3 v_center_center = node->center - scale_point;
		v_center_center = scale_factor * v_center_center;

		node->size = scale_factor * node->size;
		node->center = scale_point + v_center_center;

		if (node->children.size() > 0)
		{
			for (int i = 0; i < 8; i++)
			{
				stack.push(node->children[i]);
			}
		}
	}
}

void TranslateOct(shared_ptr<Voxel> octree, float dx, float dy, float dz) {
	stack<shared_ptr<Voxel>> stack;

	stack.push(octree);

	while (!stack.empty())
	{
		auto node = stack.top();

		stack.pop();

		node->center.setX(node->center.x() + dx);
		node->center.setY(node->center.y() + dy);
		node->center.setZ(node->center.z() + dz);

		if (node->children.size() > 0)
		{
			for (int i = 0; i < 8; i++)
			{
				stack.push(node->children[i]);
			}
		}
	}
}

//// left neighbour
//int indexLeftNeighbour = (node->indexRelativeToFather) - 4;
//
//if (indexLeftNeighbour > 0) {
//	shared_ptr<Voxel> ln = node->parent->children[indexLeftNeighbour];
//
//	if (ln->Type() == EMPTY)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//else {
//	shared_ptr<Voxel> grandFather = node->parent->parent;
//	shared_ptr<Voxel> childTest = node->parent;
//
//	while (grandFather != nullptr)
//	{
//		int nextLeftNeighbour = (childTest->indexRelativeToFather) - 4;
//
//		if (indexLeftNeighbour > 0) {
//			shared_ptr<Voxel> ln = grandFather->children[nextLeftNeighbour];
//
//			if (ln->Type() == EMPTY)
//			{
//				surface_area += node->Size() * node->Size();
//
//				break;
//			}
//		}
//		else {
//			childTest = grandFather;
//			grandFather = grandFather->parent;
//		}
//	}
//
//	if (grandFather == nullptr)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//
//// right neighbour
//int indexRightNeighbour = (node->indexRelativeToFather + 1) + 4;
//
//if (indexLeftNeighbour < 8) {
//	shared_ptr<Voxel> ln = node->parent->children[indexLeftNeighbour];
//
//	if (ln->Type() == EMPTY)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//else {
//	shared_ptr<Voxel> grandFather = node->parent->parent;
//	shared_ptr<Voxel> childTest = node->parent;
//
//	while (grandFather != nullptr)
//	{
//		int nextRightNeighbour = (childTest->indexRelativeToFather + 1) + 4;
//
//		if (indexLeftNeighbour < 8) {
//			shared_ptr<Voxel> ln = grandFather->children[nextRightNeighbour];
//
//			if (ln->Type() == EMPTY)
//			{
//				surface_area += node->Size() * node->Size();
//
//				break;
//			}
//		}
//		else {
//			childTest = grandFather;
//			grandFather = grandFather->parent;
//		}
//	}
//
//	if (grandFather == nullptr)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//
//// Front neighbour
//int indexFrontNeighbour = (node->indexRelativeToFather + 1);
//
//if (node->indexRelativeToFather == 3 || node->indexRelativeToFather == 1 || node->indexRelativeToFather == 5)
//{
//	indexFrontNeighbour = 8;
//}
//
//if (indexLeftNeighbour < 8) {
//	shared_ptr<Voxel> ln = node->parent->children[indexLeftNeighbour];
//
//	if (ln->Type() == EMPTY)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//else {
//	shared_ptr<Voxel> grandFather = node->parent->parent;
//	shared_ptr<Voxel> childTest = node->parent;
//
//	while (grandFather != nullptr)
//	{
//		int nextFrontNeighbour = (childTest->indexRelativeToFather + 1);
//
//		if (childTest->indexRelativeToFather == 3 || childTest->indexRelativeToFather == 1 || childTest->indexRelativeToFather == 5)
//		{
//			nextFrontNeighbour = 8;
//		}
//
//		if (indexLeftNeighbour < 8) {
//			shared_ptr<Voxel> ln = grandFather->children[nextFrontNeighbour];
//
//			if (ln->Type() == EMPTY)
//			{
//				surface_area += node->Size() * node->Size();
//
//				break;
//			}
//		}
//		else {
//			childTest = grandFather;
//			grandFather = grandFather->parent;
//		}
//	}
//
//	if (grandFather == nullptr)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//
//// Back neighbour
//int indexBackNeighbour = (node->indexRelativeToFather - 1);
//
//if (node->indexRelativeToFather == 4 || node->indexRelativeToFather == 6 || node->indexRelativeToFather == 2)
//{
//	indexFrontNeighbour = -1;
//}
//
//if (indexLeftNeighbour > 0) {
//	shared_ptr<Voxel> ln = node->parent->children[indexLeftNeighbour];
//
//	if (ln->Type() == EMPTY)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//else {
//	shared_ptr<Voxel> grandFather = node->parent->parent;
//	shared_ptr<Voxel> childTest = node->parent;
//
//	while (grandFather != nullptr)
//	{
//		int nextFrontNeighbour = (childTest->indexRelativeToFather - 1);
//
//		if (childTest->indexRelativeToFather == 4 || childTest->indexRelativeToFather == 6 || childTest->indexRelativeToFather == 2)
//		{
//			nextFrontNeighbour = -1;
//		}
//
//		if (indexLeftNeighbour > 0) {
//			shared_ptr<Voxel> ln = grandFather->children[nextFrontNeighbour];
//
//			if (ln->Type() == EMPTY)
//			{
//				surface_area += node->Size() * node->Size();
//
//				break;
//			}
//		}
//		else {
//			childTest = grandFather;
//			grandFather = grandFather->parent;
//		}
//	}
//
//	if (grandFather == nullptr)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//
//// Up neighbour
//int indexBackNeighbour = (node->indexRelativeToFather + 2);
//
//if (node->indexRelativeToFather == 3 || node->indexRelativeToFather == 2)
//{
//	indexFrontNeighbour = 8;
//}
//
//if (indexLeftNeighbour < 8) {
//	shared_ptr<Voxel> ln = node->parent->children[indexLeftNeighbour];
//
//	if (ln->Type() == EMPTY)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//else {
//	shared_ptr<Voxel> grandFather = node->parent->parent;
//	shared_ptr<Voxel> childTest = node->parent;
//
//	while (grandFather != nullptr)
//	{
//		int nextFrontNeighbour = (childTest->indexRelativeToFather + 2);
//
//		if (childTest->indexRelativeToFather == 3 || childTest->indexRelativeToFather == 2)
//		{
//			nextFrontNeighbour = 8;
//		}
//
//		if (indexLeftNeighbour < 8) {
//			shared_ptr<Voxel> ln = grandFather->children[nextFrontNeighbour];
//
//			if (ln->Type() == EMPTY)
//			{
//				surface_area += node->Size() * node->Size();
//
//				break;
//			}
//		}
//		else {
//			childTest = grandFather;
//			grandFather = grandFather->parent;
//		}
//	}
//
//	if (grandFather == nullptr)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//
//// Bottom neighbour
//int indexBackNeighbour = (node->indexRelativeToFather - 2);
//
//if (node->indexRelativeToFather == 4 || node->indexRelativeToFather == 5)
//{
//	indexFrontNeighbour = -1;
//}
//
//if (indexLeftNeighbour > 0) {
//	shared_ptr<Voxel> ln = node->parent->children[indexLeftNeighbour];
//
//	if (ln->Type() == EMPTY)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}
//else {
//	shared_ptr<Voxel> grandFather = node->parent->parent;
//	shared_ptr<Voxel> childTest = node->parent;
//
//	while (grandFather != nullptr)
//	{
//		int nextFrontNeighbour = (childTest->indexRelativeToFather - 2);
//
//		if (childTest->indexRelativeToFather == 4 || childTest->indexRelativeToFather == 5)
//		{
//			nextFrontNeighbour = -1;
//		}
//
//		if (indexLeftNeighbour > 0) {
//			shared_ptr<Voxel> ln = grandFather->children[nextFrontNeighbour];
//
//			if (ln->Type() == EMPTY)
//			{
//				surface_area += node->Size() * node->Size();
//
//				break;
//			}
//		}
//		else {
//			childTest = grandFather;
//			grandFather = grandFather->parent;
//		}
//	}
//
//	if (grandFather == nullptr)
//	{
//		surface_area += node->Size() * node->Size();
//	}
//}


void WriteFromTraverse(shared_ptr<Voxel> octree, std::ofstream& write_file) {
	stack<shared_ptr<Voxel>> stack;

	write_file << std::fixed << std::setprecision(3);

	write_file << octree->depth << "\n";
	write_file << octree->Center().x() << "\n";
	write_file << octree->Center().y() << "\n";
	write_file << octree->Center().z() << "\n";
	write_file << octree->Size() << "\n";

	stack.push(octree);

	while (!stack.empty())
	{
		auto node = stack.top();

		stack.pop();

		switch (node->Type())
		{
		case FILLED:
			write_file << "B ";
			break;
		case PARTIAL:
			write_file << "( ";
			break;
		case EMPTY:
			write_file << "W ";
			break;
		default:
			break;
		}

		if (node->children.size() > 0)
		{
			for (int i = 0; i < 8; i++)
			{
				stack.push(node->children[i]);
			}
		}
	}
}

shared_ptr<Voxel> ReadFromFile(std::ifstream& file) {
	stack<shared_ptr<Voxel>> stack;

	std::string stepLine;
	std::stringstream stream;
	int depth = 0;
	double x, y, z, size;
	
	std::getline(file, stepLine);
	stream << stepLine;
	stream >> depth;
	stream.clear();

	std::getline(file, stepLine);
	stream << stepLine;
	stream >> x;
	stream.clear();

	std::getline(file, stepLine);
	stream << stepLine;
	stream >> y;
	stream.clear();

	std::getline(file, stepLine);
	stream << stepLine;
	stream >> z;
	stream.clear();

	std::getline(file, stepLine);
	stream << stepLine;
	stream >> size;
	stream.clear();

	auto red = make_shared<Lambertian>(color(.65, .05, .05));
	auto octree = make_shared<Voxel>(size, point3{ x, y, z }, red);
	octree->depth = depth;

	stack.push(octree);

	std::getline(file, stepLine);
	std::stringstream stream1;
	stream1 << stepLine;

	while (!stack.empty())
	{
		auto node = stack.top();
		stack.pop();

		char delimiter = NULL;

		stream1 >> delimiter;

		switch (delimiter)
		{
		case 'B':
			node->SetType(FILLED);
			break;
		case '(':
			node->SetType(PARTIAL);
			if (octree->depth - node->depth < depth)
			{
				node->Subdivide();

				for (int i = 0; i < 8; i++)
				{
					node->children[i]->depth = node->depth - 1;
					stack.push(node->children[i]);
				}
			}
			break;
		case 'W':
			node->SetType(EMPTY);
			break;
		default:
			break;
		}
	}

	std::ofstream out_octree("29_09_TREE_test_.txt");

	WriteFromTraverse(octree, out_octree);

	return octree;
}