#pragma once

#include <vector>
#include "bounds.h"

#define MAX_LEAF_OBJECTS 5

class Octree
{
private:
	class OctreeNode
	{
	public:
		OctreeNode* octants[8];
		OctreeNode* parent;
		OrientedBoundingBox bounds;
		vector<Intersectable*> objects;
		glm::vec3 min;
		glm::vec3 max;
		bool isLeaf;

		OctreeNode()
		{
		}

		OctreeNode(glm::vec3 min, glm::vec3 max, OctreeNode* parent)
		{
			this->min = glm::vec3(std::min(min.x, max.x), std::min(min.y, max.y), std::max(min.z, max.z));
			this->max = glm::vec3(std::max(min.x, max.x), std::max(min.y, max.y), std::min(min.z, max.z));

			this->parent = parent;

			this->bounds = OrientedBoundingBox(this->min, this->max);

			for (int i = 0; i < 8; i++)
				octants[i] = nullptr;

			isLeaf = true;
		}

		void subdivide()
		{
			glm::vec3 points[8];
			glm::vec3 center = (this->min + this->max) / 2;

			//calculate the 8 corners of the octant
			points[0] = this->max;
			points[1] = glm::vec3(this->min.x, this->max.y, this->max.z);
			points[2] = glm::vec3(this->min.x, this->max.y, this->min.z);
			points[3] = glm::vec3(this->max.x, this->max.y, this->min.z);
			points[4] = glm::vec3(this->max.x, this->min.y, this->max.z);
			points[5] = glm::vec3(this->min.x, this->min.y, this->max.z);
			points[6] = this->min;
			points[7] = glm::vec3(this->max.x, this->min.y, this->min.z);

			//create a new octant and add it to this node's octant array, then eval it
			for (int i = 0; i < 8; i++)
				this->octants[i] = new OctreeNode(center, points[i], this);

			this->isLeaf = false;
		}

		void intersect(glm::vec3 point, glm::vec3 dir, Intersectable** intersect, float* dist)
		{
			//check if the ray intersects this node
			if (this->bounds.intersectRay(point, dir))
			{
				//if this is a leaf node, check all objects in this node
				if (this->isLeaf)
				{
					for (int i = 0; i < this->objects.size(); i++)
					{
						//check for intersection and save the distance
						float d = this->objects[i]->intersect(point, dir);
						if (d >= 0)
						{
							//if this object is closer or no objects saved yet, save object and distance
							if (intersect && d < *dist)
							{
								*intersect = objects[i];
								*dist = d;
							}
							else
							{
								*intersect = objects[i];
								*dist = d;
							}
						}
					}
				}
				//otherwise, intersect all this node's octants
				else
				{
					for (int i = 0; i < 8; i++)
						if (octants[i])
							octants[i]->intersect(point, dir, intersect, dist);
				}
			}
		}

		void draw(bool onlyLeafNodes)
		{
			if (!onlyLeafNodes || objects.size() != 0)
				bounds.draw();
			if (!this->isLeaf)
				for (int i = 0; i < 8; i++)
				{
					if (this->octants[i] != NULL)
						this->octants[i]->draw(onlyLeafNodes);
				}
		}

		void evalRecursive(vector<Intersectable*> scene, int currentLevel, int maxLevels)
		{
			//add any intersectables that intersect to the list
			for (int i = 0; i < scene.size(); i++)
				if (this->bounds.intersect(scene[i]))
					this->objects.push_back(scene[i]);

			if (objects.size() > 1 && currentLevel < maxLevels)
			{
				this->subdivide();

				for (int i = 0; i < 8; i++)
					this->octants[i]->evalRecursive(this->objects, ++currentLevel, maxLevels);

				this->objects.clear();
			}
		}

		void clean()
		{
			//recursively clear all octants
			for (int i = 0; i < 8; i++)
			{
				if (octants[i])
					octants[i]->clean();
			}

			//once this node's octants have been cleaned, delete empty ones from memory
			for (int i = 0; i < 8; i++)
			{
				if (octants[i] && octants[i]->objects.size() == 0 && octants[i]->isLeaf)
				{
					delete octants[i];
					octants[i] = nullptr;
				}
			}
		}

		void clear()
		{
			//recursively clear all octants
			for (int i = 0; i < 8; i++)
			{
				if (octants[i])
					octants[i]->clear();
			}

			//once this node's octants have been cleared, delete them from memory
			for (int i = 0; i < 8; i++)
			{
				if (octants[i])
				{
					delete octants[i];
					octants[i] = nullptr;
				}
			}
			
			//clear this node's Intersectable vector
			objects.clear();
		}
	};

	OctreeNode root;
	int maxLevels;
public:
	
	Octree()
	{
		root = OctreeNode(glm::vec3(-10, -10, 10), glm::vec3(10, 10, -10), nullptr);
		maxLevels = 5;
	}
	Octree(glm::vec3 min, glm::vec3 max)
	{
		root = OctreeNode(min, max, nullptr);
		maxLevels = 5;
	}

	int getMaxLevels() { return maxLevels; }
	void setMaxLevels(int x) { maxLevels = x; }

	Intersectable* intersect(glm::vec3 point, glm::vec3 dir)
	{
		Intersectable* inter = nullptr;
		Intersectable** intersect = &inter;
		float d = std::numeric_limits<float>::max();

		root.intersect(point, dir, intersect, &d);

		return *intersect;
	}

	void draw(bool onlyLeafNodes = true)
	{
		root.draw(onlyLeafNodes);
	}

	void clear()
	{
		root.clear();
	}

	void subdivide()
	{
		subdivide(&root);
	}

	void subdivide(OctreeNode* node)
	{
		if (!node->isLeaf)
		{
			for (int i = 0; i < 8; i++)
				if(node->octants[i] != NULL)
					subdivide(node->octants[i]);
		}
		else
			node->subdivide();
	}

	void eval(vector<Intersectable*> scene)
	{
		this->clear();
		root.evalRecursive(scene, 0, maxLevels);
		root.clean();
	}	
};