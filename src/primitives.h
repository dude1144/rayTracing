//Austin Anderson
//010640955

#pragma once
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "material.h"
#include "Octree.h"
#include <limits>
#include <chrono>

#define maxFloat std::numeric_limits<float>::max()
#define minFloat std::numeric_limits<float>::lowest()

enum objectType {sphere, plane, mesh, light, triangle, render_cam, NOT_DEFINED};

class Triangle;

/*
 * class to hold relevent information for a Ray
 *
 * a ray is made up of a point and a direction,
 * both are stored as GLM vector 3s. the class
 * also contins convenience methods to get a 
 * point along the ray.
 *
 */
class Ray : public Intersectable
{
public:
	glm::vec3 dir;
	glm::vec3 point;
	
	Ray(glm::vec3 p, glm::vec3 d) { this->point = p; this->dir = d; }

	void draw(float t) { ofDrawLine(point, point + t * dir); }
	glm::vec3 evalPoint(float t) { return (point + t * dir); }


	void setDirection(glm::vec3 d) { dir = glm::normalize(d); }
	glm::vec3 getDir() { return dir; }	
};

/*
 * class to hold intersection return information
 *
 * the class holds the distance from the ray origin,
 * the point the intersection occured, the normal at
 * the point of intersection, and the barrycentric
 * coordinates of the intersection. not all information
 * has to be used
 *
 */
class IntersectInfo
{
public:
	float dist;
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 barry;

	IntersectInfo()
	{
		dist = maxFloat;
		point = glm::vec3(0, 0, 0);
		normal = glm::vec3(0, 0, 0);
		barry = glm::vec3(0, 0, 0);
	}
};

/*
 * base class for objects in the scene
 *
 * class sets up basic required information
 * such as name, transformations, and the material
 * for the object; and required functions for
 * drawing in the viewport and intersecion
 *
 */
class SceneObject : public Intersectable
{
public:
	std::string name;
	objectType type = NOT_DEFINED;
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	Material mat = Material(ofColor::white, ofColor::lightGrey, 10.0f);

	virtual void draw() = 0;

	virtual bool intersect(const Ray &ray, IntersectInfo &intersect) { return false; }
	virtual bool intersectView(const Ray &ray, IntersectInfo &intersect) { return this->intersect(ray, intersect); }

	float intersect(glm::vec3 point, glm::vec3 dir) override
	{
		IntersectInfo info;
		if (this->intersect(Ray(point, dir), info))
		{
			return glm::length(point -info.point);
		}
		return -1;
	}
};

class Sphere : public SceneObject
{
public:
	float radius = 1.0;

	Sphere()
	{
		name = "Sphere" + std::to_string(count);
		type = sphere;
		count++;
	}
	Sphere(glm::vec3 pos, float r)
	{
		position = pos;
		radius = r;

		name = "Sphere" + std::to_string(count);
		type = sphere;
		count++;
	}
	Sphere(glm::vec3 pos, float r, ofColor col)
	{
		position = pos;
		radius = r;
		mat.setDiffuseColor(col);

		name = "Sphere" + std::to_string(count);
		type = sphere;
		count++;
	}
	Sphere(glm::vec3 pos, float r, ofColor difCol, ofColor specCol, float p)
	{
		position = pos;
		radius = r;
		mat.setDiffuseColor(difCol);
		mat.setSpecularColor(specCol);
		mat.setP(p);

		name = "Sphere" + std::to_string(count);
		type = sphere;
		count++;
	}

	bool intersect(const Ray& ray, IntersectInfo &intersect) override;
	bool intersect(const OrientedBoundingBox& box) override;

	void draw() { ofDrawSphere(position, radius); }

private:
	static int count;
};

class Plane : public SceneObject
{
public:
	float width = 20;
	float height = 20;
	glm::vec3 normal = glm::vec3(0, 1, 0);

