#pragma once
#include "ofMain.h"
#include "sceneObject.h"
#include "ray.h"

//  General purpose plane 
//
class Plane : public SceneObject 
{
public:
	ofPlanePrimitive plane;
	float width = 20;
	float height = 20;
	glm::vec3 normal = glm::vec3(0, 1, 0);

	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse =ofColor::darkOliveGreen, float w = 20, float h = 20) 
	{
		position = p; normal = n;
		width = w;
		height = h;
		mat.diffuseColor = diffuse;
		plane.rotateDeg(90, 1, 0, 0);
	}
	Plane(glm::vec3 pos, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, ofColor spec = ofColor::olive, float p = 1, float w = 20, float h = 20)
	{
		position = pos; normal = n;
		mat.p = p;
		width = w;
		height = h;
		mat.diffuseColor = diffuse;
		mat.specularColor = spec;
		plane.rotateDeg(90, 1, 0, 0);
	}
	Plane() { }

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);

	void draw() 
	{
		ofSetColor(mat.diffuseColor);
		plane.setPosition(position);
		plane.setWidth(width);
		plane.setHeight(height);
		plane.setResolution(4, 4);
		plane.drawWireframe();
	}

};