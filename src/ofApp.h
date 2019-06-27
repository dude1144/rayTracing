//Austin Anderson
//010640955
#pragma once

#include <vector>
#include "ofTrueTypeFont.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "primitives.h"
#include "scene.h"
#include "mesh.h"
#include "viewPlane.h"
#include "renderCam.h"
#include "light.h"
#include "bounds.h"
#include "Octree.h"

class ofApp : public ofBaseApp 
{

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
	void drawAxes();
	void createSelectedSettingsPanel();
	void handleSelectedUpdate();

	int offsetX = 0;
	int offsetY = 0;
	glm::vec3 initialPos;

	//viewport cameras
	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera  *theCam; 

	//camera to render through and image to save that render to
	RenderCam renderCam;
	ofImage   image;

	//scene to hold objects and lights and a pointer to save selected SceneObject
	SceneObject *selected;
	Scene scene;

	ofxPanel* selectedSettings = nullptr;
	vector<ofxBaseGui*> settingsList;
	
	//panel to change settings, REPLACE WITH PROPER UI

	ofxPanel debugPanel;
	bool showDebugPanel;

	ofxToggle useAntiAliasing;
	ofxToggle useMultithreading;
	ofxToggle showOctree;
	ofxToggle useOctree;
	ofxFloatSlider maxLevels;



	//booleans to track state
	bool showImage = false;
	bool dragged = false;
	bool pressed = false;
	bool moving = false;
	bool bShiftPressed = false;
};
