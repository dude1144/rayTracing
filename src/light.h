//Austin Anderson
//010640955
#pragma once
#include "primitives.h"

class Light : public SceneObject
{
public:
	float intensity; // light intensity, color, and direction(in case directionated lights will be added later)
	glm::vec3 direction;

	//ofxInputField<float> intensityInput;
	//ofxColorSlider colorInput;

	~Light()
	{

		//intensityInput.unregisterMouseEvents();
		//colorInput.unregisterMouseEvents();
	}
	Light() 
	{ 
		intensity = 1; 
		mat.diffuseColor = ofColor(255, 255, 255);
	}

	Light(glm::vec3 pos, glm::vec3 dir, float i, ofColor col) 
	{
		intensity = i;
		mat.diffuseColor = col; 
		direction = dir; 
		position = pos; 
	}
};

class PointLight : public Light //light that is just a point that shines in all directions
{
public:
	~PointLight()
	{
		//intensityInput.unregisterMouseEvents();
		//colorInput.unregisterMouseEvents();
	}
	PointLight()
	{
		intensity = 10;
		mat.diffuseColor = ofColor::white;
		position = glm::vec3(0,0,0);
	}
	PointLight(glm::vec3 pos, float i, ofColor col) 
	{ 
		intensity = i; 
		mat.diffuseColor = col; 
		position = pos;
	}
	void draw() { ofDrawSphere(position, .1); }

	bool intersect(const Ray &ray, IntersectInfo &intersect) 
	{ 
		if (glm::intersectRaySphere(ray.point, ray.dir, position, .1f, intersect.point, intersect.normal))
		{
			intersect.dist = glm::length(intersect.point - ray.point);
			return true;
		}
		return false;
	}
	bool intersectView(const Ray &ray, IntersectInfo &intersect) 
	{
		if (glm::intersectRaySphere(ray.point, ray.dir, position, .1f, intersect.point, intersect.normal))
		{
			intersect.dist = glm::length(intersect.point - ray.point);
			return true;
		}
		return false;
	}
};