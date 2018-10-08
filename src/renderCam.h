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
	ofColor ambientColor;
	float   ambientIntensity;

	RenderCam() 
	{
		ambientColor = ofColor(255, 255, 255);
		ambientIntensity = .05;
		position = glm::vec3(0, 0, 10);
		aim = glm::vec3(0, 0, -1);
	}
	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	void drawFrustum();

	void renderImage(vector<SceneObject*> objects, ofImage *image, vector<Light*> lights);

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
};