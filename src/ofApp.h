//Austin Anderson
//010640955
#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxGui.h"
#include "ray.h"
#include "sceneObject.h"
#include "sphere.h"
#include "mesh.h"
#include "Plane.h"
#include "viewPlane.h"
#include "renderCam.h"
#include "light.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	bool showImage = false;
	bool showGui = true;
	bool dragged = false;
	bool pressed = false;

	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam
	ofImage   image;
	SceneObject *selected;
	vector<SceneObject*> objects;
	vector<Light*> lights;

	ofxPanel gui;
	ofxFloatSlider obj1P;
	ofxFloatSlider obj2P;
	ofxFloatSlider obj3P;
	ofxFloatSlider light1Intensity;
	ofxFloatSlider light2Intensity;

	// set up one render camera to render image throughn
	//
	RenderCam renderCam;
};
