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
			total_volume += (node->Size() * node->Size());
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

void WriteFromTraverse(shared_ptr<Voxel> octree, std::ofstream& write_file) {
	stack<shared_ptr<Voxel>> stack;

	stack.push(octree);

	while (!stack.empty())
	{
		auto node = stack.top();

		stack.pop();

		switch (node->Type())
		{
		case FILLED:
			write_file << "B";
			break;
		case PARTIAL:
			write_file << "(";
			break;
		case EMPTY:
			write_file << "W";
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