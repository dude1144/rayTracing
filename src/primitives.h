//Austin Anderson
//010640955

#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "material.h"
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
		name = "Sphere" + count;
		count++;
		this->setupUI();
		
	}
	Sphere(glm::vec3 pos, float r, ofColor col)
	{
		position = pos;
		radius = r;
		mat.setDiffuseColor(col);

		name = "Sphere" + count;
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

		name = "Sphere" + count;
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
		normal = glm::vec3(0, 1, 0);
		plane.rotateDeg(90, 1, 0, 0);
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

		name = "Plane" + count;
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

#if _DEBUG // draw corners of plane
		glm::vec3 p1 = glm::vec3((0 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2));
		glm::vec3 p2 = glm::vec3((0 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2));
		glm::vec3 p3 = glm::vec3((1 * width) + -1 * (width / 2), position.y, (1 * height) + -1 * (height / 2));
		glm::vec3 p4 = glm::vec3((1 * width) + -1 * (width / 2), position.y, (0 * height) + -1 * (height / 2));

		ofSetColor(ofColor::red);
		ofDrawSphere(p1, .1);
		ofSetColor(ofColor::green);
		ofDrawSphere(p2, .1);
		ofSetColor(ofColor::blue);
		ofDrawSphere(p3, .1);
		ofSetColor(ofColor::purple);
		ofDrawSphere(p4, .1);
#endif
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

class Mesh : public SceneObject
{
public:
	ofxAssimpModelLoader model;
	ofxToggle smooth;

	Mesh(std::string name)
	{
		this->name = "Mesh" + std::to_string(count);
		count++;
		smooth = false;

		this->load(name);
		//model.setPosition(position.x, position.y, position.z);
		model.setPosition(1, 1, 1);
		this->setupUI();
	}


	bool intersect(const Ray &ray, IntersectInfo &intersect);
	bool intersectView(const Ray &ray, IntersectInfo &intersect);

	glm::mat4 getRotateMatrix() 
	{
		return (glm::eulerAngleYXZ(glm::radians(0.0f), glm::radians(0.0f), glm::radians(0.0f)));
	}
	glm::mat4 getTranslateMatrix() 
	{
		return (glm::translate(glm::mat4(1.0), glm::vec3(position.x, position.y, position.z)));
	}
	glm::mat4 getScaleMatrix() 
	{
		return (glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 1)));
	}

	glm::mat4 getMatrix() 
	{
		glm::mat4 scale = getScaleMatrix();
		glm::mat4 rotate = getRotateMatrix();
		glm::mat4 trans = getTranslateMatrix();

		return (trans * rotate * scale);

	}

	void draw()
	{
		ofPushMatrix();
		ofMultMatrix(getMatrix());

		for (int i = 0; i < model.getNumMeshes(); i++)
		{
			model.getMesh(i).drawWireframe();
#if _DEBUG
			vector<glm::vec3> verts = model.getMesh(i).getVertices();
			ofColor current = ofGetStyle().color;
			ofSetColor(ofColor::hotPink);
			for (int j = 0; j < verts.size(); j++)
			{
				ofDrawSphere(verts[j], .05);
				ofDrawLine(verts[j], verts[j] + (model.getMesh(i).getNormals()[j] * .2));
			}
			ofSetColor(current);
#endif
			ofPopMatrix();
		}

	}

	bool load(std::string name);

	void updateFromUI()
	{
		position = glm::vec3((float)xInput, (float)yInput, (float)zInput);
		model.setPosition(xInput, yInput, zInput);
		mat.p = mat.pInput;
		mat.diffuseColor = mat.diffuseInput;
		mat.specularColor = mat.specularInput;
	}

private:
	static int count;
	void setupUI();
};