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
	Scene scene;
	BoundingBox bound = BoundingBox(glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1),
		glm::vec3(-1, -1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, 1), glm::vec3(1, -1, 1));

	ofxPanel *gui;

#if _DEBUG // debug tools

	ofxPanel debugPanel;
	bool showDebugPanel;

	ofxToggle showIntersectionPoints;
	ofxToggle showIntersectionNormals;
	ofxToggle useAntiAliasing;

	vector<IntersectInfo> intersections;
#endif

	// set up one render camera to render image throughn
	//
	RenderCam renderCam;
};
