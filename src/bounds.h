#pragma once

#include "ofMain.h"
#include "primitives.h"
#include "light.h"

class boundingBox
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;



	glm::mat4 getRotateMatrix()
	{
		return (glm::eulerAngleYXZ(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f)));
	}
	glm::mat4 getTranslateMatrix()
	{
		return (glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)));
	}
	glm::mat4 getScaleMatrix()
	{
		return (glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 1)));
	}

	glm::mat4 getMatrix()
	{
		glm::mat4 scale = getScaleMatrix();
		glm::mat4 rotate = getRotateMatrix();
		glm::mat4 trans = getTranslateMatrix();

		return (trans * rotate * scale);

	}
};

class OrientedBoundingBox
{
public:
	glm::vec3 center;
	glm::vec3 axes[3];
	float extents[3];

	OrientedBoundingBox()
	{
		this->center = glm::vec3(0,0,0);
		this->axes[0] = glm::normalize(glm::vec3(1,0,0));
		this->axes[1] = glm::normalize(glm::vec3(0,1,0));
		this->axes[2] = glm::normalize(glm::vec3(0,0,1));
		this->extents[0] = 1;
		this->extents[1] = 1;
		this->extents[2] = 1;
	}

	OrientedBoundingBox(glm::vec3 center, glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis, float xExtent, float yExtent, float zExtent)
	{
		this->center = center;
		this->axes[0] = glm::normalize(xAxis);
		this->axes[1] = glm::normalize(yAxis);
		this->axes[2] = glm::normalize(zAxis);
		this->extents[0] = xExtent;
		this->extents[1] = yExtent;
		this->extents[2] = zExtent;
	}
	OrientedBoundingBox(glm::vec3 min, glm::vec3 max)
	{
		center = (max - min) / 2;
		axes[0] = glm::vec3(1, 0, 0);
		axes[1] = glm::vec3(0, 1, 0);
		axes[2] = glm::vec3(0, 0, 1);

		extents[0] = abs(max.x - center.x);
		extents[1] = abs(max.y - center.y);
		extents[2] = abs(max.z - center.z);
	}

	void draw();
	bool intersect(OrientedBoundingBox *box);
	bool intersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	bool intersect(glm::vec3 p1, float radius);
	bool intersect(glm::vec3 p1, glm::vec3 normal);
	
	bool contains(glm::vec3 p1);

	bool intersect(SceneObject* object);
	bool intersect(Sphere* sphere);
	bool intersect(Plane* plane);
	bool intersect(Mesh* mesh);
	bool intersect(Light* light);
};

class AxisAlignedBoundingBox 
{
public:
	glm::vec3 p1;
	glm::vec3 p2;

	AxisAlignedBoundingBox(glm::vec3 p1, glm::vec3 p2)
	{
		this->p1 = p1;
		this->p2 = p2;
	}

	bool intersect(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
	{

	}

	bool intersect(AxisAlignedBoundingBox *box)
	{
		glm::vec3 center = (p1 + p2) / 2;
		glm::vec3 boxCenter = (box->p1 + box->p2) / 2;

		float widthx = abs(p1.x - center.x);
		float widthy = abs(p1.y - center.y);
		float widthz = abs(p1.z - center.z);
		float boxwidthx = abs(box->p1.x - boxCenter.x);
		float boxwidthy = abs(box->p1.y - boxCenter.y);
		float boxwidthz = abs(box->p1.z - boxCenter.z);

		if ((center.x - boxCenter.x) > (widthx + boxwidthx))
			return false;
		if ((center.y - boxCenter.y) > (widthy + boxwidthy))
			return false;
		if ((center.z - boxCenter.z) > (widthz + boxwidthz))
			return false;

		return true;
	}
};