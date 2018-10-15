//Austin Anderson
//010640955
//from code given by professor
#pragma once
#include "sceneObject.h"

class Sphere : public SceneObject
{
public:
	float radius = 1.0;

	Sphere() { position = glm::vec3(0, 0, 0), 1, mat.diffuseColor = ofColor::white; };
	Sphere(glm::vec3 pos, float r, ofColor col) { position = pos; radius = r; mat.diffuseColor = col; }
	Sphere(glm::vec3 pos, float r, ofColor difCol, ofColor specCol, float p) { position = pos; radius = r; mat.diffuseColor = difCol; mat.specularColor = specCol; mat.p = p; }

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) 
	{
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}

	bool intersectView(const Ray &ray, glm::vec3 &point, glm::vec3 &normal)
	{
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	void draw() 
	{
		//ofSetColor(mat.diffuseColor);
		ofDrawSphere(position, radius);
	}
};
