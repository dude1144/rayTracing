#pragma once
#include "primitives.h"
#include "light.h"
#include <vector>


#define MAX_STEPS 1000
#define MISS_DIST 100
#define HIT_DIST .1
#define EPS .01

class Scene
{
public:
	vector<Intersectable*> getIntersectablesMeshAsTris();
//public:
	//list of objects
	vector<SceneObject*> objects;
	vector<Light*> lights;
	Octree tree;
	//turns all meshes into triangles
	vector<Intersectable*> getIntersectables();
	
	//void meshToTri(vector<SceneObject*> obj);


	void add(SceneObject* obj)
	{
		if (dynamic_cast<Light*>(obj))
			lights.push_back(dynamic_cast<Light*>(obj));
		else
		objects.push_back(obj);
	}

	void setupTree();


	bool remove(SceneObject* obj);

	SceneObject* intersect(const Ray &ray, IntersectInfo &intersect);
	SceneObject* rayMarch(Ray r, IntersectInfo &intersect, glm::vec3 repeat);
	float sceneSDF(glm::vec3 &p, int &index, glm::vec3 repeat);
	
};