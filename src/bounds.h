#pragma once

#include "ofMain.h"

/*class Bounds
{
public:
	glm::vec3 position;

	virtual bool intersect(BoundingBox bound) { return false; }
	virtual void draw() = 0;
};*/

/*
class BoundingBox// : public Bounds
{
public:
	glm::vec3 vert[8]; 

	BoundingBox(glm::vec3 topTopLeft, glm::vec3 topTopRight, glm::vec3 topBottomLeft, glm::vec3 topBottomRight,
		glm::vec3 bottomTopLeft, glm::vec3 bottomTopRight, glm::vec3 bottomBottomLeft, glm::vec3 bottomBottomRight)
	{
		vert[0] = topTopLeft;
		vert[1] = topTopRight;
		vert[2] = topBottomLeft;
		vert[3] = topBottomRight;
		vert[4] = bottomTopLeft;
		vert[5] = bottomTopRight;
		vert[6] = bottomBottomLeft;
		vert[7] = bottomBottomRight;
	}


	bool intersect(BoundingBox box)
	{
		//glm::proj()
		return false;
	}
	bool intersect(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
	{
		return false;
	}

	void draw()
	{
		//draw top face
		ofDrawLine(vert[0], vert[1]);
		ofDrawLine(vert[1], vert[3]);
		ofDrawLine(vert[0], vert[2]);
		ofDrawLine(vert[3], vert[2]);

		//draw bottom face
		ofDrawLine(vert[4], vert[5]);
		ofDrawLine(vert[5], vert[7]);
		ofDrawLine(vert[4], vert[6]);
		ofDrawLine(vert[7], vert[6]);

		//conntect them
		ofDrawLine(vert[0], vert[4]);
		ofDrawLine(vert[1], vert[5]);
		ofDrawLine(vert[2], vert[6]);
		ofDrawLine(vert[3], vert[7]);
	}
};*/

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
	glm::vec3 Center;
	glm::vec3 xAxis;
	glm::vec3 yAxis;
	glm::vec3 zAxis;
	float xExtent;
	float yExtent;
	float zEntent;

	OrientedBoundingBox(glm::vec3 Center, glm::vec3 xAxis, glm::vec3 yAxis, glm::vec3 zAxis, float xExtent, float yExtent, float zExtent)
	{
		this->Center = Center;
		this->xAxis = xAxis;
		this->yAxis = yAxis;
		this->zAxis = zAxis;
		this->xExtent = xExtent;
		this->yExtent = yExtent;
		this->zEntent = zEntent;
	}

	bool intersect(OrientedBoundingBox *box)
	{

	}
};

class AxisAlignedBoundingBox 
{
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