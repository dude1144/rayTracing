#include "ofApp.h"
#include <iostream>
#include <limits>
// Intersect Ray with Plane  (wrapper on glm::intersect*
//
// Convert (u, v) to (x, y, z)
// We assume u,v is in [0, 1]
//

// Get a ray from the current camera position to the (u, v) position on
// the ViewPlane
//



// This could be drawn a lot simpler but I wanted to use the getRay call
// to test it at the corners.
//

//--------------------------------------------------------------
void ofApp::setup() 
{
	objects.push_back(new Sphere(glm::vec3(0, 0, 0), 1, ofColor(74, 219, 94), ofColor::gray, 20));
	objects.push_back(new Sphere(glm::vec3(-2, 0, -2), 1.5, ofColor(255,65,65), ofColor::gray, 20));
	objects.push_back(new Sphere(glm::vec3(2, 0, -1.5), 1.2, ofColor(65, 142, 255), ofColor::gray, 20));
	objects.push_back(new Plane(glm::vec3(0, -1.6, 0), glm::vec3(0, 1, 0), ofColor::lightGray, ofColor::gray, 20));
	lights.push_back(new PointLight(glm::vec3(2, 3, 2), 10, ofColor(255,255,255)));
	lights.push_back(new PointLight(glm::vec3(-4, 2, 4), 10, ofColor(255, 255, 255)));

	image.allocate(1200, 800, OF_IMAGE_COLOR_ALPHA);

	//gui.setup();
	/*
	gui.add(obj1P.setup("Object 1 P", 20.0, 0, std::numeric_limits<float>::max()));
	gui.add(obj2P.setup("Object 2 P", 20.0, 0, std::numeric_limits<float>::max()));
	gui.add(obj3P.setup("Object 3 P", 20.0, 0, std::numeric_limits<float>::max()));
	gui.add(light1Intensity.setup("Light 1 Intensity", 10.0, 0, std::numeric_limits<float>::max()));
	gui.add(light2Intensity.setup("Light 2 Intensity", 10.0, 0, std::numeric_limits<float>::max()));
	*/

	ofSetBackgroundColor(ofColor::black);
	mainCam.setDistance(30);
	mainCam.setNearClip(.1);
	sideCam.setPosition(40, 0, 0);
	sideCam.lookAt(glm::vec3(0, 0, 0));
	theCam = &mainCam;

}
//--------------------------------------------------------------
void ofApp::update() 
{
	if (selected != nullptr)
		selected->updateFromUI();

	/*
	objects[0]->mat.p = obj1P;
	objects[1]->mat.p = obj2P;
	objects[2]->mat.p = obj3P;
	lights[0]->intensity = light1Intensity;
	lights[1]->intensity = light2Intensity;
	*/
}
//--------------------------------------------------------------
void ofApp::draw() {
	if (showImage)
		image.draw(0,0);
	else
	{
		ofEnableDepthTest();
		

		theCam->begin();
		ofNoFill();

		ofSetColor(ofColor::lightSkyBlue);
		renderCam.drawFrustum();
		ofSetColor(ofColor::blue);
		renderCam.draw();

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i] == selected)
				ofSetColor(ofColor(247, 189, 0));
			else
				ofSetColor(objects[i]->mat.diffuseColor);

			objects[i]->draw();
		}

		for (int i = 0; i < lights.size(); i++)
		{
			if(lights[i] == selected)
				ofSetColor(ofColor(247, 189, 0));
			else
				ofSetColor(lights[i]->mat.diffuseColor);
			lights[i]->draw();
		}

		ofSetColor(ofColor::gray);
		mainCam.drawFrustum();


		theCam->end();

		if (selected != nullptr && showGui)
		{
			ofDisableDepthTest();
			selected->settings.draw();
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) 
	{
	case 'I':
	case 'i':
		showImage = !showImage;
		break;
	case 'Z':
	case 'z':
		break;
	case 'R':
	case 'r':
		renderCam.renderImage(objects, &image, lights);
		image.save("render.png", OF_IMAGE_QUALITY_BEST);
		image.load("render.png");
		break;
	case 'C':
	case 'c':
		if (mainCam.getMouseInputEnabled())
			mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
		break;
	case 'G':
	case 'g':
		showGui = !showGui;
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'h':
		//bHide = !bHide;
		break;
	case OF_KEY_F1:
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	}
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) 
{
	dragged = true;
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) 
{
	pressed = true;
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) 
{
	pressed = false;
	if (dragged)
		dragged = false;
	else
	{
		//get a ray from camera position to viewport for selection
		Ray selectRay = Ray(theCam->getPosition(), glm::normalize(theCam->screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0)) - theCam->getPosition()));
		for (int i = 0; i < lights.size(); i++)
		{
			if (lights[i]->intersectView(selectRay, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)))
			{
				selected = lights[i];
				return;
			}
		}
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->intersectView(selectRay, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)))
			{
				selected = objects[i];
				return;
			}
		}
		selected = nullptr;
	}
}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) 
{
}