#pragma once
#include "sceneObject.h"

class Sphere : public SceneObject
{
public:
	float radius = 1.0;

	Sphere() { position = glm::vec3(0, 0, 0), 1, diffuseColor = ofColor::white; };
	Sphere(glm::vec3 pos, float r, ofColor col) { position = pos; radius = r; diffuseColor = col; }

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) 
	{
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
		return true;
	}
	void draw() 
	{
		ofDrawSphere(position, radius);
	}
};
