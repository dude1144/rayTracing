#pragma once

#include <vector>
#include "bounds.h"

#define MAX_LEAF_OBJECTS 3

class Octree
{
private:
	class OctreeNode
	{
	public:
		OctreeNode* octants[8];
		OctreeNode* parent;
		OrientedBoundingBox bounds;
		vector<SceneObject*> objects;
		glm::vec3 min;
		glm::vec3 max;
		bool isLeaf;

		OctreeNode()
		{
		}

		OctreeNode(glm::vec3 min, glm::vec3 max, OctreeNode* parent)
		{

			this->min.x = std::min(min.x, max.x);
			this->min.y = std::min(min.y, max.y);
			this->min.z = std::max(min.z, max.z);

			this->max.x = std::max(min.x, max.x);
			this->max.y = std::max(min.y, max.y);
			this->max.z = std::min(min.z, max.z);

			this->parent = parent;

			this->bounds = OrientedBoundingBox(this->min, this->max);

			for (int i = 0; i < 8; i++)
				octants[i] = nullptr;

			isLeaf = true;
		}

		void draw()
		{
			if (objects.size() != 0)
				bounds.draw();
			if (!this->isLeaf)
				for (int i = 0; i < 8; i++)
				{
					if (this->octants[i] != NULL)
						this->octants[i]->draw();
				}
		}

		void eval(vector<SceneObject*> scene)
		{
			for (int i = 0; i < scene.size(); i++)
			{
				if (this->bounds.intersect(scene[i]))
					this->objects.push_back(scene[i]);
			}
		}

		void eval()
		{
			if(parent)
				this->eval(parent->objects);
		}

		void clear()
		{
			for (int i = 0; i < 8; i++)
			{
				if (octants[i])
					octants[i]->clear();
			}
			for (int i = 0; i < 8; i++)
			{
				if (octants[i])
				{
					delete octants[i];
					octants[i] = nullptr;
				}
			}

			objects.clear();
		}
	};

	OctreeNode root;

	void cleanup()
	{
		queue<OctreeNode*> toClean;
		toClean.push(&root);

		while (toClean.size() != 0)
		{
			OctreeNode* current = toClean.front();

			for (int i = 0; i < 8; i++)
			{
				if (current->octants[i] && current->octants[i]->objects.size() > 0)
				{
					toClean.push(current->octants[i]);
					current->objects.clear();
				}
				else
				{
					current->octants[i] = nullptr;
				}
			}
			toClean.pop();
		}
	}
public:
	
	Octree(glm::vec3 min, glm::vec3 max)
	{
		root = OctreeNode(min, max, nullptr);
	}

	void draw()
	{
		root.draw();
	}

	void clear()
	{
		root.clear();
	}

	void eval(vector<SceneObject*> scene)
	{
		this->clear();
		queue<OctreeNode*> toEval;

		toEval.push(&root);

		while (toEval.size() != 0)
		{
			OctreeNode* current = toEval.front();
			if (current->parent)
				current->eval();
			else
				current->eval(scene);

			if (current->objects.size() > MAX_LEAF_OBJECTS)
			{
				glm::vec3 points[8];
				glm::vec3 center = (current->min + current->max) / 2;
				points[0] = current->max;
				points[1] = glm::vec3(current->min.x, current->max.y, current->max.z);
				points[2] = glm::vec3(current->min.x, current->max.y, current->min.z);
				points[3] = glm::vec3(current->max.x, current->max.y, current->min.z);
				points[4] = glm::vec3(current->max.x, current->min.y, current->max.z);
				points[5] = glm::vec3(current->min.x, current->min.y, current->max.z);
				points[6] = current->min;
				points[7] = glm::vec3(current->max.x, current->min.y, current->min.z);


				for (int i = 0; i < 8; i++)
				{
					current->octants[i] = new OctreeNode(center, points[i], current);
					toEval.push(current->octants[i]);
				}
				current->isLeaf = false;
			}
			else
			{
				current->isLeaf = true;
			}

			toEval.pop();
		}

		this->cleanup();

	}
	
};