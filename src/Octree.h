#pragma once

#include <vector>
#include "bounds.h"

template<class T>

class Octree
{
public:
	Octree octands[8];
	OrientedBoundingBox bounds;

	Octree(glm::vec3 min, glm::vec3 max)
	{
		this.bounds = OrientedBoundingBox(min, max);

		glm::vec3 center = (min + max) / 2;

		glm::vec3 points[8];

		points[0] = max;
		points[1] = glm::vec3(min.x, max.y, max.z);
		points[2] = glm::vec3(min.x, max.y, max.z);
		points[3] = glm::vec3(max.x, max.y, min.z);
		points[4] = glm::vec3(max.x, min.y, max.z);
		points[5] = glm::vec3(min.x, min.y, max.z);
		points[6] = min;
		points[7] = glm::vec3(max.x, min.y, min.z);


		for (int i = 0; i < 8; i++)
		{
			//UNFINISHED
		}
	}

};