	Plane() 
	{ 
		name = "Plane" + std::to_string(count);
		type = plane;
		count++;
		normal = glm::vec3(0, 1, 0);
	}
	Plane(glm::vec3 pos, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, ofColor spec = ofColor::olive, float p = 1, float w = 20, float h = 20)
	{
		position = pos; normal = n;
		mat.p = p;
		width = w;
		height = h;
		mat.diffuseColor = diffuse;
		mat.specularColor = spec;

		name = "Plane" + std::to_string(count);
		type = plane;
		count++;
	}

	bool intersect(const Ray &ray, IntersectInfo &intersect);
	bool intersect(const OrientedBoundingBox& box) override;
	bool intersectView(const Ray &ray, IntersectInfo &intersect);
	
	void draw()
	{
		ofPlanePrimitive plane;
		plane.rotateDeg(90, 1, 0, 0);
		plane.setPosition(position);
		plane.setWidth(width);
		plane.setHeight(height);
		plane.setResolution(4, 4);
		plane.drawWireframe();
	}


private:
	static int count;
};

class Mesh : public SceneObject
{
public:
	

	vector<ofMesh> ofmeshes;
	vector<Triangle> tris;
	bool smooth;

	Mesh()
	{
		this->name = "Mesh" + std::to_string(count);
		type = mesh;
		count++;
		smooth = false;
	}
	Mesh(std::string name)
	{
		this->name = "Mesh" + std::to_string(count);
		type = mesh;
		count++;
		smooth = false;

		this->load(name);
	}


	bool intersect(const Ray &ray, IntersectInfo &intersect);
	bool intersect(const OrientedBoundingBox& box) override;

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

		for (int i = 0; i < ofmeshes.size(); i++)
		{
			ofmeshes[i].drawWireframe();
#if _DEBUG
			vector<glm::vec3> verts = ofmeshes[i].getVertices();
			ofColor current = ofGetStyle().color;
			ofSetColor(ofColor::hotPink);
			for (int j = 0; j < verts.size(); j++)
			{
				ofDrawSphere(verts[j], .05);
				ofDrawLine(verts[j], verts[j] + (ofmeshes[i].getNormals()[j] * .2));
			}
			ofSetColor(current);
#endif
			ofPopMatrix();
		}

	}

	bool load(std::string name);

private:
	static int count;
};

class Triangle : public Intersectable
{
public:
	Mesh* parent;
	int meshNum;
	int indices[3];
	Triangle(const Triangle& t)
	{
		this->parent = t.parent;
		this->meshNum = t.meshNum;
		this->indices[0] = t.indices[0];
		this->indices[1] = t.indices[1];
		this->indices[2] = t.indices[2];
	}
	Triangle(Mesh* parent, int meshNum, int i1, int i2, int i3)
	{
		this->parent = parent;
		this->meshNum = meshNum;
		indices[0] = i1;
		indices[1] = i2;
		indices[2] = i3;
	}

	glm::vec3 get(int i)
	{
		return parent->ofmeshes[meshNum].getVertex(indices[i]);
	}

	bool intersect(const OrientedBoundingBox& box) override;
	bool intersect(const Ray &ray, IntersectInfo &intersect);
	float intersect(glm::vec3 point, glm::vec3 dir) override 
	{ 
		glm::vec3 v1 = this->parent->getMatrix() * glm::vec4(this->get(0), 1);
		glm::vec3 v2 = this->parent->getMatrix() * glm::vec4(this->get(1), 1);
		glm::vec3 v3 = this->parent->getMatrix() * glm::vec4(this->get(2), 1);

		glm::vec3 bary;

		if (glm::intersectRayTriangle(point, dir, v1, v2, v3, bary))
		{
			bary.z = 1 - (bary.x + bary.y);
			return glm::length(((v1 * bary.z) + (v2 * bary.x) + (v3 * bary.y)) - point);
		}
		return -1;
	}
	void draw() { ofDrawTriangle(this->get(0), this->get(1), this->get(2)); }
};
