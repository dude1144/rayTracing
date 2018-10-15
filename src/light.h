//Austin Anderson
//010640955
#pragma once
#include "ofMain.h"
#include "sceneObject.h"

class Light : public SceneObject
{
public:
	float intensity; // light intensity, color, and direction(in case directionated lights will be added later)
	ofColor color;
	glm::vec3 direction;

	Light() { intensity = 1; color = ofColor(255, 255, 255); }
	Light(float i, ofColor col) { intensity = i; color = col; direction = glm::vec3(0, 0, 0); position = glm::vec3(0, 0, 0); }
	Light(float i, ofColor col, glm::vec3 dir) { intensity = i; color = col; direction = dir; }
	Light(glm::vec3 pos, glm::vec3 dir, float i, ofColor col) { intensity = i; color = col; direction = dir; position = pos; }

};

class PointLight : public Light //light that is just a point that shines in all directions
{
public:
	PointLight(glm::vec3 pos, glm::vec3 dir, float i, ofColor col) { intensity = i; color = col; direction = dir; position = pos; }
	void draw() { ofDrawSphere(position, .1); }
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return glm::intersectRaySphere(ray.p, ray.d, position, .1f, point, normal); }

};