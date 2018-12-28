//Austin Anderson
//010640955
//from code given by professor
#pragma once

//#include "sceneObject.h"
#include "primitives.h"
#include "scene.h"
#include "viewPlane.h"
#include "light.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <future>
#include <atomic>


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
		ambientIntensity = 0.05;
		position = glm::vec3(0, 0, 10);
		aim = glm::vec3(0, 0, -1);

		settings.setup();
	}

	Ray getRay(float u, float v);
	void draw() 
	{ 
		ofDrawBox(position, 1.0); 
	}

	void drawFrustum();
	glm::vec3 screenToWorld(float u, float v) { return view.toWorld(u, v); }

	void renderImage(Scene scene, ofImage *image, bool antiAlias, bool multiThread);       //render the image

	glm::vec3 aim;
	ViewPlane view;          // The camera viewplane, this is the view that we will render
	void updateFromUI()
	{
		return;
	}

private:
	void renderImagePiece(Scene scene, ofImage *image, int startWidth, int startHeight, int endWidth, int endHeight, bool antiAlias);
	ofColor averageColors(vector<ofColor> colors);
	ofColor getColor(Scene scene, float u, float v);
	//void getColor(Scene scene, float u, float v, std::promise<ofColor> &&p);
	ofColor lambertian(SceneObject *obj, Light *light, IntersectInfo intersect);       //calculate lambertian color
	ofColor blinn_phong(SceneObject *obj, Light *light, IntersectInfo intersect);      //calculate Blinn-Phong color
	bool inShadow(vector<SceneObject*> objects, Light *light, IntersectInfo intersect);//determine if point is in a shadow

	int numDivisions = 16;
};