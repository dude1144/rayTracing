#pragma once
#include "ofMain.h"
#include "sceneObject.h"
#include "ray.h"

//  General purpose plane 
//
class Plane : public SceneObject 
{
public:
	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse =ofColor::darkOliveGreen, float w = 20, float h = 20) 
	{
		position = p; normal = n;
		width = w;
		height = h;
		diffuseColor = diffuse;
		plane.rotateDeg(90, 1, 0, 0);
	}
	Plane() { }
	glm::vec3 normal = glm::vec3(0, 1, 0);
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	void draw() 
	{
		plane.setPosition(position);
		plane.setWidth(width);
		plane.setHeight(height);
		plane.setResolution(4, 4);
		plane.drawWireframe();
	}
	ofPlanePrimitive plane;
	float width = 20;
	float height = 20;
};