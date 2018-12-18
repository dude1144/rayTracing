//Austin Anderson
//010640955
#pragma once
#include "ofMain.h"
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

		this->setupUI();
	}

	Light(glm::vec3 pos, glm::vec3 dir, float i, ofColor col) 
	{
		intensity = i;
		mat.diffuseColor = col; 
		direction = dir; 
		position = pos; 

		this->setupUI();
	}

	void updateFromUI()
	{
		position = glm::vec3((float)xInput, (float)yInput, (float)zInput);
		mat.p = mat.pInput;
		intensity = mat.pInput;
		mat.diffuseColor = mat.diffuseInput;
		mat.specularColor = mat.specularInput;

	}

	void setupUI()
	{
		settings.setup("Light");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(&mat.materialGroup);
		this->updateFromUI();
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

		this->setupUI();
	}
	PointLight(glm::vec3 pos, float i, ofColor col) 
	{ 
		intensity = i; 
		mat.diffuseColor = col; 
		position = pos; 

		this->setupUI();
	}
	void draw() { ofDrawSphere(position, .1); }

	void updateFromUI()
	{
		position = glm::vec3((float)xInput, (float)yInput, (float)zInput);
		mat.p = mat.pInput;
		intensity = mat.pInput;
		mat.diffuseColor = mat.diffuseInput;
		mat.specularColor = mat.specularInput;

	}

	void setupUI()
	{
		settings.setup("Light");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(&mat.materialGroup);
		this->updateFromUI();
	}

	bool intersect(const Ray &ray, IntersectInfo &intersect) 
	{ 
		if (glm::intersectRaySphere(ray.p, ray.d, position, .1f, intersect.point, intersect.normal))
		{
			intersect.dist = glm::length(intersect.point - ray.p);
			return true;
		}
		return false;
	}
	bool intersectView(const Ray &ray, IntersectInfo &intersect) 
	{
		if (glm::intersectRaySphere(ray.p, ray.d, position, .1f, intersect.point, intersect.normal))
		{
			intersect.dist = glm::length(intersect.point - ray.p);
			return true;
		}
		return false;
	}
};