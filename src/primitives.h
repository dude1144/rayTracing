//Austin Anderson
//010640955

#pragma once
#include "ofMain.h"
#include "material.h"
#include "ofxGui.h"
#include <limits>

class Ray
{
public:
	Ray(glm::vec3 p, glm::vec3 d) { this->p = p; this->d = d; }
	void draw(float t) { ofDrawLine(p, p + t * d); }

	glm::vec3 evalPoint(float t)
	{
		return (p + t * d);
	}

	glm::vec3 p, d;
};

class IntersectInfo
{
public:
	float dist;
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 barry;

	IntersectInfo()
	{
		dist = std::numeric_limits<float>::max();
		point = glm::vec3(0, 0, 0);
		normal = glm::vec3(0, 0, 0);
		barry = glm::vec3(0, 0, 0);
	}
};

class SceneObject
{
public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(0, 0, 0);

	Material mat = Material(ofColor::white, ofColor::lightGrey, 10.0f);

	ofxPanel settings;
	ofxGuiGroup positionGroup;
	ofxInputField<float> xInput;
	ofxInputField<float> yInput;
	ofxInputField<float> zInput;
	std::string name;

	virtual void draw() = 0;
	virtual void updateFromUI() = 0;

	//point and normal save the point the ray intersects as well as the normal at that point
	virtual bool intersect(const Ray &ray, IntersectInfo &intersect) { return false; }
	virtual bool intersectView(const Ray &ray, IntersectInfo &intersect) { return false; }
};

class Sphere : public SceneObject
{
public:
	//class members
	float radius = 1.0;

	Sphere()
	{
		name = "Sphere" + std::to_string(count);
		count++;
		this->setupUI();
		
	}
	Sphere(glm::vec3 pos, float r, ofColor col)
	{
		position = pos;
		radius = r;
		mat.setDiffuseColor(col);

		name = "Sphere" + std::to_string(count);
		count++;
		this->setupUI();
	}
	Sphere(glm::vec3 pos, float r, ofColor difCol, ofColor specCol, float p)
	{
		position = pos;
		radius = r;
		mat.setDiffuseColor(difCol);
		mat.setSpecularColor(specCol);
		mat.setP(p);

		name = "Sphere" + std::to_string(count);
		count++;
		this->setupUI();
	}

	bool intersect(const Ray &ray, IntersectInfo &intersect);
	bool intersectView(const Ray &ray, IntersectInfo &intersect);


	void draw()
	{
		ofDrawSphere(position, radius);
	}

	void updateFromUI()
	{
		position = glm::vec3((float)xInput, (float)yInput, (float)zInput);
		mat.p = mat.pInput;
		mat.diffuseColor = mat.diffuseInput;
		mat.specularColor = mat.specularInput;
	}
private:
	static int count;
	void setupUI();
};

class Plane : public SceneObject
{
public:
	ofPlanePrimitive plane;
	float width = 20;
	float height = 20;
	glm::vec3 normal = glm::vec3(0, 1, 0);

	ofxGuiGroup normalGroup;
	ofxInputField<float> xNormalInput;
	ofxInputField<float> yNormalInput;
	ofxInputField<float> zNormalInput;

	Plane() 
	{ 
		name = "Plane" + count;
		count++;
		this->setupUI();
	}
	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, float w = 20, float h = 20)
	{
		position = p; 
		normal = n;
		width = w;
		height = h;
		mat.diffuseColor = diffuse;
		plane.rotateDeg(90, 1, 0, 0);

		name = "Plane" + std::to_string(count);
		count++;
		this->setupUI();
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

		name = "Plane" + std::to_string(count);
		count++;
		this->setupUI();
	}

	bool intersect(const Ray &ray, IntersectInfo &intersect);
	bool intersectView(const Ray &ray, IntersectInfo &intersect);

	void draw()
	{
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
		mat.p = mat.pInput;
		mat.diffuseColor = mat.diffuseInput;
		mat.specularColor = mat.specularInput;
	}
private:
	static int count;
	void setupUI();
};