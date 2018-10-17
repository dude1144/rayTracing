//Austin Anderson
//010640955
#pragma once
#include "ofMain.h"
#include "sceneObject.h"

class Light : public SceneObject
{
public:
	float intensity; // light intensity, color, and direction(in case directionated lights will be added later)
	glm::vec3 direction;

	ofxGuiGroup positionGroup;
	ofxInputField<float> xInput;
	ofxInputField<float> yInput;
	ofxInputField<float> zInput;
	ofxInputField<float> intensityInput;
	ofxColorSlider colorInput;

	Light() 
	{ 
		intensity = 1; 
		mat.diffuseColor = ofColor(255, 255, 255);

		settings.setup("Light");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(intensityInput.setup("Intensity", intensity, 0, std::numeric_limits<float>::max()));
		settings.add(colorInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	}

	Light(glm::vec3 pos, glm::vec3 dir, float i, ofColor col) 
	{
		intensity = i;
		mat.diffuseColor = col; 
		direction = dir; 
		position = pos; 

		settings.setup("Light");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(intensityInput.setup("Intensity", intensity, 0, std::numeric_limits<float>::max()));
		settings.add(colorInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	}

};

class PointLight : public Light //light that is just a point that shines in all directions
{
public:
	PointLight(glm::vec3 pos, float i, ofColor col) 
	{ 
		intensity = i; 
		mat.diffuseColor = col; 
		position = pos; 

		settings.setup("Light");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(intensityInput.setup("Intensity", intensity, 0, std::numeric_limits<float>::max()));
		settings.add(colorInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	}
	void draw() { ofDrawSphere(position, .1); }

	void updateFromUI()
	{
		position = glm::vec3((float)xInput, (float)yInput, (float)zInput);
		intensity = intensityInput;
		mat.diffuseColor = colorInput;
	}

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return glm::intersectRaySphere(ray.p, ray.d, position, .1f, point, normal); }
	bool intersectView(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return glm::intersectRaySphere(ray.p, ray.d, position, .1f, point, normal); }
};