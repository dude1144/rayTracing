//Austin Anderson
//010640955
//from code given by professor
#pragma once
#include "ofMain.h"
#include "sceneObject.h"
#include "ray.h"

//  General purpose plane 
//
class Plane : public SceneObject 
{
public:
	ofPlanePrimitive plane;
	float width = 20;
	float height = 20;
	glm::vec3 normal = glm::vec3(0, 1, 0);

	ofxGuiGroup positionGroup;
	ofxInputField<float> xInput;
	ofxInputField<float> yInput;
	ofxInputField<float> zInput;
	ofxGuiGroup normalGroup;
	ofxInputField<float> xNormalInput;
	ofxInputField<float> yNormalInput;
	ofxInputField<float> zNormalInput;
	ofxGuiGroup materialGroup;
	ofxInputField<float> pInput;
	ofxColorSlider diffuseInput;
	ofxColorSlider specularInput;



	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse =ofColor::darkOliveGreen, float w = 20, float h = 20) 
	{
		position = p; normal = n;
		width = w;
		height = h;
		mat.diffuseColor = diffuse;
		plane.rotateDeg(90, 1, 0, 0);

		settings.setup("Plane");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(normalGroup.setup("Normal"));
		normalGroup.add(xNormalInput.setup("X", normal.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		normalGroup.add(yNormalInput.setup("Y", normal.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		normalGroup.add(zNormalInput.setup("Z", normal.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(materialGroup.setup("Material"));
		materialGroup.add(pInput.setup("P", mat.p, 0, std::numeric_limits<float>::max()));
		materialGroup.add(diffuseInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		materialGroup.add(specularInput.setup("Specular", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	}
	Plane(glm::vec3 pos, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, ofColor spec = ofColor::olive, float p = 1, float w = 20, float h = 20)
	{
		position = pos; normal = n;
		mat.p = p;
		width = w;
		height = h;
		mat.diffuseColor = diffuse;
		mat.specularColor = spec;
		plane.rotateDeg(90, 1, 0, 0);

		settings.setup("Plane");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(normalGroup.setup("Normal"));
		normalGroup.add(xNormalInput.setup("X", normal.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		normalGroup.add(yNormalInput.setup("Y", normal.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		normalGroup.add(zNormalInput.setup("Z", normal.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(materialGroup.setup("Material"));
		materialGroup.add(pInput.setup("P", mat.p, 0, std::numeric_limits<float>::max()));
		materialGroup.add(diffuseInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		materialGroup.add(specularInput.setup("Specular", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
	}
	Plane() { settings.setup("Plane"); }

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	bool intersectView(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);

	void draw() 
	{
		//ofSetColor(mat.diffuseColor);
		plane.setPosition(position);
		plane.setWidth(width);
		plane.setHeight(height);
		plane.setResolution(4, 4);
		plane.drawWireframe();
	}

	void updateFromUI()
	{
		position = glm::vec3((float)xInput, (float)yInput, (float)zInput);
		normal = glm::normalize(glm::vec3((float)xNormalInput, (float)yNormalInput, (float)zNormalInput));
		normal = glm::normalize(normal);
		mat.p = pInput;
		mat.diffuseColor = diffuseInput;
		mat.specularColor = specularInput;
	}

};