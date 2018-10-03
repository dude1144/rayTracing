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
	Light ambientLight;

	RenderCam() 
	{
		ambientLight = Light(1, ofColor::white);
		position = glm::vec3(0, 0, 10);
		aim = glm::vec3(0, 0, -1);
	}
	Ray getRay(float u, float v);
	void draw() { ofDrawBox(position, 1.0); };
	void drawFrustum();

	void renderImage(vector<SceneObject*> objects, ofImage *image);

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 
};