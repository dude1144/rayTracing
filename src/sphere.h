//Austin Anderson
//010640955
//from code given by professor
#pragma once
#include "sceneObject.h"

class Sphere : public SceneObject
{
public:
	float radius = 1.0;
	ofxGuiGroup positionGroup;
	ofxInputField<float> xInput;
	ofxInputField<float> yInput;
	ofxInputField<float> zInput;
	/*ofxGuiGroup materialGroup;
	ofxInputField<float> pInput;
	ofxColorSlider diffuseInput;
	ofxColorSlider specularInput;*/

	Sphere() 
	{
		settings.setup("Sphere");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(&mat.materialGroup);
		this->updateFromUI();
		/*materialGroup.add(pInput.setup("P", mat.p, 0, std::numeric_limits<float>::max()));
		materialGroup.add(diffuseInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		materialGroup.add(specularInput.setup("Specular", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		*/
	}
	Sphere(glm::vec3 pos, float r, ofColor col) 
	{
		position = pos; 
		radius = r; 
		mat.setDiffuseColor(col); 

		settings.setup("Sphere");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(&mat.materialGroup);
		this->updateFromUI();
		/*materialGroup.add(pInput.setup("P", mat.p, 0, std::numeric_limits<float>::max()));
		materialGroup.add(diffuseInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		materialGroup.add(specularInput.setup("Specular", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		*/
	}
	Sphere(glm::vec3 pos, float r, ofColor difCol, ofColor specCol, float p) 
	{
		position = pos; 
		radius = r; 
		mat.setDiffuseColor(difCol); 
		mat.setSpecularColor(specCol);
		mat.setP(p); 

		settings.setup("Sphere");
		settings.add(positionGroup.setup("Position"));
		positionGroup.add(xInput.setup("X", position.x, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(yInput.setup("Y", position.y, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		positionGroup.add(zInput.setup("Z", position.z, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
		settings.add(&mat.materialGroup);
		this->updateFromUI();
		/*materialGroup.add(pInput.setup("P", mat.p, 0, std::numeric_limits<float>::max()));
		materialGroup.add(diffuseInput.setup("Diffuse", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		materialGroup.add(specularInput.setup("Specular", mat.diffuseColor, ofColor(0, 0, 0), ofColor(255, 255, 255)));
		*/
	}

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) 
	{
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}

	bool intersectView(const Ray &ray, glm::vec3 &point, glm::vec3 &normal)
	{
		return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
	}
	void draw() 
	{
		//ofSetColor(mat.diffuseColor);
		ofDrawSphere(position, radius);
	}

	void updateFromUI()
	{
		position = glm::vec3((float)xInput, (float)yInput, (float)zInput);
		mat.p = mat.pInput;
		mat.diffuseColor = mat.diffuseInput;
		mat.specularColor = mat.specularInput;
	}
};
