#pragma once

#include "ofMain.h"

class OrientedBoundingBox;
class AxisAlignedBoundingBox;

//virtual class, marks object as intersectable and requires implementation of intersect methods
class Intersectable
{
public:
	virtual bool intersect(const OrientedBoundingBox& box) { return false; }
	virtual bool intersect(const AxisAlignedBoundingBox& box) { return false; }
	virtual float intersect(glm::vec3 point, glm::vec3 dir) { return false; }
};

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
		center = (max + min) / 2;
		axes[0] = glm::vec3(1, 0, 0);
		axes[1] = glm::vec3(0, 1, 0);
		axes[2] = glm::vec3(0, 0, 1);

		extents[0] = abs(max.x - center.x);
		extents[1] = abs(max.y - center.y);
		extents[2] = abs(max.z - center.z);
	}

	void draw();
	bool intersectOBB(OrientedBoundingBox *box) const;
	bool intersectTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const;
	bool intersectSphere(glm::vec3 p1, float radius) const;
	bool intersectPlane(glm::vec3 p1, glm::vec3 normal) const;
	bool intersectRay(glm::vec3 point, glm::vec3 dir) const;
	bool contains(glm::vec3 p1) const;

	bool intersect(Intersectable* toIntersect) const;
};

class AxisAlignedBoundingBox 
{
public:
	glm::vec3 min;
	glm::vec3 max;

	AxisAlignedBoundingBox(glm::vec3 min, glm::vec3 max)
	{
		this->min = min;
		this->max = max;
	}

	glm::vec3 center() { return (this->min + this->max) / 2; }

	void draw();
	bool intersectOBB(OrientedBoundingBox *box) const;
	bool intersectTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const;
	bool intersectSphere(glm::vec3 p1, float radius) const;
	bool intersectPlane(glm::vec3 p1, glm::vec3 normal) const;
	bool intersectRay(glm::vec3 point, glm::vec3 dir) const;
	bool contains(glm::vec3 p1) const;
};