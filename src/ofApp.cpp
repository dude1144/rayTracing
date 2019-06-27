#include "ofApp.h"
#include <iostream>
#include <limits>



//--------------------------------------------------------------
void ofApp::setup() 
{
	//set up scene
	scene.add(new Plane(glm::vec3(0, -1.6, 0), glm::vec3(0, 1, 0), ofColor::lightGray, ofColor::gray, 20));
	scene.add(new Sphere(glm::vec3(0, 0, 0), 1, ofColor(74, 219, 94), ofColor::lightGreen, 20));
	//scene.add(new Sphere(glm::vec3(-2, 0, -2), 1.5, ofColor(255,65,65), ofColor::gray, 20));
	//scene.add(new Sphere(glm::vec3(2, 0, -1.5), 1.2, ofColor(65, 142, 255), ofColor::gray, 20));
	scene.add(new PointLight(glm::vec3(2, 3, 2), 10, ofColor(255,255,255)));
	scene.add(new PointLight(glm::vec3(-4, 2, 4), 10, ofColor(255, 255, 255)));
	
	//allocate the image for the render
	image.allocate(600, 400, OF_IMAGE_COLOR_ALPHA);

 // setup debug panel
	debugPanel.setup("DEBUG");
	debugPanel.add(useAntiAliasing.setup("Use Antialiasing", false));
	debugPanel.add(useMultithreading.setup("Use Multithreading", false));
	debugPanel.add(useOctree.setup("Use Octree", false));
	debugPanel.add(showOctree.setup("Show Octree", false));
	debugPanel.add(maxLevels.setup("Max Levels", 5, 1, 50));

	//setup view
	ofSetBackgroundColor(ofColor(50,50,50));
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
		handleSelectedUpdate();

	scene.tree.setMaxLevels(maxLevels);
}
//--------------------------------------------------------------
void ofApp::draw() 
{
	ofSetColor(ofColor(255, 255, 255));
	if (showImage)
		image.draw(0, 0, ofGetWidth(), ofGetHeight());

	else
	{
		//enable depth testing and turn off wireframe mode
		ofEnableDepthTest();
		ofNoFill();

		//start the camera and draw the axes
		theCam->begin();

			drawAxes();
		
			//draw the render cam
			ofSetColor(ofColor::lightSkyBlue);
			renderCam.drawFrustum();
			ofSetColor(ofColor::blue);
			renderCam.draw();

			//turn fill back on
			ofFill();

			//draw the scene
			scene.draw(selected, showOctree);

		//end the camera
		theCam->end();

		// draw debug panel
		if (showDebugPanel)
		{
			ofDisableDepthTest();
			debugPanel.draw();
		}
		//draw selected object's GUI
		if (selected)
		{
			ofDisableDepthTest();
			selectedSettings->draw();
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
	switch (key)
	{
	case 'O':
	case 'o':
		renderCam.useOctree = true;
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
	case 'I':
	case 'i':
		showImage = !showImage;
		break;
	case 'R':
	case 'r':
	{
		std::cout << "building Octree" << std::endl;
		scene.setupTree();
		std::cout << "Octree built" << std::endl;
		image.allocate(600, 400, OF_IMAGE_COLOR_ALPHA);
#if _DEBUG
		renderCam.renderImage(&scene, &image, useAntiAliasing, useMultithreading);
#else
		renderCam.renderImage(&scene, &image, false, true);
#endif
		break;
	}
	case 'X':
	case 'x':
		if(!moving)
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
	case 'T':
	case 't':
		{
		scene.setupTree();
		break;
		}
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
		//toggle debug panel and update position 
#if _DEBUG 
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
		mainCam.disableMouseInput();
		Ray selectRay = Ray(theCam->getPosition(), glm::normalize(theCam->screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0)) - theCam->getPosition()));
		IntersectInfo temp;
		Plane(initialPos, theCam->getLookAtDir()*-1).intersect(selectRay, temp);
		selected->position = temp.point;
	}
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) 
{
	pressed = true;

	if (selected)
	{
		//in the mouse is clicked while on a panel, disable the camera
		if ((selectedSettings && selectedSettings->getShape().inside(x,y)) || (showDebugPanel && debugPanel.getShape().inside(x, y)))
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
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) 
{
	pressed = false;
	moving = false;

	if (!mainCam.getMouseInputEnabled())
		mainCam.enableMouseInput();

	if (dragged)
		dragged = false;
	else
	{
		dragged = false;

		//get a ray from camera position to viewport for selection and make a variable to keep distance
		Ray selectRay = Ray(theCam->getPosition(), glm::normalize(theCam->screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0)) - theCam->getPosition()));
		float dist = maxFloat;

		SceneObject* s = scene.intersectSelect(selectRay);

		if (selected != s)
		{
			selected = s;
			this->createSelectedSettingsPanel();
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

void ofApp::drawAxes()
{
	ofSetColor(ofColor::green);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	ofSetColor(ofColor::blue);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	ofSetColor(ofColor::red);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
}

void ofApp::createSelectedSettingsPanel()
{
	//delete all parts of the previous settings panel and clear the vector

	if (selected)
	{
		delete selectedSettings;
		for (int i = 0; i < settingsList.size(); i++)
			delete settingsList[i];
		settingsList.clear();
		
		//create new panel for settings
		selectedSettings = new ofxPanel();
		selectedSettings->setup(selected->name + " settings");

		//setup basic parts that all SceneObjects have
		// position
		settingsList.push_back(new ofxGuiGroup());
		settingsList.push_back(new ofxInputField<float>());
		settingsList.push_back(new ofxInputField<float>());
		settingsList.push_back(new ofxInputField<float>());
		// rotation
		settingsList.push_back(new ofxGuiGroup());
		settingsList.push_back(new ofxInputField<float>());
		settingsList.push_back(new ofxInputField<float>());
		settingsList.push_back(new ofxInputField<float>());
		// scale 
		settingsList.push_back(new ofxGuiGroup());
		settingsList.push_back(new ofxInputField<float>());
		settingsList.push_back(new ofxInputField<float>());
		settingsList.push_back(new ofxInputField<float>());

		//setup everything
		((ofxGuiGroup*)settingsList[0])->setup("Position");
		((ofxGuiGroup*)settingsList[0])->add(((ofxInputField<float>*)settingsList[1])->setup("X", selected->position.x, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[0])->add(((ofxInputField<float>*)settingsList[2])->setup("Y", selected->position.y, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[0])->add(((ofxInputField<float>*)settingsList[3])->setup("Z", selected->position.z, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[4])->setup("Rotation");
		((ofxGuiGroup*)settingsList[4])->add(((ofxInputField<float>*)settingsList[5])->setup("X", selected->position.x, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[4])->add(((ofxInputField<float>*)settingsList[6])->setup("Y", selected->position.y, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[4])->add(((ofxInputField<float>*)settingsList[7])->setup("Z", selected->position.z, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[8])->setup("Scale");
		((ofxGuiGroup*)settingsList[8])->add(((ofxInputField<float>*)settingsList[9])->setup("X", selected->position.x, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[8])->add(((ofxInputField<float>*)settingsList[10])->setup("Y", selected->position.y, minFloat, maxFloat));
		((ofxGuiGroup*)settingsList[8])->add(((ofxInputField<float>*)settingsList[11])->setup("Z", selected->position.z, minFloat, maxFloat));

		//add these to the gui
		selectedSettings->add(settingsList[0]);
		selectedSettings->add(settingsList[4]);
		selectedSettings->add(settingsList[8]);
		
		//handle everything else, length of settingsList is 11 at this point
		switch (selected->type)
		{
			case sphere:
			{
				// radius
				settingsList.push_back(new ofxInputField<float>());
				selectedSettings->add(((ofxInputField<float>*)settingsList[12])->setup("Radius", ((Sphere*)selected)->radius, 0, maxFloat));
				break;
			}
			case plane:
			{
				// normal
				settingsList.push_back(new ofxGuiGroup());
				settingsList.push_back(new ofxInputField<float>());
				settingsList.push_back(new ofxInputField<float>());
				settingsList.push_back(new ofxInputField<float>());

				((ofxGuiGroup*)settingsList[12])->setup("Normal");
				((ofxGuiGroup*)settingsList[12])->add(((ofxInputField<float>*)settingsList[13])->setup("X", ((Plane*)selected)->normal.x, minFloat, maxFloat));
				((ofxGuiGroup*)settingsList[12])->add(((ofxInputField<float>*)settingsList[14])->setup("Y", ((Plane*)selected)->normal.y, minFloat, maxFloat));
				((ofxGuiGroup*)settingsList[12])->add(((ofxInputField<float>*)settingsList[15])->setup("Z", ((Plane*)selected)->normal.z, minFloat, maxFloat));

				selectedSettings->add(settingsList[12]);
				break;
			}
			case mesh:
			{
				//smooth toggle
				settingsList.push_back(new ofxToggle());
				selectedSettings->add(((ofxToggle*)settingsList[12])->setup("Smooth", ((Mesh*)selected)->smooth));
				break;
			}
			case light:
			{

				break;
			}
			case render_cam:
			{

				break;
			}
		}
	}
}

void ofApp::handleSelectedUpdate()
{
	switch (selected->type)
	{
		case sphere:
		{
			Sphere* castSelected = (Sphere*)selected;

			castSelected->position.x = *((ofxInputField<float>*)settingsList[1]);
			castSelected->position.y = *((ofxInputField<float>*)settingsList[2]);
			castSelected->position.z = *((ofxInputField<float>*)settingsList[3]);

			castSelected->rotation.x = *((ofxInputField<float>*)settingsList[5]);
			castSelected->rotation.y = *((ofxInputField<float>*)settingsList[6]);
			castSelected->rotation.z = *((ofxInputField<float>*)settingsList[7]);

			castSelected->scale.x = *((ofxInputField<float>*)settingsList[9]);
			castSelected->scale.y = *((ofxInputField<float>*)settingsList[10]);
			castSelected->scale.z = *((ofxInputField<float>*)settingsList[11]);

			castSelected->radius = *((ofxInputField<float>*)settingsList[12]);

			break;
		}
		case plane:
		{
			Plane* castSelected = (Plane*)selected;

			castSelected->position.x = *((ofxInputField<float>*)settingsList[1]);
			castSelected->position.y = *((ofxInputField<float>*)settingsList[2]);
			castSelected->position.z = *((ofxInputField<float>*)settingsList[3]);

			castSelected->rotation.x = *((ofxInputField<float>*)settingsList[5]);
			castSelected->rotation.y = *((ofxInputField<float>*)settingsList[6]);
			castSelected->rotation.z = *((ofxInputField<float>*)settingsList[7]);

			castSelected->scale.x = *((ofxInputField<float>*)settingsList[9]);
			castSelected->scale.y = *((ofxInputField<float>*)settingsList[10]);
			castSelected->scale.z = *((ofxInputField<float>*)settingsList[11]);

			float x = *((ofxInputField<float>*)settingsList[13]);
			float y = *((ofxInputField<float>*)settingsList[14]);
			float z = *((ofxInputField<float>*)settingsList[15]);

			castSelected->normal = glm::normalize(glm::vec3(x, y, z));

			break;
		}
		case mesh:
		{
			Mesh* castSelected = (Mesh*)selected;

			castSelected->position.x = *((ofxInputField<float>*)settingsList[1]);
			castSelected->position.y = *((ofxInputField<float>*)settingsList[2]);
			castSelected->position.z = *((ofxInputField<float>*)settingsList[3]);

			castSelected->rotation.x = *((ofxInputField<float>*)settingsList[5]);
			castSelected->rotation.y = *((ofxInputField<float>*)settingsList[6]);
			castSelected->rotation.z = *((ofxInputField<float>*)settingsList[7]);

			castSelected->scale.x = *((ofxInputField<float>*)settingsList[9]);
			castSelected->scale.y = *((ofxInputField<float>*)settingsList[10]);
			castSelected->scale.z = *((ofxInputField<float>*)settingsList[11]);

			castSelected->smooth = *((ofxToggle*)settingsList[12]);

			break;
		}
	}
}
