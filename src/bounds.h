#pragma once

#include "ofMain.h"

/*class Bounds
{
public:
	glm::vec3 position;

	virtual bool intersect(BoundingBox bound) { return false; }
	virtual void draw() = 0;
};*/

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
};

class AxisAlignedBoundingBox 
{
	glm::vec3 position;
	float xSize;
	float ySize;
	float zSize;


};