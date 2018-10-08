#pragma once
#include "ofMain.h"
#include "ray.h"

class Light
{
public:
	float intensity;
	ofColor color;
	glm::vec3 direction, position;

	Light() { intensity = 1; color = ofColor(255, 255, 255); }
	Light(float i, ofColor col) { intensity = i; color = col; direction = glm::vec3(0, 0, 0); position = glm::vec3(0, 0, 0); }
	Light(float i, ofColor col, glm::vec3 dir) { intensity = i; color = col; direction = dir; }
	Light(glm::vec3 pos, glm::vec3 dir, float i, ofColor col) { intensity = i; color = col; direction = dir; position = pos; }

	virtual void draw(){}
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 pos, glm::vec3 dir, float i, ofColor col) { intensity = i; color = col; direction = dir; position = pos; }
	void draw() { ofDrawSphere(position, .1); Ray(position, direction).draw(.3); }

};