#include "ofApp.h"
#include <iostream>
#include <limits>

//--------------------------------------------------------------
void ofApp::setup() 
{
	/*
	scene.add(new Sphere(glm::vec3(0, 0, 0), 1, ofColor(74, 219, 94), ofColor::gray, 20));
	scene.add(new Sphere(glm::vec3(-2, 0, -2), 1.5, ofColor(255,65,65), ofColor::gray, 20));
	scene.add(new Sphere(glm::vec3(2, 0, -1.5), 1.2, ofColor(65, 142, 255), ofColor::gray, 20));*/
	scene.add(new PointLight(glm::vec3(2, 3, 2), 10, ofColor(255,255,255)));
	scene.add(new Plane(glm::vec3(0, -1.6, 0), glm::vec3(0, 1, 0), ofColor::lightGray, ofColor::gray, 20));
	scene.add(new PointLight(glm::vec3(-4, 2, 4), 10, ofColor(255, 255, 255)));
	
	//image.allocate(1200, 800, OF_IMAGE_COLOR_ALPHA);
	image.allocate(600, 400, OF_IMAGE_COLOR_ALPHA);
	//image.allocate(120, 80, OF_IMAGE_COLOR_ALPHA);
	//image.allocate(60, 40, OF_IMAGE_COLOR_ALPHA);
	//image.allocate(6, 4, OF_IMAGE_COLOR_ALPHA);

#if _DEBUG // setup debug panel
	debugPanel.setup("DEBUG", "settings.xml");
	debugPanel.add(showIntersectionPoints.setup("Show Intersect Points", false));
	debugPanel.add(showIntersectionNormals.setup("Show Intersect Norms", false));
	debugPanel.add(useAntiAliasing.setup("Use Antialiasing", false));
#endif

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
	if (selected)
		selected->updateFromUI();
}
//--------------------------------------------------------------
void ofApp::draw() 
{
	ofSetColor(ofColor(255, 255, 255));
	if (showImage)
		image.draw(0,0, ofGetWidth(), ofGetHeight());

	else
	{
		ofEnableDepthTest();
		

		theCam->begin();
		ofNoFill();

		//bound.draw();
		ofSetColor(ofColor::lightSkyBlue);
		renderCam.drawFrustum();
		ofSetColor(ofColor::blue);
		renderCam.draw();

#if _DEBUG //draw intersection points
		if (showIntersectionPoints)
		{
			ofSetColor(ofColor(204, 0, 153));
			for (int i = 0; i < intersections.size(); i++)
			{
				ofDrawSphere(intersections[i].point, .05);
			}
		}
		if (showIntersectionNormals)
		{
			ofSetColor(ofColor(204, 0, 153));
			for (int i = 0; i < intersections.size(); i++)
			{
				ofDrawLine(intersections[i].point, intersections[i].point + (intersections[i].normal * .2));
			}
		}
#endif

		for (int i = 0; i < scene.objects.size(); i++)
		{
			if (scene.objects[i] == selected)
				ofSetColor(ofColor(247, 189, 0));
			else
				ofSetColor(scene.objects[i]->mat.diffuseColor);

			scene.objects[i]->draw();
		}

		for (int i = 0; i < scene.lights.size(); i++)
		{
			if(scene.lights[i] == selected)
				ofSetColor(ofColor(247, 189, 0));
			else
				ofSetColor(scene.lights[i]->mat.diffuseColor);
			scene.lights[i]->draw();
		}

		ofSetColor(ofColor::gray);
		mainCam.drawFrustum();

		theCam->end();
#if _DEBUG // draw debug panel
		if (showDebugPanel)
		{
			ofDisableDepthTest();
			debugPanel.draw();
		}
#endif

		if (selected && showGui)
		{
			ofDisableDepthTest();
			selected->settings.draw();
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) 
{
	switch (key) 
	{
	case 'T':
	case 't':
	{
		glm::vec3 barry = glm::vec3(0,0,0);

		glm::intersectRayTriangle(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(1, 1, 1), glm::vec3(-1, 0, 1), glm::vec3(-1, 0, -1), barry);
		barry.z = 1 - (barry.x + barry.y);
		glm::vec3 point = (glm::vec3(-1, 0, 1) * barry.x) + (glm::vec3(-1, 0, -1) * barry.y)+ (glm::vec3(1, 1, 1) * barry.z);
		cout << barry << ", " <<point << endl;

		break;
	}
	case 'I':
	case 'i':
		showImage = !showImage;
		break;
	case 'Z':
	case 'z':
		break;
	case 'R':
	case 'r':
		renderCam.renderImage(scene, &image, useAntiAliasing);
		image.save("render.png", OF_IMAGE_QUALITY_BEST);
		image.load("render.png");
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
	case OF_KEY_F10:
#if _DEBUG //toggle debug panel and update position 
		showDebugPanel = !showDebugPanel;
		debugPanel.setPosition(ofGetWindowWidth() - (debugPanel.getWidth() + 5), 10);
#endif
		break;
	}
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) 
{
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

	if (selected)
	{
		if (selected->settings.getShape().inside(x, y))
		{
			mainCam.disableMouseInput();
		}
	}
#if _DEBUG // check if mouse is on debug panel
	else if (showDebugPanel)
	{
		if (debugPanel.getShape().inside(x, y))
		{
			mainCam.disableMouseInput();
		}
	}
#endif
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) 
{
	pressed = false;

	if (!mainCam.getMouseInputEnabled())
		mainCam.enableMouseInput();
	if (dragged)
		dragged = false;
	else
	{
		selected = nullptr;
		//get a ray from camera position to viewport for selection
		Ray selectRay = Ray(theCam->getPosition(), glm::normalize(theCam->screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0)) - theCam->getPosition()));
		float dist = std::numeric_limits<float>::max();
#if _DEBUG //clear intersection list
		intersections.clear();
#endif
		for (int i = 0; i < scene.lights.size(); i++)
		{
			IntersectInfo temp;
			if (scene.lights[i]->intersectView(selectRay, temp))
			{
#if _DEBUG //add point to intersection list
				intersections.push_back(temp);
#endif
				if (temp.dist < dist)
				{
					selected = scene.lights[i];
					dist = temp.dist;
				}
			}
		}
		for (int i = 0; i < scene.objects.size(); i++)
		{
			IntersectInfo temp;
			if (scene.objects[i]->intersectView(selectRay, temp))
			{
#if _DEBUG //add point to intersection list
				intersections.push_back(temp);
#endif
				if (temp.dist < dist)
				{
					selected = scene.objects[i];
					dist = temp.dist;
				}
			}
		}
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
	scene.add(new Mesh(dragInfo.files[0]));
}
