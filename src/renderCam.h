//Austin Anderson
//010640955
//from code given by professor
#pragma once

#include "sceneObject.h"
#include <vector>
#include "viewPlane.h"
#include "light.h"

//  render camera  - currently must be z axis aligned (we will improve this in project 4)
//
class RenderCam : public SceneObject
{
public:
	ofColor ambientColor;     //color and intensity of ambient light
	float   ambientIntensity;

	RenderCam()
	{
		ambientColor = ofColor(255, 255, 255);
		ambientIntensity = .05;
		position = glm::vec3(0, 0, 10);
		aim = glm::vec3(0, 0, -1);

		settings.setup();
	}

	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	void drawFrustum();
	glm::vec3 screenToWorld(float u, float v) { return view.toWorld(u, v); }

	void renderImage(vector<SceneObject*> objects, ofImage *image, vector<Light*> lights);       //render the image

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render
	void updateFromUI()
	{
		return;
	}

private:
	ofColor lambertian(SceneObject *obj, Light *light, glm::vec3 point, glm::vec3 normal);       //calculate lambertian color
	ofColor blinn_phong(SceneObject *obj, Light *light, glm::vec3 point, glm::vec3 normal);      //calculate Blinn-Phong color
	bool inShadow(vector<SceneObject*> objects, Light *light, glm::vec3 point, glm::vec3 normal);//determine if point is in a shadow
};