#include "boolean_operations.h"

shared_ptr<Voxel> Union(shared_ptr<Voxel> octree1, shared_ptr<Voxel> octree2) {
	shared_ptr<Voxel> union_octree = make_shared<Voxel>(octree1->Size(), octree1->Center(), octree1->material_ptr);

	stack<shared_ptr<Voxel>> stack1;
	stack<shared_ptr<Voxel>> stack2;
	stack<shared_ptr<Voxel>> stack3;

	stack1.push(octree1);
	stack2.push(octree2);
	stack3.push(union_octree);

	while (!stack1.empty() && !stack2.empty())
	{
		auto node1 = stack1.top();
		auto node2 = stack2.top();
		auto node_union = stack3.top();

		stack1.pop();
		stack2.pop();
		stack3.pop();

		if (node1->Type() == FILLED || node2->Type() == FILLED)
		{
			node_union->SetType(FILLED);

			continue;
		}

		if (node1->Type() == PARTIAL && node2->Type() == EMPTY)
		{
			node_union->SetType(PARTIAL);
			node_union->children = node1->children;

			continue;
		}

		if (node1->Type() == EMPTY && node2->Type() == PARTIAL)
		{
			node_union->SetType(PARTIAL);
			node_union->children = node2->children;

			continue;
		}

		if (node1->Type() == PARTIAL && node2->Type() == PARTIAL)
		{
			node_union->SetType(PARTIAL);

			if (node1->children.size() > 0 && node2->children.size() > 0)
			{
				int fullChildrenCount = 0;

				for (int i = 0; i < 8; i++)
				{
					if (node1->children[i]->Type() == FILLED || node2->children[i]->Type() == FILLED)
					{
						fullChildrenCount++;
					}
				}

				if (fullChildrenCount == 8)
				{
					node_union->SetType(FILLED);

					continue;
				}

				node_union->Subdivide();
				for (int i = 0; i < 8; i++)
				{
					stack1.push(node1->children[i]);
					stack2.push(node2->children[i]);
					stack3.push(node_union->children[i]);
				}
			}
		}
	}

	return union_octree;
}

shared_ptr<Voxel> Intersection(shared_ptr<Voxel> octree1, shared_ptr<Voxel> octree2) {
	shared_ptr<Voxel> union_octree = make_shared<Voxel>(octree1->Size(), octree1->Center(), octree1->material_ptr);

	stack<shared_ptr<Voxel>> stack1;
	stack<shared_ptr<Voxel>> stack2;
	stack<shared_ptr<Voxel>> stack3;

	stack1.push(octree1);
	stack2.push(octree2);
	stack3.push(union_octree);

	while (!stack1.empty() && !stack2.empty())
	{
		auto node1 = stack1.top();
		auto node2 = stack2.top();
		auto node_union = stack3.top();

		stack1.pop();
		stack2.pop();
		stack3.pop();

		if (node1->Type() == EMPTY || node2->Type() == EMPTY)
		{
			node_union->SetType(EMPTY);

			continue;
		}

		if (node1->Type() == FILLED && node2->Type() == FILLED)
		{
			node_union->SetType(FILLED);

			continue;
		}

		if (node1->Type() == PARTIAL || node2->Type() == PARTIAL)
		{
			node_union->SetType(PARTIAL);

			if (node2->Type() == FILLED)
			{
				node2->Subdivide();

				for (auto child : node2->children)
				{
					child->SetType(FILLED);
				}
			}

			if (node1->Type() == FILLED)
			{
				node1->Subdivide();

				for (auto child : node1->children)
				{
					child->SetType(FILLED);
				}
			}

			if (node1->children.size() > 0 && node2->children.size() > 0)
			{
				node_union->Subdivide();
				for (int i = 0; i < 8; i++)
				{
					stack1.push(node1->children[i]);
					stack2.push(node2->children[i]);
					stack3.push(node_union->children[i]);
				}
			}
		}
	}

	return union_octree;
}

shared_ptr<Voxel> Difference(shared_ptr<Voxel> octree1, shared_ptr<Voxel> octree2) {
	shared_ptr<Voxel> diff_octree = make_shared<Voxel>(octree1->Size(), octree1->Center(), octree1->material_ptr);

	stack<shared_ptr<Voxel>> stack1;
	stack<shared_ptr<Voxel>> stack2;
	stack<shared_ptr<Voxel>> stack3;

	stack1.push(octree1);
	stack2.push(octree2);
	stack3.push(diff_octree);

	while (!stack1.empty() && !stack2.empty())
	{
		auto node1 = stack1.top();
		auto node2 = stack2.top();
		auto node_union = stack3.top();

		stack1.pop();
		stack2.pop();
		stack3.pop();

		if (node2->Type() == EMPTY)
		{
			if (node1->Type() == FILLED) {
				node_union->SetType(FILLED);
			}
			else if (node1->Type() == PARTIAL)
			{
				node_union->SetType(PARTIAL);
				node_union->children = node1->children;
			}

			continue;
		}

		if (node2->Type() == FILLED)
		{
			node_union->SetType(EMPTY);
			continue;
		}

		if (node2->Type() == PARTIAL)
		{
			if (node1->Type() == EMPTY) continue;

			if (node1->Type() == PARTIAL)
			{
				node_union->SetType(PARTIAL);

				if (node1->children.size() > 0 && node2->children.size() > 0)
				{
					node_union->Subdivide();
					for (int i = 0; i < 8; i++)
					{
						stack1.push(node1->children[i]);
						stack2.push(node2->children[i]);
						stack3.push(node_union->children[i]);
					}
				}
			}
			else {
				node1->Subdivide();
				node_union->SetType(PARTIAL);

				for (auto child : node1->children)
				{
					child->SetType(FILLED);
				}

				if (node1->children.size() > 0 && node2->children.size() > 0)
				{
					node_union->Subdivide();
					for (int i = 0; i < 8; i++)
					{
						stack1.push(node1->children[i]);
						stack2.push(node2->children[i]);
						stack3.push(node_union->children[i]);
					}
				}
			}
		}
	}

	return diff_octree;
}