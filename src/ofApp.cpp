#include "ofApp.h"
#include "ray.h"
#include "sceneObject.h"
#include "stdlib.h"
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
	objects.push_back(new Sphere(glm::vec3(-.5, 0, 0), 1, ofColor::green));
	objects.push_back(new Sphere(glm::vec3(-2, 0, -2), 1.5, ofColor::red));
	objects.push_back(new Sphere(glm::vec3(2, 0, -1.5), 1.2, ofColor::blue));
	objects.push_back(new Plane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0), ofColor::lightGray));

	image.allocate(1200, 800, OF_IMAGE_COLOR_ALPHA);

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
}
//--------------------------------------------------------------
void ofApp::draw() {
	if (showImage)
		image.draw(0,0);
	else
	{
		theCam->begin();
		ofSetColor(ofColor::green);
		ofNoFill();

		ofSetColor(ofColor::lightSkyBlue);
		renderCam.drawFrustum();
		ofSetColor(ofColor::blue);
		renderCam.draw();

		for (int i = 0; i < objects.size(); i++)
		{
			ofSetColor(objects[i]->diffuseColor);
			objects[i]->draw();
		}

		theCam->end();
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
	case 'R':
	case 'r':
		renderCam.renderImage(objects, &image);
		image.save("render.png", OF_IMAGE_QUALITY_BEST);
		image.load("render.png");
		break;
	case 'C':
	case 'c':
		if (mainCam.getMouseInputEnabled())
			mainCam.disableMouseInput();
		else mainCam.enableMouseInput();
		break;
	case 'F':
	case 'b':
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
void ofApp::mouseDragged(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
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
void ofApp::dragEvent(ofDragInfo dragInfo) {
}