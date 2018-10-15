//Austin Anderson
//010640955
//from code given by professor
#pragma once
#include "ofMain.h"
#include "ray.h"
#include "material.h"

class SceneObject 
{
public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	Material mat = Material(ofColor::gray, ofColor::lightGray, 1.0f);

	virtual void draw() = 0;    // pure virtual funcs - must be overloaded

	//point and normal save the point the ray intersects as well as the normal at that point
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return false; }
	virtual bool intersectView(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return false; }
};

