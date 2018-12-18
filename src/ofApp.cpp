#include "ofApp.h"
#include <iostream>
#include <limits>

//--------------------------------------------------------------
void ofApp::setup() 
{
	scene.add(new Plane(glm::vec3(0, -1.6, 0), glm::vec3(0, 1, 0), ofColor::lightGray, ofColor::gray, 20));
	scene.add(new Sphere(glm::vec3(0, 0, 0), 1, ofColor(74, 219, 94), ofColor::gray, 20));
	scene.add(new Sphere(glm::vec3(-2, 0, -2), 1.5, ofColor(255,65,65), ofColor::gray, 20));
	scene.add(new Sphere(glm::vec3(2, 0, -1.5), 1.2, ofColor(65, 142, 255), ofColor::gray, 20));
	scene.add(new PointLight(glm::vec3(2, 3, 2), 10, ofColor(255,255,255)));
	scene.add(new PointLight(glm::vec3(-4, 2, 4), 10, ofColor(255, 255, 255)));
	
	image.allocate(1200, 800, OF_IMAGE_COLOR_ALPHA);
	//image.allocate(600, 400, OF_IMAGE_COLOR_ALPHA);
	//image.allocate(120, 80, OF_IMAGE_COLOR_ALPHA);
	//image.allocate(60, 40, OF_IMAGE_COLOR_ALPHA);
	//image.allocate(6, 4, OF_IMAGE_COLOR_ALPHA);

	addPanel.setup("ADD");
	addPanel.add(addSphere.setup("Add Sphere"));
	addPanel.add(addPlane.setup("Add Plane"));
	addPanel.add(addLight.setup("Add Light"));
	
	addSphere.addListener(this, &ofApp::pressedAddSphere);
	addPlane.addListener(this, &ofApp::pressedAddPlane);
	addLight.addListener(this, &ofApp::pressedAddLight);

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

		if (selected)
		{
			ofDisableDepthTest();
			selected->settings.draw();
		}
		if (bShowAddPanel)
		{
			ofDisableDepthTest();
			addPanel.draw();
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
	switch (key)
	{
	case 'A':
	case 'a':
		if (bShiftPressed && !moving)
		{
			addPanel.setPosition(ofGetMouseX(), ofGetMouseY());
			bShowAddPanel = true;
		}
		break;
	case 'G':
	case 'g':
		break;
	case OF_KEY_SHIFT:
		bShiftPressed = true;
		break;
	}
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) 
{
	switch (key) 
	{
	case 'L':
	case 'l':
		pressedAddLight();
		break;
	case 'S':
	case 's':
		pressedAddSphere();
		break;
	case 'P':
	case 'p':
		pressedAddPlane();
		break;
	case 'I':
	case 'i':
		showImage = !showImage;
		break;
	case 'R':
	case 'r':
		renderCam.renderImage(scene, &image, true);
		image.save("render.png", OF_IMAGE_QUALITY_BEST);
		image.load("render.png");
		break;
	case 'X':
	case 'x':
		if (moving)
		{

		}
		else
		{
			if (selected)
			{
				if (scene.remove(selected))
				{
					selected = nullptr;
				}
			}
		}
		break;
	case OF_KEY_SHIFT:
		bShiftPressed = false;
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
	if (selected && moving)
	{
#if _DEBUG
		cout << "moving" << endl;
#endif
		mainCam.disableMouseInput();
		Ray selectRay = Ray(theCam->getPosition(), glm::normalize(theCam->screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0)) - theCam->getPosition()));
		IntersectInfo temp;
		Plane(initialPos, theCam->getLookAtDir()*-1).intersect(selectRay, temp);
		selected->position = temp.point;
		selected->xInput = temp.point.x;
		selected->yInput = temp.point.y;
		selected->zInput = temp.point.z;
	}
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
		else
		{
			Ray selectRay = Ray(theCam->getPosition(), glm::normalize(theCam->screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0)) - theCam->getPosition()));
			if (selected->intersectView(selectRay, IntersectInfo()))
			{
				initialPos = selected->position;
				moving = true;
			}
		}
	}
#if _DEBUG // check if mouse is on debug panel
	if (showDebugPanel)
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
	moving = false;
	bShowAddPanel = false;

	if (!mainCam.getMouseInputEnabled())
		mainCam.enableMouseInput();
	if (dragged)
		dragged = false;
	else
	{
		selected = nullptr;
		dragged = false;
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

void ofApp::pressedAddSphere()
{
	scene.add(new Sphere());
	bShowAddPanel = false;
	mainCam.enableMouseInput();
}
void ofApp::pressedAddPlane()
{
	scene.add(new Plane());
	bShowAddPanel = false;
	mainCam.enableMouseInput();
}
void ofApp::pressedAddLight()
{
	scene.add(new PointLight());
	bShowAddPanel = false;
	mainCam.enableMouseInput();
}
